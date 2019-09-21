/*
 * DataSelector.h
 *
 *  Created on: 21 сент. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_DATASELECTOR_H_
#define LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_DATASELECTOR_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <JsonObjectUtil.h>

#include <EntityJsonRequestResponse.h>

class DataSelector {
public:
	DataSelector() {}
	virtual ~DataSelector() {}

	virtual void selectData(EntityJsonRequestResponse* req) = 0;

};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_DATASELECTOR_H_ */
