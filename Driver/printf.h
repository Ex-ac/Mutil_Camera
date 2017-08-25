#ifndef _printf_H
#define _printf_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include "stm32f10x.h"

#define usart_TX (GPIO_Pin_9)
#define usart_RX (GPIO_Pin_10)

void printf_Init(void);
int fputc(int ch, FILE *p);


#ifdef __cplusplus
}
#endif

#endif
