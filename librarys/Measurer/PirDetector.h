/*
 * PirDetector.h
 *
 *  Created on: 20 окт. 2018 г.
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
	PirDetector(uint8_t _pin,PinDigital *_signalLed,std::function<PinEvent(PinEvent)> _processPinEvent){
		pirDetectorPin=new PinDigital("pirDetectorPin",_pin);
		humanPresentTrigger=new TimeTrigger(0,humanNotPresentInterval,false,[this](){onHumanPresentTrigger();});
		//pirPin=_pin;
		signalLed=_signalLed;
		processPinEvent=_processPinEvent;
	}
	virtual ~PirDetector();

	boolean loop(){
		boolean changed=humanPresentTrigger->loop();
		if(changed){
			onPirWasChanged(pirDetectorPin->constructEvent(PIN_EVENT_STATE_UPDATED,true));
		}
		return changed;
	}

	void onMovementDetected(){
		Serial.println("-------handleMovementDetected");
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

		processPinEvent(pirDetectorPin->constructEvent(PIN_EVENT_STATE_UPDATED,true));
	}

	void handleLightOnOff(){
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
};

#endif /* LIBRARIES_MEASURER_PIRDETECTOR_H_ */
