/*
 * DHT22Mock.h
 *
 *  Created on: 26 мар. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBMOCK_DHT22MOCK_H_
#define LIBRARIES_ENTITYLIBMOCK_DHT22MOCK_H_

#include "Arduino.h"
#include "AMock.h"
#include "ObjectUtils.h"

class DHT22Mock:public AMock {
public:
	DHT22Mock(){};
	virtual ~DHT22Mock(){};

	DHT22Mock(uint8_t pin, uint8_t type){ }

	float readHumidity(){
		return 60+random(1, 15);
	}
	float readTemperature(){
		return 10+random(10, 20);
	}

};

#endif /* LIBRARIES_ENTITYLIBMOCK_DHT22MOCK_H_ */
