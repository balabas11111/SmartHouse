/*
 * I2Chelper.h
 *
 *  Created on: 13 окт. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_I2CHELPER_I2CHELPER_H_
#define LIBRARIES_I2CHELPER_I2CHELPER_H_

#include <Arduino.h>

class I2Chelper{

public:
	I2Chelper(uint8_t clockPin,uint8_t dataPin,boolean active);
	void init();
	String scan();
private:
	uint8_t sda;
	uint8_t scl;
	boolean active;
	uint8_t devCount;

	void initWire();
};


#endif /* LIBRARIES_I2CHELPER_I2CHELPER_H_ */
