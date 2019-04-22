#include "stm32f10x.h"
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>

int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	// 1 Включение тактирования 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
	// 2 Настройка пинов
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 3 Настройка SysTick Timer
	if(SysTick_Config(SystemCoreClock / 1000))
	while(1);

	while (1) {
		static int ledval = 0;
		static int buttonval = 0;
		
		// 4 Переключение светодиода
		GPIO_WriteBit(GPIOC, GPIO_Pin_14, (ledval) ? Bit_SET : Bit_RESET);
		ledval = 1 - ledval;
		
		// отслеживание состояния кнопки
		buttonval = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10);	
		GPIO_WriteBit(GPIOC, GPIO_Pin_15, (buttonval)  ? Bit_SET : Bit_RESET);
		Delay(250);   // wait n ms
	}
}

// 5 таймер
static __IO uint32_t TimingDelay;

void Delay(uint32_t nTime) 
{
	TimingDelay = nTime;
	while (TimingDelay != 0) ;
}

void SysTick_Handler(void)
{
	if (TimingDelay != 0x00)
		TimingDelay--;
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
	/* Infinite loop */
	/* Use GDB to find out why we're here */
	while (1) ;
}
#endif


