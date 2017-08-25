#include "SpiMaster.h"
#include "sysTick.h"

//AbstractSpiSliver::AbstractSpiSliver(SpiMaster *master,
//	const ControlPin &pin) : p_Master(master), cSnPin(pin)
//{
//   
//}


AbstractSpiSliver::AbstractSpiSliver(const AbstractSpiSliver &sliver) 
	: p_Master(sliver.p_Master), cSnPin(sliver.cSnPin)
{ 

}

AbstractSpiSliver::AbstractSpiSliver(SpiMaster *p_master, u16 pin,
	GPIO_TypeDef *p_gpio) : p_Master(p_master), cSnPin(pin, p_gpio)
{
}
void AbstractSpiSliver::init()
{
	cSnPin.setOutput(true);
	cSnPin.setHight(true);
}
bool AbstractSpiSliver::selectThisSliver()
{
    if (p_Master->setRunningSliver(this))
    {
		cSnPin.setHight(false);
        p_Master->setBusy(true);
		sysTickDelayUs(1);
        return true;
    }
    else
    {
        return false;
    }
}

void AbstractSpiSliver::unselectThisSliver()
{

	sysTickDelayUs(1);
	cSnPin.setHight(true);
    p_Master->setBusy(false);
}


void AbstractSpiSliver::sendByte(u8 data)
{
	p_Master->sendByte(data);
}

u8 AbstractSpiSliver::readByte()
{
	return p_Master->readByte();
}

SpiMaster::SpiMaster(SPI_TypeDef *spi)
    : _p_Stm32Spi(spi)
{
	_isBusy = false;
}

void SpiMaster::init()
{
	GPIO_InitTypeDef gpioInitData;
    SPI_InitTypeDef spiInitData;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	//same
    //spi
    spiInitData.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    spiInitData.SPI_Mode = SPI_Mode_Master;
    spiInitData.SPI_DataSize = SPI_DataSize_8b;
	spiInitData.SPI_CPOL = SPI_CPOL_High;
    spiInitData.SPI_CPHA = SPI_CPHA_2Edge;
    spiInitData.SPI_NSS = SPI_NSS_Soft;
    spiInitData.SPI_FirstBit = SPI_FirstBit_MSB;
    spiInitData.SPI_CRCPolynomial = 7;
	
	
	if (_p_Stm32Spi == SPI1)
    {
        //spi rcc
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

		
		gpioInitData.GPIO_Pin = Spi1Sck | Spi1Mosi;
        gpioInitData.GPIO_Mode = GPIO_Mode_AF_PP;
        gpioInitData.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(Spi1Gpio, &gpioInitData);
		
		
		gpioInitData.GPIO_Pin = Spi1Miso;
		gpioInitData.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(Spi1Gpio, &gpioInitData);
		
//		gpioInitData.GPIO_Pin = Spi1Miso;
//		gpioInitData.GPIO_Mode = GPIO_Mode_AF_OD;
//		GPIO_Init(Spi1Gpio, &gpioInitData);
		

        //APB2
        spiInitData.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; //72m

        //dma

        _p_DmaReadChannel = DMA1_Channel2;
        _p_DmaWriteChannel = DMA1_Channel3;

    }
    else if (_p_Stm32Spi == SPI2)
    {
        //spi rcc
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		
        gpioInitData.GPIO_Pin = Spi2Sck | Spi2Mosi ;
        gpioInitData.GPIO_Mode = GPIO_Mode_AF_PP;
        gpioInitData.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(Spi2Gpio, &gpioInitData);
	
		gpioInitData.GPIO_Pin = Spi2Miso;
		gpioInitData.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(Spi2Gpio, &gpioInitData);
        //36m
		spiInitData.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
        //dma

        _p_DmaReadChannel = DMA1_Channel4;
        _p_DmaWriteChannel = DMA1_Channel5;


    }
    else
    {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
        gpioInitData.GPIO_Pin = Spi3Sck | Spi3Mosi;
        gpioInitData.GPIO_Mode = GPIO_Mode_AF_PP;
        gpioInitData.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(Spi3Gpio, &gpioInitData);
		
		gpioInitData.GPIO_Pin = Spi3Miso;
		gpioInitData.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(Spi3Gpio, &gpioInitData);
        //APB1
        spiInitData.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; //36m


        _p_DmaReadChannel = DMA2_Channel1;
        _p_DmaWriteChannel = DMA2_Channel2;

    }

	SPI_Init(_p_Stm32Spi, &spiInitData);
	
    SPI_Cmd(_p_Stm32Spi, ENABLE);
	
}

