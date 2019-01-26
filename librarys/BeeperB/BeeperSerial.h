/*
 * BeeperSerial.h
 *
 *  Created on: 26 џэт. 2019 у.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_BEEPERB_BEEPERSERIAL_H_
#define LIBRARIES_BEEPERB_BEEPERSERIAL_H_

#include "Arduino.h"
#include "interfaces/Loopable.h"
#include "BeeperB.h"

class BeeperSerial:public Loopable {
public:
	BeeperSerial(BeeperB* beeper,uint16_t* sound,uint16_t* delays,uint8_t itemCount){
		this->beeper=beeper;
		this->sound=sound;
		this->delays=delays;
		this->itemCount=itemCount;
	}

	virtual ~BeeperSerial(){};

	virtual boolean loop() override{
		return check();
	}

	void startWithCheck(boolean clause){
		if(!clause){
			return;
		}
		start();
	}

	void start(){
		if(running){
			return;
		}
		current=0;
		lastTime=millis();
		up=true;
		running=true;
		Serial.println(FPSTR("Start playing"));
	}

	void stop(){
		if(!running){
			return;
		}
		beeper->playGenerator(false);
		running=false;
		current=0;
		up=true;
		Serial.println(FPSTR("Stop playing"));
	}

	boolean isRunning(){
		return running;
	}

protected:
	BeeperB* beeper;
	uint16_t* sound;
	uint16_t* delays;

	uint8_t itemCount;

	uint8_t current;
	boolean running;
	boolean up;
	long lastTime;

	boolean check(){
			if(running){
				beeper->playGenerator(up);

				uint16_t interv=delays[current];

				if(up){
					interv=sound[current];
				}

				if(lastTime+interv<=millis()){
					/*
					Serial.print(FPSTR("BeeperSerial change"));
					Serial.print(FPSTR(" interv="));
					Serial.print(interv);
					Serial.print(FPSTR(" lastTime="));
					Serial.print(lastTime);
					Serial.print(FPSTR(" up="));
					Serial.print(up);
					Serial.print(FPSTR(" current="));
					Serial.println(current);
					*/
					lastTime=millis();
					up=!up;

					if(up){
						current++;
					}
					if(current==itemCount){
						current=0;
					}
				}
			}

			return running;
		}
};

#endif /* LIBRARIES_BEEPERB_BEEPERSERIAL_H_ */
