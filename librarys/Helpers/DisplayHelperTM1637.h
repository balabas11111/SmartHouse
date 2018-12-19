/*
 * DisplayHelper.h
 *
 *  Created on: 20 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DISPLAYHELPER_DISPLAYHELPER_OLED_H_
#define LIBRARIES_DISPLAYHELPER_DISPLAYHELPER_OLED_H_

#include <DisplayHelperAbstract.h>
#include "Arduino.h"
#include "TM1637.h"
#include "EspSettingsBox.h"
#include "AbstractItem.h"

#define TURN_OFF_SECS 15

class DisplayHelperTM1637: public DisplayHelperAbstract {

public:
	DisplayHelperTM1637(EspSettingsBox* espSettingsBox,TM1637* timeDisplay)
				:DisplayHelperAbstract(espSettingsBox){
		this->timeDisplay=timeDisplay;

		timeDisplay->set(2);
		timeDisplay->init();
	}

	virtual ~DisplayHelperTM1637(){};

	virtual boolean displayActivity(boolean val){
		timeDisplay->point(val);
		return val;
	};

protected:
//--------------------------------------------
	virtual boolean handleDisplayBegin() override{
		int8_t data[4]={1,0,10,13};
		timeDisplay->display(data);

		return true;
	}

	virtual boolean handleDisplayClear() override{
		int8_t data[4]={0,0,0,0};
		timeDisplay->display(data);

		return true;
	}

	virtual boolean displayLine(int8_t* numbers,uint8_t numCount) override{
		timeDisplay->display(numbers);
		return true;
	}


//-------------------------------------------
protected:
	TM1637* timeDisplay;
};

#endif /* LIBRARIES_DISPLAYHELPER_DISPLAYHELPER_OLED_H_ */
