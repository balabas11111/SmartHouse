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

class Pir_Sensor: public PinDigital {
public:
	Pir_Sensor(String name,uint8_t _pin,std::function<void(void)> onChangeFunction, uint16_t analogLevelForHigh, uint humanNotPresentInterval )
		:PinDigital(name,_pin,[this](){processPinChange();}){
		this->humanNotPresentInterval=humanNotPresentInterval;
		this->humanPresentTrigger=new TimeTrigger(0,humanNotPresentInterval,false,[this](){onHumanPresentTrigger();});
		this->externalFunction=onChangeFunction;
		this->analogLevelForHigh=analogLevelForHigh;
		setSetAllowed(0, false);
	}
	virtual ~Pir_Sensor(){};

	boolean loop(){
		humanPresentTrigger->loop();
		if(analogLevelForHigh!=0){
			processInterrupt();
		}
		return PinDigital::loop();
	}

	int getVal() override{
		if(analogLevelForHigh!=0){
			return (getValAnalog()>=analogLevelForHigh)?HIGH:LOW;
		}

		return PinDigital::getVal();
	}

	void processPinChange(){
		Serial.print("Pir change ");
		Serial.println(getVal());

		if(isOn()){
			onMovementDetected();
		}else{
			onNoMovementDetected();
		}
	}

	boolean isHumanPresented(){
		return humanPresented;
	}

	void onMovementDetected(){
		Serial.println(FPSTR("-------handleMovementDetected"));

		boolean executeAction=!humanPresented && externalFunction!=nullptr;
		humanPresented=true;
		humanPresentTrigger->setActive(false);

		if(executeAction){
			externalFunction();
		}

	}

	void onNoMovementDetected(){
		//Serial.println("-------handle NO MovementDetected");
		humanPresentTrigger->saveTime();
		humanPresentTrigger->setActive(true);
	}

	void onHumanPresentTrigger(){
		Serial.println(FPSTR("-------no movement during period"));

		humanPresented=false;
		humanPresentTrigger->setActive(false);

		if(externalFunction!=nullptr){
			externalFunction();
		}
	}

	virtual void displayDetails() override{
		Serial.print(FPSTR("Read pir"));
		Serial.print(FPSTR(" dR="));
		Serial.print(String(digitalRead(pin)));
		Serial.print(FPSTR(" aR="));
		Serial.print(String(analogRead(pin)));
		Serial.print(FPSTR(" int="));
		Serial.print(String(isInterruptAttached()));
		Serial.print(FPSTR("* Pir changed humanPresented="));
		Serial.print(isHumanPresented());
		Serial.print(FPSTR(" on="));
		Serial.println(isOn());

		Serial.println();
	};

private:
	TimeTrigger *humanPresentTrigger;

	uint humanNotPresentInterval=30000;
	boolean humanPresented=false;

	std::function<void(void)> externalFunction;
	uint16_t analogLevelForHigh=0;
};

#endif /* LIBRARIES_MEASURER_PIRDETECTOR_H_ */
