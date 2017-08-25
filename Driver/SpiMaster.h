#ifndef SPIMASTER_H_
#define SPIMASTER_H_

#include "stm32f10x.h"
#include "public.h"


class SpiMaster;

class AbstractSpiSliver
{
public:
//    AbstractSpiSliver(SpiMaster *p_master, const ControlPin &pin);
	AbstractSpiSliver(SpiMaster *p_master, u16 pin, GPIO_TypeDef *p_gpio);
	AbstractSpiSliver(const AbstractSpiSliver &sliver);
	void init();
    virtual bool selectThisSliver();
    virtual void unselectThisSliver();
	virtual void sendByte(u8 data);
	virtual u8 readByte();
    SpiMaster *getPointToMaster() {return p_Master;}
private:
    SpiMaster *p_Master;
    ControlPin cSnPin;
};

#define Spi1Sck GPIO_Pin_5
#define Spi1Miso GPIO_Pin_6
#define Spi1Mosi GPIO_Pin_7
#define Spi1Gpio GPIOA
  
#define Spi2Sck GPIO_Pin_13
#define Spi2Miso GPIO_Pin_14
#define Spi2Mosi GPIO_Pin_15
#define Spi2Gpio GPIOB

#define Spi3Sck GPIO_Pin_3
#define Spi3Miso GPIO_Pin_4
#define Spi3Mosi GPIO_Pin_5
#define Spi3Gpio GPIOB



class SpiMaster
{
public:
	enum class DmaMode : u8
	{
		ReadOnly = 0x01,
		WriteOnly = 0x02,
		ReadAndWrite = 0x03,
	};
    SpiMaster(SPI_TypeDef *stm32Spi);
	void init();
    bool isBusy() {return _isBusy;}
    void setBusy(bool busy) {_isBusy = busy;}
    AbstractSpiSliver *getRuningSliver() {return _p_RunningSliver;}
    bool setRunningSliver(AbstractSpiSliver *runningSliver);

    void sendByte(u8 data);
    u8 readByte();
    void spiDMAReadStart(u8 *p_Buff, const unsigned short count);
	void spiDMAWriteStart(u8 *p_Buff, const unsigned short count);
	
	void dmaInit(DmaMode mode);

private:
    AbstractSpiSliver *_p_RunningSliver;
    SPI_TypeDef *_p_Stm32Spi;
	DMA_Channel_TypeDef *_p_DmaReadChannel;
	DMA_Channel_TypeDef *_p_DmaWriteChannel;
    bool _isBusy;
};


#endif
