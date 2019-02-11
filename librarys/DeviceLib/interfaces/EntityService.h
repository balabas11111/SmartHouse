/*
 * EntityService.h
 *
 *  Created on: 11 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_INTERFACES_ENTITYSERVICE_H_
#define LIBRARIES_DEVICELIB_INTERFACES_ENTITYSERVICE_H_

#include <ArduinoJson.h>
#include <interfaces/Nameable.h>

class EntityService: public Nameable {
public:
	virtual JsonArray& getAbstractItems(JsonArray& items,uint8_t pageId)=0;
	virtual JsonArray& postAbstractItems(JsonArray& items,uint8_t pageId)=0;
};

#endif /* LIBRARIES_DEVICELIB_INTERFACES_ENTITYSERVICE_H_ */
