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
	virtual boolean loop()=0;
	virtual void displayDetails(){};
	virtual ~Loopable(){};
};


#endif /* LIBRARIES_PINDIGITAL_LOOPER_H_ */
