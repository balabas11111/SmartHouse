/*
 * Looper.h
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_PINDIGITAL_LOOPABLE_H_
#define LIBRARIES_PINDIGITAL_LOOPABLE_H_

#include <Arduino.h>

class Loopable{

public:
	virtual ~Loopable(){};

	virtual boolean loop(){return false;};
	//virtual void displayDetails(){};
};


#endif /* LIBRARIES_PINDIGITAL_LOOPER_H_ */
