/*
 * EntityProcessor.h
 *
 *  Created on: 11 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_INTERFACES_ENTITYPROCESSOR_H_
#define LIBRARIES_DEVICELIB_INTERFACES_ENTITYPROCESSOR_H_

#include <ArduinoJson.h>

class EntityProcessor {
public:
	virtual ~EntityProcessor(){};

	virtual JsonArray& getAbstractItems(JsonArray& items,uint8_t pageId);
	virtual JsonArray& postAbstractItems(JsonArray& items,uint8_t pageId);
};

#endif /* LIBRARIES_DEVICELIB_INTERFACES_ENTITYPROCESSOR_H_ */
