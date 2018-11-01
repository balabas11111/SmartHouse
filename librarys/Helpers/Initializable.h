/*
 * Initializable.h
 *
 *  Created on: 20 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_PINEVENTLIB_INITIALIZABLE_H_
#define LIBRARIES_PINEVENTLIB_INITIALIZABLE_H_

#include "Arduino.h"

class Initializable{

public:
	virtual ~Initializable(){};

	boolean init(){
		//Serial.println("Initializer called");
		return initialize(true);
	}

	virtual boolean initialize(boolean _init)=0;

protected:

	boolean initialized;
};



#endif /* LIBRARIES_PINEVENTLIB_INITIALIZABLE_H_ */
