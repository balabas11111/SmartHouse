/*
 * Pin.h
 *
 *  Created on: 13 ����. 2019 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_PIN_PIN_H_
#define LIBRARIES_ENTITYLIBSIMPLE_PIN_PIN_H_

#include <Arduino.h>
#include <FunctionalInterrupt.h>
#include <utils/DeviceUtils.h>

class Pin {
public:
	Pin(uint8_t pin, uint8_t pinMod, uint8_t onLevel = HIGH){
		this->pin = pin;
		this->pinMod = pinMod;

		if(onLevel != HIGH){
			this->offLevel = HIGH;
		}else{
			this->offLevel = LOW;
		}

		this->onLevel = onLevel;

		if(validateMode()){
			pinMode(pin, pinMod);
		}
	};
	virtual ~Pin(){};

	virtual uint8_t getValue(){
		return digitalRead(pin);
	}

	virtual void setValue(uint8_t value){
		UNUSED(value);
	}

	void setOnOpposite(){
		setOn(!isOn());
	}

	void setOn(bool on = true){
		setValue(on?onLevel:offLevel);
	}

	bool isOn(){
		return getValue() == onLevel;
	}
protected:
	uint8_t pin;
	uint8_t pinMod;

	void attachPinInterrupt(std::function<void(void)> func, int mode){
		if(mode<1){
			Serial.print(FPSTR("Unknown interrupt mode "));
			Serial.println(mode);
		}
		attachInterrupt(this->pin, func, mode);

		Serial.print(FPSTR("Interrupt attached to pin"));
		Serial.println(this->pin);
	}
	virtual bool validateMode() {return true;}

	uint8_t onLevel = HIGH;
	uint8_t offLevel = LOW;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_PIN_PIN_H_ */
