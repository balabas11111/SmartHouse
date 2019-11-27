/*
 * Bme280sensor.h
 *
 *  Created on: Mar 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSENSORS_TM1637DISPLAY_H_
#define LIBRARIES_ENTITYLIBSENSORS_TM1637DISPLAY_H_

#include <Arduino.h>
#include <TM1637.h>

#include <EntityUpdate.h>

#define TURN_ON_INTERVAL 60000
//--------------------------------

class TM1637Display: public EntityUpdate {
public:
	TM1637Display(uint8_t pinData, uint8_t pinClock) {
		this->pinData = pinData;
		this->pinClock = pinClock;

		this->tm1637 = new TM1637(pinClock, pinData);
		this->tm1637->clearDisplay();
	}

	virtual ~TM1637Display() {}

	void displayData(int value) {
		init();
		powerOn();
		tm1637->clearDisplay();
		Serial.print(FPSTR("TM1637 value="));
		Serial.println(value);

		if(value<0){
			displayValue[0] = SYMBOL_MINUS;
		} else {
			displayValue[0] = 0x00;
		}

		displayValue[1] = value / 100;
		displayValue[2] = (value % 100) / 10;
		displayValue[3] = (value - displayValue[1] *100 - displayValue[2]*10) %1;

		if(displayValue[1] == 0){
			displayValue[1] = 0x00;
		}
		//displayValue[3] = minute % 10;

		Serial.print(FPSTR("TM1637 value = "));
		for(uint8_t i=0; i<4; i++) {
			Serial.print(displayValue[i]);
		}
		Serial.println();

		tm1637->display(displayValue);
	}

	virtual void loop() override {
		if(on && TURN_ON_INTERVAL + turnOnTime < millis()) {
			powerOff();
		}
	}

protected:
	uint8_t pinData;
	uint8_t pinClock;
	bool initialized = false;
	bool on = false;
	unsigned long turnOnTime = 0;

	void init() {
		if (!initialized) {
			tm1637->set();
			tm1637->init();
			tm1637->point(0);
			initialized = true;
			Serial.println(FPSTR("TM1637 initialized"));
		}
	}

	void powerOn(){
		on = true;
		turnOnTime = millis();
		Serial.println(FPSTR("TM1637 on"));
	}

	void powerOff(){
		on = false;
		tm1637->clearDisplay();
		Serial.println(FPSTR("TM1637 off"));
	}

private:
	TM1637* tm1637;
	int8_t displayValue[4] = {0x00, 0x00, 0x00, 0x00};
};

#endif /* LIBRARIES_ENTITYLIBSENSORS_BME280SENSOR_H_ */
