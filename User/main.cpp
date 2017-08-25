#include "stm32f10x.h"
#include "SpiMaster.h"
#include "PixelBase.h"
#include "public.h"
#include "AbstractSignal.h"
#include "generalizeddatastructure.h"
#include "globalvariable.h"
#include "exti.h"
#include "Protocols.h"
#include "sysTick.h"
#include "sdio_sd.h"
#include "ff.h"
#include <stdlib.h>
#include <time.h>

#include "printf.h"

u32 u8Tou32(const u8 *data)
{
	u32 temp = 0x00;
	for (int i = 0; i < 4; i++)
	{
		temp |= (data[i] << ((4 - i - 1) * 8));
	}
	return temp;
}


int main()
{
    //初始化 IRQ, SDIO， 串口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	SDIO_Interrupts_Config();
	
	printf_Init();
	
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

//	
//	spiMaster3.init();
//	
//	spiMaster3.dmaInit(SpiMaster::DmaMode::ReadAndWrite);
//	pixelBase3.init();
//	
	
	cameraList[0] = &pixelBase1;
	cameraList[1] = &pixelBase2;
	
	
	u32 tempU32;
	//test

	exti5_9Init();

	
	
	
	 
//	u8 data[RequestCommandBuffSize] = {0x00};
//	TakePictureInfo info;
//	info.exposure = Exposure::AutoExposure;
//	info.imageFormat = ImageFormat::JPG;
//	info.isColor = true;
//	info.quality = Quality::VeryHigh;
//	info.resolution = Resolution::W2592;
//	SpiSignal *p_SpiSignal;
//	
//	
//	MasterProtocols::takePictureRequest(&pixelBase1, &data, &info);

//	p_SpiSignal = new SpiSignal(&pixelBase1, false);
//	p_SpiSignal->setData(&data);
//	signalQueue.push(p_SpiSignal);
	
//	p_SpiSignal = new SpiSignal(&pixelBase2, false);
//	MasterProtocols::takePictureRequest(&pixelBase2, &data, &info);
//	p_SpiSignal->setData(&data);
//	signalQueue.push(p_SpiSignal);
//	
	//拍照完成创建文件


//	result = f_mkdir(dirName);
	
    //信号处理
    AbstractSignal *tempSignal;
     while (true)
    {
	
		if (hasPCRequest)
		{
			switch (uartBuff[0])
			{
				case 's':
					char temp[3] = {0x00};
					temp[0] = 's';
					switch (uartBuff[2])
					{
						case 0x01:
							
							temp[1] = 0x01;
							//set number of the camera
							temp[2] = cameraCount;
							
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
					for (int i = 0; i < 3; ++i)
					{
						USART_SendData(USART1, temp[i]);
						while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
						{
						}
					}
				break;
				
				default:
				/*
					usartBuff[0] = 'c';
					for (int i = 0; i < 12; ++i)
					{
						USART_SendData(USART1, usartBuff[i]);
						while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
						{
						}
					}
				*/
					SpiSignal *p_SpiSignal;
					tempU32 = u8Tou32(uartBuff + 2);
					for (int i = 0; i < cameraCount; ++i)
					{
						if (tempU32 & 0x01 == 0x01)
						{
							p_SpiSignal = new SpiSignal(cameraList[i], false);
							p_SpiSignal->setData(uartBuff + 6);
							signalQueue.push(p_SpiSignal);
							
						}
					}
					if (uartBuff[6] == 0x05)
					{
						char dirName[64] = "\0";

						numberToString(timeCount, dirName);
						result = f_mkdir(dirName);
					}
				break;
				
			}
			hasPCRequest = false;
		}
	
	
        tempSignal = signalQueue.pop();
		if (tempSignal != nullptr)
		{
			if (tempSignal->done())
			{
//				printf("delete signal\n");
				delete tempSignal;
			}
//			else
//			{	
//				printf("signal\n");
//				
//			}
		}
		else
		{	
//			printf("Empty!!");
		}
	}
	
}





