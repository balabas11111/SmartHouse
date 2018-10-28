/*
 * PinEventProcessor.h
 *
 *  Created on: 14 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_PINEVENT_PINEVENTPROCESSOR_H_
#define LIBRARIES_PINEVENT_PINEVENTPROCESSOR_H_

#include "PinEvent.h"

class PinEventProcessor{

public:
	virtual String getName()=0;
	virtual boolean processEvent(PinEvent event)=0;
	virtual PinEvent processEventNow(PinEvent event)=0;
	virtual boolean isDispatcherOfEvent(PinEvent event)=0;
	virtual boolean isTargetOfEvent(PinEvent event)=0;

	virtual ~PinEventProcessor(){};

	String printProcessInfo(PinEvent event){
		return  "Processor ("+getName()+") t="+String(isTargetOfEvent(event))+" d="+String(isDispatcherOfEvent(event))+" Event->"+event.getText();
	}

	PinEvent executeIsDispatcherFunction(PinEvent event){
		if(isDispatcherOfEvent(event))
			if(isDispatcherPostFunction!=nullptr){
				//PinEvent result=
				return (isDispatcherPostFunction(event).setIsNotEmpty(true));
			}
		return PinEvent();
	}
	void executeExternalFunction(PinEvent event){
		if(externalFunction!=nullptr){
			Serial.println("Execute external function");
			externalFunction(event);
		}else{
			Serial.println("No external function attached");
		}
	}
	boolean hasExternalFunction(){
		return externalFunction!=nullptr;
	}

	virtual PinEvent constructEvent(String evName,boolean bubble){
		Serial.println("Empty Event constructed");
		return PinEvent();
	}

protected:
	std::function<PinEvent(PinEvent)> externalFunction;
	std::function<PinEvent(PinEvent)> isDispatcherPostFunction;



};



#endif /* LIBRARIES_PINEVENT_PINEVENTPROCESSOR_H_ */
