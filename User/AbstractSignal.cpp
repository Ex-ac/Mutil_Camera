#include "AbstractSignal.h"
#include "globalvariable.h"

#include <stdio.h>
#include <stdlib.h>

SignalQueue signalQueue(MaxNumberOfSignal);

void inseratIntoSignalQueue(AbstractSignal *p_signal)
{
    while (!signalQueue.push(p_signal))
    {
        signalQueue.pop()->done();
    }
}

void SpiSignal::setData(uchar *data)
{
    for (int i = 0; i < RequestCommandBuffSize; ++i)
    {
        this->data[i] = data[i];
    }
}
void SpiSignal::getData(uchar *data)
{
    for (int i = 0; i < RequestCommandBuffSize; ++i)
    {
        data[i] = (this->data)[i];
    }
}

bool SpiSignal::done()
{
    bool ok = false;
    SpiSignal *p_SpiSignal = dynamic_cast<SpiSignal *>(this);
	PixelBase *p_PixelBase = p_SpiSignal->getPointToTrager();
    if (!p_SpiSignal->typeIsAnswer()) //发送命令
    { 
        if ( p_PixelBase->_irqControlPin.isHight())
        {
            if (p_PixelBase->selectThisSliver())
            {
                uchar tdata[RequestCommandBuffSize] = {0x00};
                p_SpiSignal->getData(tdata);
				p_SpiSignal->getPointToTrager()->setHasAnswer(false);
                p_SpiSignal->getPointToTrager()->sendRequestCommand(&tdata);
                //				p_SpiSignal->getPointToTrager()->_runningStatus = PixelBase::RunningStatus::WaitForAnswered;
                ok = true;
                p_PixelBase->unselectThisSliver();
            }
            else
            {
//                printf("Wait for Spi\n");
                ok = false;
                inseratIntoSignalQueue(p_SpiSignal);
            }
        }
        else
        {
//            printf("Wait for PixelBase\n");
            ok = false;
            inseratIntoSignalQueue(p_SpiSignal);
        }
    }
    else
    {
        if (p_SpiSignal->getPointToTrager()->selectThisSliver())
        {
            
            p_PixelBase->getFirst11ByteCommand();

            if (MasterProtocols::packAnswerCommand(p_PixelBase) ==
                uchar(MasterProtocols::AnswerCommand::GetPicturePack)) //是否为传输命令
            {
                //在DMA中断中进行校验和多的最后两个命令字
                MasterProtocols::getPicturePackAnswer(p_PixelBase,
                                                      &(p_PixelBase->_picturePackInfo));
                ok = true;
            }
            else
            {
                //获得后面两个命令字
                p_PixelBase->getLast2ByteCommand();

                if (MasterProtocols::packCheck(p_PixelBase))
                {
                    ErrorCode errorCode;
                    switch (MasterProtocols::packAnswerCommand(p_PixelBase))
                    {
                    case uchar(MasterProtocols::AnswerCommand::Inquiry):
                        errorCode = MasterProtocols::inquiryAnswer(p_PixelBase,
                                                                   &(p_PixelBase->_driverInfo));
                        break;

                    case uchar(MasterProtocols::AnswerCommand::Focus):
                        errorCode = MasterProtocols::foucsAnswer(p_PixelBase,
                                                                 &(p_PixelBase->_driverInfo.zoom));
                        break;

                    case uchar(MasterProtocols::AnswerCommand::TakePicture):
                        errorCode = MasterProtocols::takePictureAnswer(p_PixelBase,
                                                                       &(p_PixelBase->_picturePackInfo));
					    char dirName[64] = "\0";
						
						numberToString(timeCount, dirName);

						p_PixelBase->createSaveFile(dirName);
						
						if (errorCode == ErrorCode::NoneError)
						{
							SpiSignal *p_SignalTemp = new SpiSignal(p_PixelBase, false);
							u8 data[RequestCommandBuffSize] = {0x00};
							MasterProtocols::getPicturePackRequest(p_PixelBase, &data, 1,
                                                               &(p_PixelBase->_picturePackInfo));
							p_SignalTemp->setData(data);
							inseratIntoSignalQueue(p_SignalTemp);
						}
                        break;

                    default:
                        errorCode = ErrorCode::UnkownError;
                    }
                    p_SpiSignal->getPointToTrager()->setHasAnswer(true);
                    ok = true;
					
					p_PixelBase->sendToPCShort();
					
					if (errorCode != ErrorCode::NoneError)
					{
//						printf("Code Error\n");
						SpiSignal *p_SignalTemp = new SpiSignal(p_PixelBase, false);
						p_SignalTemp->setData((p_PixelBase->sendDataBuff));
						inseratIntoSignalQueue(p_SignalTemp);
					}
                }
				else
				{
//						printf("Code Error\n");
						SpiSignal *p_SignalTemp = new SpiSignal(p_PixelBase, false);
						p_SignalTemp->setData((p_PixelBase->sendDataBuff));
						inseratIntoSignalQueue(p_SignalTemp);
						
						
				}
                p_SpiSignal->getPointToTrager()->unselectThisSliver();
            }
        }
        else
        {
//            printf("Wait for Spi\n");
            ok = false;
            inseratIntoSignalQueue(p_SpiSignal);
        }
    }
	
	return ok;

}

bool SDSignal::done()
{

    return p_Traget->savePackData();
}
