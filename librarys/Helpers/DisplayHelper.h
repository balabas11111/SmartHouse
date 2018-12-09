/*
 * DisplayHelper.h
 *
 *  Created on: 20 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DISPLAYHELPER_DISPLAYHELPER_H_
#define LIBRARIES_DISPLAYHELPER_DISPLAYHELPER_H_

#include "Arduino.h"


class DisplayHelper{

public:
	virtual ~DisplayHelper(){};

	virtual boolean clearDisplay()=0;

	virtual boolean addStringToDisplay(String str,int row,int col,String sender)=0;

	virtual boolean displayLine(String str,int row,int col)=0;

	virtual uint8_t getStartRow()=0;

	virtual uint8_t getRowIncrement()=0;

	virtual uint8_t getTotalDetectedSensors()=0;

	virtual uint8_t getTotalPages()=0;

	virtual void lock(bool doLock)=0;
};

#endif /* LIBRARIES_DISPLAYHELPER_DISPLAYHELPER_H_ */
