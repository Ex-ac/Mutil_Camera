#include "Exti.h"

void exti5_9Init(void)
{
	GPIO_InitTypeDef gpioInitData;
	NVIC_InitTypeDef nvicInitData;
	EXTI_InitTypeDef extiInitData;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	gpioInitData.GPIO_Mode = GPIO_Mode_IPU;
	gpioInitData.GPIO_Pin = GPIO_Pin_5;
	gpioInitData.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &gpioInitData);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource5);
	EXTI_ClearITPendingBit(EXTI_Line5);
	
	gpioInitData.GPIO_Mode = GPIO_Mode_IPU;
	gpioInitData.GPIO_Pin = GPIO_Pin_6;
	gpioInitData.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &gpioInitData);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource6);
	EXTI_ClearITPendingBit(EXTI_Line6);
	
	
	gpioInitData.GPIO_Mode = GPIO_Mode_IPU;
	gpioInitData.GPIO_Pin = GPIO_Pin_7;
	gpioInitData.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &gpioInitData);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource7);
	EXTI_ClearITPendingBit(EXTI_Line7);        
	
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource7);
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource8);
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource9);
	
	
	EXTI_ClearITPendingBit(EXTI_Line5);
	EXTI_ClearITPendingBit(EXTI_Line6);
	EXTI_ClearITPendingBit(EXTI_Line7);
	EXTI_ClearITPendingBit(EXTI_Line8);
	EXTI_ClearITPendingBit(EXTI_Line9);
	
	
	extiInitData.EXTI_Line = EXTI_Line5 | EXTI_Line6 | EXTI_Line7;
//	| EXTI_Line7 | EXTI_Line8 |
//		EXTI_Line9;
	extiInitData.EXTI_LineCmd = ENABLE;
	extiInitData.EXTI_Mode  = EXTI_Mode_Interrupt;
	extiInitData.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&extiInitData);
	
	
	nvicInitData.NVIC_IRQChannel = EXTI9_5_IRQn;
	nvicInitData.NVIC_IRQChannelCmd = ENABLE;
	nvicInitData.NVIC_IRQChannelPreemptionPriority = 0;
	nvicInitData.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&nvicInitData);
}
