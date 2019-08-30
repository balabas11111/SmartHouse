/*
 * TM1637DisplayTime.h
 *
 *  Created on: 25 мар. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSENSORS_TM1637DISPLAYTIME_H_
#define LIBRARIES_ENTITYLIBSENSORS_TM1637DISPLAYTIME_H_

#include "UpdateAble.h"
#include "NtpBox.h"

class TM1637DisplayTime {
public:
	TM1637DisplayTime(){
	};
	virtual ~TM1637DisplayTime(){};

	virtual void update(){

	}
	virtual void updateTime(int8_t* vals){
		putToDisplay(vals);
	}
	virtual void display(int8_t* vals){
		putToDisplay(vals);
	}
	virtual void tick(){
		displaySeparator(!separState);
	}
protected:
	bool separState=false;
	void displaySeparator(bool display){
		separState=display;
	}
	void putToDisplay(int8_t* vals){

	}
};

#endif /* LIBRARIES_ENTITYLIBSENSORS_TM1637DISPLAYTIME_H_ */
