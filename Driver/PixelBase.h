#ifndef PIXELBASE_H_
#define PIXELBASE_H_


#include "Protocols.h"
#include "generalizeddatastructure.h"
#include "SpiMaster.h"
#include "public.h"
#include "ff.h"


using PackBuffQueue = Queue<PackBuff *>;

class PixelBase : public AbstractProtocolsTransfers, public AbstractSpiSliver
{

public:

	enum class RunningStatus : u8
	{
		Free = 0x00,
		WaitForAnswered = 0x01,
		Transfering = 0x02
	};
	
	PixelBase(SpiMaster *p_master, u16 csn_pin, GPIO_TypeDef *csn_p_gpio, u16 irqPin, GPIO_TypeDef *irq_p_gpio, u8 id);
	
	void init();
    virtual void sendRequestCommand(uchar (*data)[RequestCommandBuffSize]);
    virtual void sendByte(u8 data);
    virtual uchar getByte();
    virtual void getLongByte(uchar *buff, const unsigned short size);
    bool getHasAnswer() {return _hasAnswered;}
    void setHasAnswer(bool t = true) {_hasAnswered = t;}
    DriverInfo _driverInfo;
    PicturePackInfo _picturePackInfo;
    PackBuffQueue _packBuffQueue;
	u8 getId() {return _id;}
	bool getTransferHasCompleted() {return _transferHasCompleted;}
	void setTransferHasCompleted(bool t) {_transferHasCompleted = t;}
	bool createSaveFile(const char *dirName, UINT sizeOdByte);
	bool createSaveFile(const char *dirName) {return createSaveFile(dirName, _picturePackInfo.sizeOfByte);} 
	bool savePackData(PackBuff *);
	bool savePackData() {return savePackData(_packBuffQueue.pop());}
	void closeSaveFile();
	
	void sendToPCShort();
	void sendToPCLong();
	
	ControlPin _irqControlPin;
private:
    bool _hasAnswered = true;
	const u8 _id;
	bool _transferHasCompleted = true;
	FIL _saveFile;
	
	
};

#endif
