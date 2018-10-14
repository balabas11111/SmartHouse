/*
 * Looper.h
 *
 *  Created on: 13 окт. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_PINDIGITAL_LOOPER_H_
#define LIBRARIES_PINDIGITAL_LOOPER_H_

#include <Loopable.h>
#include <Arduino.h>

class Looper{

public:
	Looper(Loopable** items,uint8_t sizeIn);
	String displayDetails();
	boolean loop();
private:
	Loopable** items;
	uint8_t size;
	uint8_t arraySize;
};



#endif /* LIBRARIES_PINDIGITAL_LOOPER_H_ */
