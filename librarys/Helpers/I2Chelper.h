/*
 * I2Chelper.h
 *
 *  Created on: 13 окт. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_I2CHELPER_I2CHELPER_H_
#define LIBRARIES_I2CHELPER_I2CHELPER_H_

#include <Arduino.h>
#include "Initializable.h"

class I2Chelper:public Initializable{

public:
	I2Chelper(uint8_t clockPin,uint8_t dataPin,boolean active);
	virtual boolean initialize(boolean _init) override;

	String scan();
private:
	uint8_t sda;
	uint8_t scl;
	uint8_t devCount;

	void initWire();
};


#endif /* LIBRARIES_I2CHELPER_I2CHELPER_H_ */
