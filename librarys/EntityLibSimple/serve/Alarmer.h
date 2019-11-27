/*
 * Alarmer.h
 *
 *  Created on: 27 нояб. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_SERVE_ALARMER_H_
#define LIBRARIES_ENTITYLIBSIMPLE_SERVE_ALARMER_H_

#include "Arduino.h"
#include "BeeperSerial.h"
#include "WS2811Led.h"
#include "EntityUpdate.h"

#include <functional>

#define DEFAULT_ALARM_NOTIFY_INTERVAL 60000
#define DEFAULT_ALARM_CHECK_INTERVAL 30000

class Alarmer: public EntityUpdate {
public:
	Alarmer(BeeperSerial* beeper, std::function<bool(void)> checkForAlarmFunction = nullptr, WS2811Led* led = nullptr){
		this->beeper = beeper;
		this->checkForAlarmFunction = checkForAlarmFunction;
		this->led = led;
	};
	virtual ~Alarmer(){}

	void checkForAlarm(bool clause) {
		bool oldState = alarmed;
		alarmed = clause;

		if(alarmed) {
			if(!oldState){
				playSound();
				Serial.println(FPSTR("Alarm ON"));
			}

			redLight();
		}
		if(!alarmed && oldState) {
			greenLight();
			Serial.println(FPSTR("Alarm OFF"));
			this->beeper->getBeeper()->shortBeep();
		}

		Serial.print(FPSTR("CHECK Alarmed = "));
		Serial.println(alarmed);
	}

	virtual void loop() override {
		if(checkForAlarmFunction!=nullptr && lastCheck + DEFAULT_ALARM_CHECK_INTERVAL < millis()) {
			checkForAlarm(checkForAlarmFunction());
			lastCheck = millis();
		}

		if(alarmed && lastNotify + DEFAULT_ALARM_NOTIFY_INTERVAL < millis()) {
			playSound();
		}
	}
private:
	BeeperSerial* beeper;
	WS2811Led* led;
	std::function<bool(void)> checkForAlarmFunction;

	bool alarmed = false;

	unsigned long lastNotify = 0;
	unsigned long lastCheck = 0;

	void playSound() {
		lastNotify = millis();
		beeper->startOnceWithAutoStop();
	}

	void redLight() {
		if(led!=nullptr) {
			led->setRed();
		}
	}
	void greenLight() {
		if(led!=nullptr) {
			led->setGreen();
		}
	}

};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_SERVE_ALARMER_H_ */
