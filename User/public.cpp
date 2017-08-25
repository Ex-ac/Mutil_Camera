#include "public.h"
#include "stm32f10x.h"

ControlPin::ControlPin(u16 pin, GPIO_TypeDef *p_gpio) : _pin(pin),
	_p_gpio(p_gpio)
{
//	setControlPinOutput(isOutput);
}

void ControlPin::setHight(bool h)
{
	if (h)
		GPIO_SetBits(_p_gpio, _pin);
	else
		GPIO_ResetBits(_p_gpio, _pin);

	for (u8 i = 0; i < 3;)
	{

		if ((GPIO_ReadInputDataBit(_p_gpio, _pin) == Bit_SET) == h)
			i++;
	}
}

void ControlPin::setOutput(bool h)
{
	GPIO_InitTypeDef gpioInitData;
	if (h)
	{
		gpioInitData.GPIO_Mode = GPIO_Mode_Out_PP;
	}
	else
	{
		gpioInitData.GPIO_Mode = GPIO_Mode_IPU;
	}
	gpioInitData.GPIO_Speed = GPIO_Speed_50MHz;
	gpioInitData.GPIO_Pin = _pin;
	GPIO_Init(_p_gpio, &gpioInitData);
}




void numberToString(unsigned int number, char *p_char)
{
	unsigned char i = 0;
	char temp[64];
	do
	{
		temp[i] = number % 10 + '0';
		number /= 10;
		 i ++;
	} while(number != 0);
	
	for (unsigned char j = 0; j < i; ++j)
	{
		*(p_char + j) = temp[i - 1 - j];
	}
	*(p_char + i) = '\0';
}

