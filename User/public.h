#ifndef PUBLIC_H_
#define PUBLIC_H_

#include "stm32f10x.h"

class ControlPin
{
public:
	ControlPin(u16 pin, GPIO_TypeDef *p_gpio);
	ControlPin(const ControlPin & pin) { _pin = pin._pin; _p_gpio = pin._p_gpio;}
	void setHight(bool h = true);
	void setOutput(bool h = true);
	bool isHight() {return (GPIO_ReadInputDataBit(_p_gpio, _pin) == Bit_SET);}
private:
	u16 _pin;
	GPIO_TypeDef *_p_gpio;
};

void numberToString(unsigned int number, char *p_char);
#endif
