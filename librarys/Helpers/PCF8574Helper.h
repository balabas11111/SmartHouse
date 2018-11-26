/*
 * PCF8574Helper.h
 *
 *  Created on: 25 нояб. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_HELPERS_PCF8574HELPER_H_
#define LIBRARIES_HELPERS_PCF8574HELPER_H_

#include <Arduino.h>
#include <Initializable.h>
#include <PCF8574.h>
#include <PinDigitalVirtual.h>
#include <ESP_Consts.h>
#include <Loopable.h>

class PCF8574Helper: public Initializable,public Loopable {
public:
	PCF8574Helper(PinDigitalVirtual** pins,uint8_t pinCount){
		this->pins=pins;
		this->pinCount=pinCount;
	}

	PCF8574Helper(uint8_t intPin,PinDigitalVirtual** pins,uint8_t pinCount){
		pinCont.enableInterrupt(intPin,[this](void){processInterrupt();});
		this->pins=pins;
		this->pinCount=pinCount;
		this->intEnabled=true;
	}
	virtual ~PCF8574Helper(){};

	virtual boolean initialize(boolean init){
		Serial.println(FPSTR(MESSAGE_PCF8574HELPER_BEGIN_INIT));

		pinCont.begin();

		for(uint8_t i=0;i<pinCount;i++){
			pins[i]->initialize(i,[this](uint8_t p){return this->externalGet(p);}, [this](uint8_t p,uint8_t v){return this->externalSet(p,v);});
			pinCont.pinMode(pins[i]->getPin(),pins[i]->getPinModeInOut());
		}

		Serial.print(FPSTR(MESSAGE_DEVICE_INIT_COUNT));
		Serial.println(pinCount);
		Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));

		initialized=init;
		return init;
	}

	boolean loop(){
		boolean result=false;
		if(!intEnabled){
			processInterrupt();
		}

		for(uint8_t i=0;i<pinCount;i++){
			result = pins[i]->loop() || result;
		}

		return result;
	}

	void processInterrupt(){
		Serial.println(FPSTR("process interrupt on PCF8574"));

		for(uint8_t i=0;i<pinCount;i++){
			if(pins[i]->getPinModeInOut()==INPUT){
				pins[i]->processInterrupt();
			}
		}

		Serial.println(FPSTR("finish interrupt"));
	}

	int externalGet(uint8_t pinId){
		return pinCont.digitalRead(getPinIndex(pinId));
	}
	void externalSet(uint8_t pinId,uint8_t val){
		pinCont.digitalWrite(getPinIndex(pinId), val);
	}

	uint8_t getPinIndex(uint8_t pin){
		if(pins[pin]->getPin()==pin){
			return pin;
		}
		for(uint8_t i=0;i<pinCount;i++){
			if(pins[i]->getPin()==pin){
				return pin;
			}
		}

		return 255;
	}

private:
	PCF8574 pinCont;
	PinDigitalVirtual** pins;
	uint8_t pinCount;
	bool intEnabled=false;
};

#endif /* LIBRARIES_HELPERS_PCF8574HELPER_H_ */
