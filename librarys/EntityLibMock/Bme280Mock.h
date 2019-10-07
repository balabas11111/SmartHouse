/*
 * Bme280Mock.h
 *
 *  Created on: 26 мар. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBMOCK_BME280MOCK_H_
#define LIBRARIES_ENTITYLIBMOCK_BME280MOCK_H_

#include <DeviceUtils.h>
#include "Arduino.h"
#include "AMock.h"

class Bme280Mock:public AMock {
public:
	Bme280Mock(){};
	virtual ~Bme280Mock(){};

	float readHumidity(){
		return 50+random(0,10);
	}
	float readTemperature(){
		return 30-random(0,10);
	}
	float readPressure(){
		return 100500+random(0,100);
	}
};

#endif /* LIBRARIES_ENTITYLIBMOCK_BME280MOCK_H_ */
