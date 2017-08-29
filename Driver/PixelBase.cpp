#include "PixelBase.h"
#include "stm32f10x.h"
#include <stdio.h>

PixelBase::PixelBase(SpiMaster *p_master, u16 csn_pin, GPIO_TypeDef *csn_p_gpio, u16 irqPin, GPIO_TypeDef *irq_p_gpio, u8 id)
	: AbstractProtocolsTransfers(), AbstractSpiSliver(p_master, csn_pin, csn_p_gpio),  _packBuffQueue(2), 
		_irqControlPin(irqPin, irq_p_gpio), _id(id)
{
}

void PixelBase::init()
{
	AbstractSpiSliver::init();
}


void PixelBase::sendRequestCommand(uchar (*data)[RequestCommandBuffSize])
{
    _hasAnswered = false;
    AbstractProtocolsTransfers::sendRequestCommand(data);
}

uchar PixelBase::getByte()
{
    return getPointToMaster()->readByte();
}

void PixelBase::getLongByte(uchar *buff, const unsigned short size)
{
    getPointToMaster()->spiDMAReadStart(buff, size);
}

void PixelBase::sendByte(u8 data)
{
    getPointToMaster()->sendByte(data);
}


bool PixelBase::createSaveFile(const char *dirName, UINT sizeOfByte)
{
	FRESULT result;
	char fileName[128];
	char *p = fileName;
	while ((*dirName) != '\0')
	{
		*(p ++) = *(dirName ++);
	}
	*(p ++) = '/';
	*(p ++) = '0' + (_id / 100);
	*(p ++) = '0' + ((_id % 100) / 10);
	*(p ++) = '0' + (_id & 10);
	*(p ++) = '.';
	*(p ++) = 'j';
	*(p ++) = 'p';
	*(p ++) = 'g';
	*(p ++) = '\0';
	
	result = f_open(&_saveFile, fileName, FA_OPEN_ALWAYS | FA_WRITE);
	if (result != FR_OK)
	{
		return false;
	}
	result = f_lseek(&_saveFile, sizeOfByte);
	if (result != FR_OK || _saveFile.fptr != sizeOfByte)
	{
		//error
//		printf("create file error\n");
		f_close(&_saveFile);
		return false;
	}
//	printf("create file %d success, byte: %ld\n", _id, sizeOfByte);
	_transferHasCompleted = false;
	return true;
}

bool PixelBase::savePackData(PackBuff *p_packBuff)
{
	FRESULT result;
	UINT count = 0;
	if (p_PackBuff == nullptr)
	{
		return false;
	}
	
	
	result = f_lseek(&_saveFile, 1024 * (p_packBuff->getNumberOfPack() - 1));
//	if (result != FR_OK)
//	{
//		return false;
//	}
		
	if (_picturePackInfo.sizeOfPack == p_packBuff->getNumberOfPack())
	{
	
		result = f_write(&_saveFile, 
			(const void*)(p_packBuff->getPointToPackBuff()), 
			p_packBuff->getSizeOfByte(), &count);
		
//		uchar *p = (p_packBuff->getPointToPackBuff() + p_packBuff->getSizeOfByte() - 1);
//		unsigned short i = 0;
//		for (; i < p_packBuff->getSizeOfByte(); ++ i)
//		{
//			if (*(p--) != 0x00)
//			{
//				break;
//			}
//		}
//		result = f_lseek(&_saveFile, (_picturePackInfo.sizeOfByte - i));
//		if (result != FR_OK)
//		{
//			//error
//		}
		result = f_truncate(&_saveFile);
		closeSaveFile();
	}
	else
	{
		result = f_write(&_saveFile, 
			(const void*)(p_packBuff->getPointToPackBuff()), 
				512, &count);
//		if (result != FR_OK || count != 512)
//		{
////		printf("save file error\n");
//		return false;
//		}
		result = f_write(&_saveFile, 
			(const void*)(p_packBuff->getPointToPackBuff() + 512), 
				512, &count);
//		if (result != FR_OK || count != 512)
//		{
////		printf("save file error\n");
//		return false;
//		}
		
	}
	
	setHasAnswer(true);
	delete p_packBuff;
//	printf("Pack has delete\n");
	return true;
}

void PixelBase::closeSaveFile()
{
	FRESULT result;
	_transferHasCompleted = true;
	
	if (result == FR_OK)
	{
//		printf("Truncate file.\n");
	}
	
	f_close(&_saveFile);
}

void PixelBase::sendToPCShort()
{
	uchar tempData[14] = {0x00};
	tempData[0] = 'c';
	for(uchar i = 0; i < AnswerCommandBuffSize; ++i)
	{
		tempData[i + 1] = this->receiveDataBuff[i];
	}
	
	tempData[4] = _id;
	for (int i = 0; i < AnswerCommandBuffSize + 1; ++i)
	{
		USART_SendData(USART1, tempData[i]);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET)
		{
		}
	}
}

void PixelBase::sendToPCLong()
{
	uchar temp[14 + 1024] = {0xff};
	
	temp[0] = 'c';
	unsigned short i = 1;
	for (uchar j = 0; j < AnswerCommandBuffSize - 2; ++i, ++j)
	{
		 temp[i] = this->receiveDataBuff[j];
	}
	temp[4] = _id;
	uchar *ptemp1 = p_PackBuff->getPointToPackBuff();
	for (unsigned short j = 0; j < p_PackBuff->getSizeOfByte() ; i ++, j ++)
	{
		temp[i] = *(ptemp1 + j);
	}
	for (uchar j = AnswerCommandBuffSize - 2; j < AnswerCommandBuffSize; ++i, ++j)
	{
		temp[i] = this->receiveDataBuff[j];
	}
	
	for (unsigned short j = 0; j < i; ++ j)
	{
		USART_SendData(USART1, temp[j]);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET)
		{
		}
	}
}
