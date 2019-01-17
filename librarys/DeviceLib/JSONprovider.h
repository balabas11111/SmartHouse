/*
 * JSONprovider.h
 *
 *  Created on: 1 џэт. 2019 у.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_JSONPROVIDER_H_
#define LIBRARIES_DEVICELIB_JSONPROVIDER_H_

#include "Arduino.h"
#include "Nameable.h"

#define KIND_PINDIGITAL "KIND_PINDIGITAL"
#define KIND_SENSOR "KIND_SENSOR"
#define KIND_SERVICE "KIND_SERVICE"

class JSONprovider:public Nameable {
public:
	JSONprovider(){};
	virtual ~JSONprovider(){};

	//virtual String getName()=0;
	virtual String getJson()=0;
};

#endif /* LIBRARIES_DEVICELIB_JSONPROVIDER_H_ */
