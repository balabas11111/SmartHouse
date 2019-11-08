/*
 * DataSelector.h
 *
 *  Created on: 21 ����. 2019 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_DATASELECTOR_H_
#define LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_DATASELECTOR_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <utils/JsonObjectUtil.h>

#include <EntityJsonRequestResponse.h>

class DataSelector {
public:
	DataSelector() {}
	virtual ~DataSelector() {}

	virtual void selectData(EntityJsonRequestResponse* req) = 0;

};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_DATASELECTOR_H_ */
