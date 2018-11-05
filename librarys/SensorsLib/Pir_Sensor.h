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
#include "AbstractItem.h"

class Pir_Sensor: public Loopable,public AbstractItem {
public:
	Pir_Sensor(String name,uint8_t _pin,std::function<void(void)> onChangeFunction)
		:AbstractItem(_pin,name,"PirDetector","Human/No_Human","Human detector",0, 1,-2,2){
		pirDetectorPin=new PinDigital(name,_pin,[this](){processPinChange();});
		humanPresentTrigger=new TimeTrigger(0,humanNotPresentInterval,false,[this](){onHumanPresentTrigger();});
		//pirPin=_pin;
		//signalLed=_signalLed;
		externalFunction=onChangeFunction;

		items[0]={0,item.name,item.type,PinDigital,item.descr,pirDetectorPin->getVal(),0,-2,2,""};
	}
	virtual ~Pir_Sensor(){};



	boolean loop(){
		humanPresentTrigger->loop();
		return pirDetectorPin->loop();
	}

	void processPinChange(){
		Serial.print("Pir change ");
		Serial.println(pirDetectorPin->getVal());

		if(pirDetectorPin->isOn()){
			onMovementDetected();
		}else{
			onNoMovementDetected();
		}
	}

	boolean isHumanPresented(){
		return humanPresented;
	}

	void onMovementDetected(){
		Serial.println("-------handleMovementDetected");
		/*if(signalLed!=nullptr)
			signalLed->changeAndDelay(50, 2);
			*/

		if(!humanPresented && externalFunction!=nullptr){
			externalFunction();
		}

		humanPresented=true;
		humanPresentTrigger->setActive(false);

		handleLightOnOff();
	}

	void onNoMovementDetected(){
		//Serial.println("-------handle NO MovementDetected");
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
		/*if(signalLed!=nullptr)
			signalLed->turnOnOff(humanPresented);
			*/
	}

private:
	PinDigital *pirDetectorPin;
	TimeTrigger *humanPresentTrigger;

	const int humanNotPresentInterval=30000;

	boolean humanPresented=false;

	//PinDigital *signalLed;
	std::function<void(void)> externalFunction;

};

#endif /* LIBRARIES_MEASURER_PIRDETECTOR_H_ */
