/*
 * Looper.h
 *
 *  Created on: 13 окт. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_PINDIGITAL_LOOPABLE_H_
#define LIBRARIES_PINDIGITAL_LOOPABLE_H_

#include <Arduino.h>

class Loopable{

public:
	virtual boolean loop()=0;
	virtual String getName();
	virtual String displayDetails();
	//virtual ~Loopable()=0;
};


#endif /* LIBRARIES_PINDIGITAL_LOOPER_H_ */
