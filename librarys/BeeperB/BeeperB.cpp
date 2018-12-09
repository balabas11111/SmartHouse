/*
 * ButtonB.cpp
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */
#include "Arduino.h"
#include "BeeperB.h"

const int defaultBeepDelay=250;
const int defaultBeep=500;

	BeeperB::BeeperB(uint8_t pin,uint8_t NO_PLAY_FLAG,uint8_t PLAY_FLAG,boolean sendNoPlayFLag){
		this->_pin=pin;
		this->_NO_PLAY_FLAG=NO_PLAY_FLAG;
		this->_PLAY_FLAG=PLAY_FLAG;
		this->useWriteMethodInsteadPlay=sendNoPlayFLag;

		noPlayGenerator();
	}
	void BeeperB::init(){
		pinMode(_pin, OUTPUT);
		digitalWrite(_pin, _NO_PLAY_FLAG);
	}
	void BeeperB::shortBeep(){
		doBeep(0);
	}
	void BeeperB::longBeep(){
		doBeep(5000);
	}
	void BeeperB::playGenerator(){
		digitalWrite(_pin, _PLAY_FLAG);
	}
	void BeeperB::noPlayGenerator(){
		digitalWrite(_pin, _NO_PLAY_FLAG);
	}
	void BeeperB::play(unsigned int frequency, unsigned long duration){
		if(useWriteMethodInsteadPlay){

		}
		tone(_pin,frequency,duration);
	}
	void BeeperB::noPlay(){
		noTone(_pin);
		if(useWriteMethodInsteadPlay){
			noPlayGenerator();
		}
	}

	void BeeperB::doBeep(unsigned long duration){
		if(duration==0){
			duration=defaultBeep;
		}

		int del=duration-defaultBeepDelay;

		if(del<=0){
			duration=defaultBeepDelay+1;
		}

	    play(2500, del);
	    delay(duration);
	    noPlay();
	}



