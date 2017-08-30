#include "globalvariable.h"
#include "stm32f10x.h"



u32 timeCount = 0;



//初始化Spi资源
SpiMaster spiMaster1(SPI1);
SpiMaster spiMaster2(SPI2);
//SpiMaster spiMaster3(SPI3);


bool hasPCRequest = false;
u8 uartBuff[20];

//
PixelBase *cameraList[CameraCount];




//PixelBase初始化
PixelBase pixelBase1(&spiMaster1, GPIO_Pin_0, GPIOA, GPIO_Pin_5, GPIOE, 0);
PixelBase pixelBase2(&spiMaster2, GPIO_Pin_0, GPIOB, GPIO_Pin_6, GPIOD, 1);
PixelBase pixelBase3(&spiMaster2, GPIO_Pin_1, GPIOC, GPIO_Pin_7, GPIOC, 2);

