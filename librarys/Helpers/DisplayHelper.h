/*
 * DisplayHelper.h
 *
 *  Created on: 20 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DISPLAYHELPER_DISPLAYHELPER_H_
#define LIBRARIES_DISPLAYHELPER_DISPLAYHELPER_H_

#include "Arduino.h"
#include <Initializable.h>

class DisplayHelper: public Initializable {

public:
	DisplayHelper(boolean _init);
	virtual ~DisplayHelper(){};

	virtual boolean initialize(boolean _init) override{
		Serial.println("--------------Display Helper init------------");

		Serial.println("--------------Display Helper complete------------");
		initialized=_init;
		return true;
	}

	boolean clearDisplay(){
		Serial.println("Display cleared");
		return true;
	}

	boolean addStringToDisplay(String str,int row,int col,String sender){
		Serial.println(str+" row="+String(row)+" col="+String(col)+" sender="+sender);
		return true;
	}
};

#endif /* LIBRARIES_DISPLAYHELPER_DISPLAYHELPER_H_ */
