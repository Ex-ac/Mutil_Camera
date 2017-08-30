#ifndef USART_H_
#define USART_H_

#include "stm32f10x.h"


#define usart_TX (GPIO_Pin_9)
#define usart_RX (GPIO_Pin_10)

void usartInit();


#endif
