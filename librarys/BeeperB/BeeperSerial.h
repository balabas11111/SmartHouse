/*
 * BeeperSerial.h
 *
 *  Created on: 26 ���. 2019 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_BEEPERB_BEEPERSERIAL_H_
#define LIBRARIES_BEEPERB_BEEPERSERIAL_H_

#include "Arduino.h"
#include "BeeperB.h"
#include "EntityUpdate.h"

class BeeperSerial: public EntityUpdate {
public:
	BeeperSerial(BeeperB* beeper, uint8_t itemCount = 3, uint16_t* sound =
			nullptr, uint16_t* delays = nullptr) {
		this->beeper = beeper;
		this->itemCount = itemCount;

		if (sound == nullptr) {
			this->sound = new uint16_t[itemCount];

			for(int i=0; i<itemCount; i++) {
				this->sound[i] = 1000;
			}
		} else {
			this->sound = sound;
		}

		if (delays == nullptr) {
			this->delays = new uint16_t[itemCount];

			for(int i=0; i<itemCount; i++) {
				this->delays[i] = 500;
			}
		} else {
			this->delays = delays;
		}

		this->interval = -1;
	}

	virtual ~BeeperSerial() {}

	virtual void loop() override {
		check();

		if(stopDispatched) {
			autoStop = false;
			stopDispatched = false;
			stop();
		}
	}

	void startWithCheck(boolean clause) {
		if (!clause) {
			return;
		}
		start();

	}

	void startOnceWithAutoStop() {
		start();
		this->autoStop = true;

		for(int i=0; i<3; i++) {
			/*
			Serial.print(FPSTR("i="));
			Serial.print(i);
			Serial.print(FPSTR(" sound="));
			Serial.print(sound[i]);
			Serial.print(FPSTR(" delay="));
			Serial.print(delays[i]);
			*/
		}
	}

	void start() {
		if (running) {
			return;
		}
		first = true;
		current = 0;
		lastTime = millis();
		up = true;
		running = true;
		autoStop = false;
		Serial.println(FPSTR("Start playing"));
	}

	void stop() {
		if (!running) {
			return;
		}
		beeper->playGenerator(false);
		running = false;
		current = 0;
		up = true;
		Serial.println(FPSTR("Stop playing"));
	}

	bool isRunning() {
		return running;
	}

	BeeperB* getBeeper(){
		return this->beeper;
	}

protected:
	BeeperB* beeper;
	uint16_t* sound;
	uint16_t* delays;

	uint8_t itemCount;

	uint8_t current = 0;
	bool running = false;
	bool up = false;
	long lastTime = -1;
	bool autoStop = false;
	bool stopDispatched = false;
	bool first = false;

	boolean check() {
		if (running) {
			if(first) {
				beeper->playGenerator(up);
				first = false;
			}

			uint16_t interv = delays[current];

			if (up) {
				interv = sound[current];
			}

			if (lastTime + interv <= millis()) {

				/*Serial.print(FPSTR("BeeperSerial change"));
				Serial.print(FPSTR(" interv="));
				Serial.print(interv);
				Serial.print(FPSTR(" lastTime="));
				Serial.print(lastTime);
				Serial.print(FPSTR(" up="));
				Serial.print(up);
				Serial.print(FPSTR(" current="));
				Serial.println(current);
*/
				lastTime = millis();
				up = !up;

				if (up) {
					current++;
				}
				if (current == itemCount) {
					current = 0;
					if(autoStop) {
						//Serial.println(FPSTR("auto stop dispatched"));
						stopDispatched = true;
					}
				}

				if(!first) {
					beeper->playGenerator(up);
				}
				first = false;
			}
		}

		return running;
	}
};

#endif /* LIBRARIES_BEEPERB_BEEPERSERIAL_H_ */
