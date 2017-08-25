#ifndef ABSTRACTSIGNAL_H_
#define ABSTRACTSIGNAL_H_

#include "generalizeddatastructure.h"
#include "SpiMaster.h"
#include "Protocols.h"
#include "PixelBase.h"
#include "globalvariable.h"

class AbstractSignal;

enum {MaxNumberOfSignal = 50};

//信号队列
using SignalQueue = Queue<AbstractSignal *>;
extern SignalQueue signalQueue;



class AbstractSignal
{
public:
	AbstractSignal(){}
    virtual bool done() = 0;
};

void inseratIntoSignalQueue(AbstractSignal *p_signal);

class SpiSignal : public AbstractSignal
{
public:
    SpiSignal(PixelBase *p, bool isAnswer = true) : 
        AbstractSignal() , p_Traget(p), _typeIsAnswer(isAnswer) {}
    bool typeIsAnswer() { return _typeIsAnswer;}
    void setData(uchar *data);
    void getData(uchar *data);
    PixelBase *getPointToTrager() {return p_Traget;}
    virtual bool done();

private:
    PixelBase *p_Traget;
    bool _typeIsAnswer;
    uchar data[RequestCommandBuffSize];
};

class SDSignal : public AbstractSignal
{	
public:
		SDSignal(PixelBase *p) : AbstractSignal() , p_Traget(p) {}
		PixelBase *getPointToTrager() {return p_Traget;}
		virtual bool done();
private:
	PixelBase *p_Traget;
};


#endif