void SpiMaster::dmaInit(DmaMode mode)
{
    NVIC_InitTypeDef nvicInitData;
    DMA_InitTypeDef dmaInitData;

    if (_p_Stm32Spi == SPI3)
    {
       RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);

    }
    else
    {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE); 
    }
	
	
	nvicInitData.NVIC_IRQChannelCmd = ENABLE;
	nvicInitData.NVIC_IRQChannelPreemptionPriority = 1;
	nvicInitData.NVIC_IRQChannelSubPriority = 0;
	
	dmaInitData.DMA_M2M = DMA_M2M_Disable;
	dmaInitData.DMA_MemoryDataSize  = DMA_MemoryDataSize_Byte;
	dmaInitData.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dmaInitData.DMA_Mode = DMA_Mode_Normal;
	
	dmaInitData.DMA_PeripheralBaseAddr = u32(&(_p_Stm32Spi->DR));
	dmaInitData.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	dmaInitData.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dmaInitData.DMA_Priority = DMA_Priority_VeryHigh;
		
	if ((u8(mode) & 0x01) > 0)
	{
		DMA_DeInit(_p_DmaReadChannel);
		dmaInitData.DMA_DIR = DMA_DIR_PeripheralSRC;
		
		if (_p_Stm32Spi == SPI1)
		{
			nvicInitData.NVIC_IRQChannel = DMA1_Channel2_IRQn;
		}
		else if (_p_Stm32Spi == SPI2)
		{
			nvicInitData.NVIC_IRQChannel = DMA1_Channel4_IRQn;
		}
		else
		{
			nvicInitData.NVIC_IRQChannel = DMA2_Channel1_IRQn;
		}
		
		
		DMA_Init(_p_DmaReadChannel, &dmaInitData);
		DMA_ITConfig(_p_DmaReadChannel, DMA_IT_TC, ENABLE);
		
		_p_Stm32Spi->CR2 |= 1 << 0;
		NVIC_Init(&nvicInitData);
		
	}
	if ((u8(mode) & 0x02) > 0)
	{
		DMA_DeInit(_p_DmaWriteChannel);
		dmaInitData.DMA_DIR = DMA_DIR_PeripheralDST;
		
		if (_p_Stm32Spi == SPI1)
		{
			nvicInitData.NVIC_IRQChannel = DMA1_Channel3_IRQn;
		}
		else if (_p_Stm32Spi == SPI2)
		{
			nvicInitData.NVIC_IRQChannel = DMA1_Channel5_IRQn;
		}
		else
		{
			nvicInitData.NVIC_IRQChannel = DMA2_Channel2_IRQn;
		}
		
		DMA_Init(_p_DmaWriteChannel, &dmaInitData);
		DMA_ITConfig(_p_DmaWriteChannel, DMA_IT_TC, ENABLE);
		_p_Stm32Spi->CR2 |= 1 << 1;
		NVIC_Init(&nvicInitData);
	}

}

void SpiMaster::sendByte(u8 data)
{
	bool txe;
	u16 i = 0;
	do
	{
		txe = (_p_Stm32Spi->SR & 0x02) == RESET;
		if (i ++ > 10000)
		{
			break;
		}
	} while (txe);
    _p_Stm32Spi->DR = data;
}

u8 SpiMaster::readByte()
{	
	sendByte(0xff);
	while (((_p_Stm32Spi->SR) & 0x01)== RESET)
	{
		;
	}
	return _p_Stm32Spi->DR;
}

void SpiMaster::spiDMAReadStart(u8 *p_Buff, const unsigned short count)
{
	u8 tempByte = 0xff;
	_p_DmaWriteChannel->CCR &= ~(1 << 7);
	DMA_ITConfig(_p_DmaWriteChannel, DMA_IT_TC, DISABLE);
	//
    _p_DmaReadChannel->CMAR = u32(p_Buff);
    _p_DmaReadChannel->CNDTR = count;
	
	//
	_p_DmaWriteChannel->CMAR = u32(&tempByte);
	_p_DmaWriteChannel->CNDTR = count;
    DMA_Cmd(_p_DmaReadChannel, ENABLE);
	DMA_Cmd(_p_DmaWriteChannel, ENABLE);
}

void SpiMaster::spiDMAWriteStart(u8 *p_Buff, const unsigned short count)
{
	DMA_ITConfig(_p_DmaWriteChannel, DMA_IT_TC, ENABLE);
	_p_DmaWriteChannel->CCR |= (1 << 7);
    _p_DmaReadChannel->CMAR = u32(p_Buff);
    _p_DmaReadChannel->CNDTR = count;
    DMA_Cmd(_p_DmaWriteChannel, ENABLE);
}


bool SpiMaster::setRunningSliver(AbstractSpiSliver *runningSliver)
{
	if (_isBusy)
	{
		return false;
	}
	else
	{
		_p_RunningSliver = runningSliver;
		return true;
	}
}
