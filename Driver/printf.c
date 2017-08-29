#include "printf.h"

void printf_Init()
{
	GPIO_InitTypeDef gpioInitData;
	NVIC_InitTypeDef nvicInitData;
	USART_InitTypeDef usartInitData;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	gpioInitData.GPIO_Pin = usart_RX;
	gpioInitData.GPIO_Speed = GPIO_Speed_50MHz;
	gpioInitData.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &gpioInitData);
	
	gpioInitData.GPIO_Pin = usart_TX;
	gpioInitData.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &gpioInitData);
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
	usartInitData.USART_BaudRate = 11520;
	usartInitData.USART_WordLength = USART_WordLength_8b;
	usartInitData.USART_StopBits = USART_StopBits_1;
	usartInitData.USART_Parity = USART_Parity_No;
	usartInitData.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usartInitData.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &usartInitData);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	nvicInitData.NVIC_IRQChannel = USART1_IRQn;
	nvicInitData.NVIC_IRQChannelCmd = ENABLE;
	nvicInitData.NVIC_IRQChannelPreemptionPriority = 0;
	nvicInitData.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&nvicInitData);

}

int fputc(int i, FILE *f)
{
	USART_SendData(USART1, (u8) i);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == Bit_RESET)
	{
		;
	}
	return i;	
}
