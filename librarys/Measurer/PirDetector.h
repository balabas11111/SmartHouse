/*
 * PirDetector.h
 *
 *  Created on: 20 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_MEASURER_PIRDETECTOR_H_
#define LIBRARIES_MEASURER_PIRDETECTOR_H_

#include <PinDigital.h>
#include "Arduino.h"
#include "TimeTrigger.h"
#include "Loopable.h"

class PirDetector: public Loopable {
public:
	PirDetector(String name,uint8_t _pin,PinDigital *_signalLed,std::function<void(void)> onChangeFunction,uint8_t fieldId,String queueName){
		pirDetectorPin=new PinDigital(name,_pin,nullptr,fieldId,queueName);
		humanPresentTrigger=new TimeTrigger(0,humanNotPresentInterval,false,[this](){onHumanPresentTrigger();});
		//pirPin=_pin;
		signalLed=_signalLed;
		externalFunction=onChangeFunction;
	}
	virtual ~PirDetector(){};

	boolean loop(){
		boolean changed=humanPresentTrigger->loop();

		if(changed){
			if(pirDetectorPin->isOn()){
				onMovementDetected();
			}else{
				onNoMovementDetected();
			}
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

		if(!humanPresented && externalFunction!=nullptr){
			externalFunction();
		}

		humanPresented=true;
		humanPresentTrigger->setActive(false);

		handleLightOnOff();
	}

	void onNoMovementDetected(){
		Serial.println("-------handle NO MovementDetected");
		humanPresentTrigger->saveTime();
		humanPresentTrigger->setActive(true);
	}

	void onHumanPresentTrigger(){
		Serial.println("-------no movement during period");

		humanPresented=false;
		humanPresentTrigger->setActive(false);

		handleLightOnOff();

		if(externalFunction!=nullptr){
			externalFunction();
		}
	}

	void handleLightOnOff(){
		if(signalLed!=nullptr)
			signalLed->turnOnOff(humanPresented);
	}

	virtual String getJson(){
		String result=
				"{\"id\":\""+String(pirDetectorPin->getItem().id)+"\","
				+"\"name\":\""+pirDetectorPin->getItem().name+"\","
				+"\"type\":\"PirDetector\","
				+"\"size\":\"Human/No_Human\","
				+"\"descr\":\"Human detector\","
				+"\"val\":\""+String(humanPresented)+"\","
				+"\"fieldId\":\""+String(pirDetectorPin->getItem().fieldId)+"\","
				+"\"childCount\":\"0\","
				+"\"queueName\":\""+pirDetectorPin->getQueueName()+"\","
				+"\"items\":[]}";

		return result;
	}
	virtual String getSimpleJson(){
		String result=
				"{\"name\":\""+pirDetectorPin->getItem().name+"\","
				+"\"descr\":\"Human detector\","
				+"\"val\":\""+String(humanPresented)+"\","
				+"\"fieldId\":\""+String(pirDetectorPin->getItem().fieldId)+"\","
				+"\"queueName\":\""+pirDetectorPin->getQueueName()+"\","
				+"\"items\":[]}";

		return result;
	}

private:
	PinDigital *pirDetectorPin;
	TimeTrigger *humanPresentTrigger;

	const int humanNotPresentInterval=30000;

	boolean humanPresented=false;

	PinDigital *signalLed;
	std::function<void(void)> externalFunction;

};

#endif /* LIBRARIES_MEASURER_PIRDETECTOR_H_ */
