/*
 * DHT22Mock.h
 *
 *  Created on: 26 ���. 2019 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBMOCK_DHT22MOCK_H_
#define LIBRARIES_ENTITYLIBMOCK_DHT22MOCK_H_

#include <DeviceUtils.h>
#include "Arduino.h"
#include "AMock.h"

class MAX6675Mock:public AMock {
public:
	MAX6675Mock(){};
	virtual ~MAX6675Mock(){};

	MAX6675Mock(uint8_t pinSCK, uint8_t pinCS, uint8_t pinSO){
		UNUSED(pinSCK);
		UNUSED(pinCS);
		UNUSED(pinSO);
	}

	double readCelsius(){
		return 60+random(80, 120);
	}
};

#endif /* LIBRARIES_ENTITYLIBMOCK_DHT22MOCK_H_ */
