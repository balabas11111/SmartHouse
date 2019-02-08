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
#include "OLED.h"
#include "EspSettingsBox.h"
#include "AbstractSensor.h"

#define TURN_OFF_SECS 15

//http://javl.github.io/image2cpp/

class DisplayHelperOLED: public DisplayHelperAbstract {

public:
	DisplayHelperOLED(EspSettingsBox* espSettingsBox)
				:DisplayHelperAbstract(espSettingsBox){
		clearDisplaybeforeCurrentpage=true;
	}

	virtual ~DisplayHelperOLED(){};

protected:
//--------------------------------------------
	virtual boolean handleDisplayBegin() override{
		display.begin();
		return true;
	}

	virtual boolean handleDisplayClear() override{
		display.clear();
		return true;
	}

	virtual boolean handleDisplayPower() override{
		if(displayOn){display.on();}else{display.off();}

		return displayOn;
	}

	virtual boolean handleDisplayLine(String str,int row,int col,String sender) override{
		char l[16];
		strcpy(l, str.c_str());

		display.print(l,row,col);

		return true;
	}
//-------------------------------------------
protected:
	OLED display=new OLED(SDA, SCL);
};

#endif /* LIBRARIES_DISPLAYHELPER_DISPLAYHELPER_OLED_H_ */
