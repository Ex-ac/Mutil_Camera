#include "sysTick.h"

void sysTickDelayMs(u32 i)
{
	u32 temp;
	SysTick->LOAD = i * 9000;
	SysTick->CTRL = 0x01;
	SysTick->VAL = 0;
	do
	{
		temp = SysTick->CTRL;
	} while((temp & 0x01) && (!(temp & (0x01 << 16))));
	SysTick->VAL = 0; 
	SysTick->CTRL = 0;
}

void sysTickDelayUs(u32 i)
{
	u32 temp;
	SysTick->LOAD = i * 9;
	SysTick->CTRL = 0x01;
	SysTick->VAL = 0;
	do
	{
		temp = SysTick->CTRL;
	} while((temp & 0x01) && (!(temp & (0x01 << 16))));
	SysTick->VAL = 0; 
	SysTick->CTRL = 0;
}
