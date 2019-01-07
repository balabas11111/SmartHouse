/*
 * PirDetector.h
 *
 *  Created on: 20 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_MEASURER_PIRDETECTOR_H_
#define LIBRARIES_MEASURER_PIRDETECTOR_H_

#define MIN_NO_PRESENTED_INTERVAL 5000

#include <PinDigital.h>
#include "Arduino.h"
#include "TimeTrigger.h"
#include "Loopable.h"
#include "AbstractItem.h"

const PROGMEM char PirSensor_name[]="PirSensor";
const PROGMEM char PirSensor_descr[]="Датчик движения";

class Pir_Sensor: public PinDigital {
public:
	Pir_Sensor(String name,uint8_t _pin,std::function<void(void)> onChangeFunction, int analogLevelForHigh, uint humanNotPresentInterval )
		//:PinDigital(name,_pin,[this](){processPinChange();}){
		:PinDigital(name,_pin,[this](){processPinChange();}, INPUT, CHANGE, LOW, LOW,0,FPSTR(PirSensor_name),FPSTR(PinDigital_highLow),FPSTR(PirSensor_descr)){
		this->humanNotPresentInterval=humanNotPresentInterval;
		if(this->humanNotPresentInterval<MIN_NO_PRESENTED_INTERVAL){
			this->humanNotPresentInterval=MIN_NO_PRESENTED_INTERVAL;
		}
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
			//Serial.println("readAnalog");
			lastAnalog=analogRead(pin);
			return (lastAnalog>=analogLevelForHigh)?HIGH:LOW;
		}

		return PinDigital::getVal();
	}

	void processPinChange(){
		/*Serial.print(FPSTR("Pir change val="));
		Serial.print(getVal());
		Serial.print(FPSTR(" lastAnalog="));
		Serial.println(lastAnalog);
		*/
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
		if(!humanPresented){
			Serial.println(FPSTR("-------handleMovementDetected"));
		}

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

	String getKind() override{
		return KIND_SENSOR;
	}

private:
	int lastAnalog=0;
	TimeTrigger *humanPresentTrigger;

	uint humanNotPresentInterval=30000;
	boolean humanPresented=false;

	std::function<void(void)> externalFunction;
	int analogLevelForHigh=0;
};

#endif /* LIBRARIES_MEASURER_PIRDETECTOR_H_ */
