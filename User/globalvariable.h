//定义全局变量
#ifndef GLOBALVARIABLE_H_
#define GLOBALVARIABLE_H_

#include "SpiMaster.h"
#include "PixelBase.h"
#include "public.h"
#include "AbstractSignal.h"
#include "globalvariable.h"

#include "globalvariable.h"

#define CameraCount 3

//Spi资源
extern SpiMaster spiMaster1;
extern SpiMaster spiMaster2;
//extern SpiMaster spiMaster3;
//PixelBase资源
extern PixelBase pixelBase1;
extern PixelBase pixelBase2;
extern PixelBase pixelBase3;

extern u32 timeCount;


extern PixelBase *cameraList[CameraCount];

extern bool hasPCRequest;
extern u8 uartBuff[20];

#endif
