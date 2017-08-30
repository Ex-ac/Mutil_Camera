#include "stm32f10x.h"
#include "SpiMaster.h"
#include "PixelBase.h"
#include "public.h"
#include "AbstractSignal.h"
#include "generalizeddatastructure.h"
#include "globalvariable.h"
#include "exti.h"
#include "Protocols.h"

#include "sdio_sd.h"
#include "ff.h"
#include "usart.h"
#include "sysTick.h"

void whereIsError();
inline u32 u8Tou32(const u8 *data);
inline void dealPCIn();



int main()
{
	
	
	
	//初始化 IRQ, SDIO， 串口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
		
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	SDIO_Interrupts_Config();

	usartInit();

	//挂载
	FATFS temp;
	FRESULT result;
	result = f_mount(0, &temp);

	spiMaster1.init();

	spiMaster1.dmaInit(SpiMaster::DmaMode::ReadAndWrite);
	pixelBase1.init();

	spiMaster2.init();

	spiMaster2.dmaInit(SpiMaster::DmaMode::ReadAndWrite);
	pixelBase2.init();

	
//		spiMaster3.init();
//	
//		spiMaster3.dmaInit(SpiMaster::DmaMode::ReadAndWrite);
		pixelBase3.init();
	//

	cameraList[0] = &pixelBase1;
	cameraList[1] = &pixelBase2;
	cameraList[2] = &pixelBase3;

//	//test

	exti5_9Init();

//	//信号处理
	AbstractSignal *tempSignal;
	while (true)
	{
		dealPCIn();
		tempSignal = signalQueue.pop();
		if (tempSignal != nullptr)
		{
			if (tempSignal->done())
			{
				delete tempSignal;
			}

		}
//		else
//		{
//			for (u8 i = 0; i < 3; ++i)
//			{
//				USART_SendData(USART1, 'h');
//				while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET)
//				{
//				}
//			}
//			sysTickDelayMs(200);
//		}
	}
}


void whereIsError()
{
	for (u8 i = 0; i < 3; ++i)
	{
		USART_SendData(USART1, 'b');
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET)
		{
		}
	}
}


u32 u8Tou32(const u8 *data)
{
	u32 temp = 0x00;
	for (int i = 0; i < 4; i++)
	{
		temp |= (data[i] << ((4 - i - 1) * 8));
	}
	return temp;
}

void dealPCIn()
{
	if (hasPCRequest)
	{
		switch (uartBuff[0])
		{
		case 's':
		{
			char temp[3] = {0x00};
			temp[0] = 's';
			switch (uartBuff[2])
			{
			case 0x01:

				temp[1] = 0x01;
				//set number of the camera
				temp[2] = CameraCount;

				break;

			case 0x02:
				//
				temp[1] = 0x02;
				temp[2] = 0xff;
				//处理的

				break;

			default:

				temp[1] = 0x03;
				temp[2] = 0xff;
				//处理的
				timeCount = u8Tou32(uartBuff + 3);
				break;
			}
			for (u8 i = 0; i < 3; ++i)
			{
				USART_SendData(USART1, temp[i]);
				while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET)
				{
				}
			}
			break;
		}
		default:
			SpiSignal *p_SpiSignal;
			u32 tempU32 = u8Tou32(uartBuff + 2);
			for (u8 i = 0; i < CameraCount; ++i)
			{
				if (tempU32 & 0x01 == 0x01)
				{
					p_SpiSignal = new SpiSignal(cameraList[i], false);
					p_SpiSignal->setData(uartBuff + 6);
					signalQueue.push(p_SpiSignal);
				}
				tempU32 >>= 1;
			}
			if (uartBuff[6] == 0x05)
			{
				char dirName[64] = "\0";

				numberToString(timeCount, dirName);
				FRESULT result = f_mkdir(dirName);
			}
			break;
		}
		hasPCRequest = false;
	}
}

