#include "globalvariable.h"
#include "stm32f10x.h"

u32 timeCount = 0;
u8 cameraCount = 2;


//初始化Spi资源
SpiMaster spiMaster1(SPI1);
SpiMaster spiMaster2(SPI2);
SpiMaster spiMaster3(SPI3);


bool hasPCRequest = false;
u8 uartBuff[20];

//
PixelBase *cameraList[2];




//PixelBase初始化
PixelBase pixelBase1(&spiMaster2, GPIO_Pin_0, GPIOA, GPIO_Pin_5, GPIOE, 0);
PixelBase pixelBase2(&spiMaster2, GPIO_Pin_14, GPIOA, GPIO_Pin_6, GPIOD, 1);
PixelBase pixelBase3(&spiMaster3, GPIO_Pin_3, GPIOA, GPIO_Pin_7, GPIOE, 2);