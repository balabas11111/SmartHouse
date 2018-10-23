/*
 * PirDetector.h
 *
 *  Created on: 20 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_MEASURER_PIRDETECTOR_H_
#define LIBRARIES_MEASURER_PIRDETECTOR_H_

#include "Arduino.h"
#include "TimeTrigger.h"
#include "PinDigital.h"
#include "Loopable.h"

class PirDetector {
public:
	PirDetector(uint8_t _pin,PinDigital *_signalLed,std::function<PinEvent(PinEvent)> _processPinEvent,std::function<void(void)> _externalFunction){
		pirDetectorPin=new PinDigital("pirDetectorPin",_pin);
		humanPresentTrigger=new TimeTrigger(0,humanNotPresentInterval,false,[this](){onHumanPresentTrigger();});
		//pirPin=_pin;
		signalLed=_signalLed;
		processPinEvent=_processPinEvent;
		externalFunction=_externalFunction;
	}
	virtual ~PirDetector();

	boolean loop(){
		boolean changed=humanPresentTrigger->loop();
		if(changed){
			onPirWasChanged(pirDetectorPin->constructEvent(PIN_EVENT_STATE_UPDATED,true));
		}
		return changed;
	}

	boolean isHumanPresented(){
		return humanPresented;
	}

	void onMovementDetected(){
		Serial.println("-------handleMovementDetected");
		if(signalLed!=nullptr)
			signalLed->changeAndDelay(50, 2);

		humanPresented=true;
		humanPresentTrigger->setActive(false);
		handleLightOnOff();
	}

	void onNoMovementDetected(){
		Serial.println("-------handle NO MovementDetected");
		humanPresentTrigger->saveTime();
		humanPresentTrigger->setActive(true);
		handleLightOnOff();
	}

	void onHumanPresentTrigger(){
		Serial.println("-------no movement during period");

		humanPresented=false;
		humanPresentTrigger->setActive(false);

		handleLightOnOff();

		if(processPinEvent!=nullptr)
			processPinEvent(pirDetectorPin->constructEvent(PIN_EVENT_STATE_UPDATED,true));

		if(externalFunction!=nullptr){
			externalFunction();
		}
	}

	void handleLightOnOff(){
		if(signalLed!=nullptr)
			signalLed->turnOnOff(humanPresented);
	}

	PinEvent onPirWasChanged(PinEvent event){
		Serial.println("//On pir changed");
		//On pir changed
		boolean dispatch=!humanPresented;

		if(event.getVal()==HIGH){
			onMovementDetected();
		} else{
			onNoMovementDetected();
		}

		processPinEvent(event.setIsBubble(dispatch));

		return PinEvent();
	}
private:
	PinDigital *pirDetectorPin;
	TimeTrigger *humanPresentTrigger;

	const int humanNotPresentInterval=30000;

	boolean humanPresented=false;

	PinDigital *signalLed;
	std::function<PinEvent(PinEvent)> processPinEvent;
	std::function<void(void)> externalFunction;
};

#endif /* LIBRARIES_MEASURER_PIRDETECTOR_H_ */
