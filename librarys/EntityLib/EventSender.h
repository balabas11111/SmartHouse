/*
 * EventSender.h
 *
 *  Created on: 16 мар. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIB_EVENTSENDER_H_
#define LIBRARIES_ENTITYLIB_EVENTSENDER_H_

#include "ArduinoJson.h"

class EventSender {
public:
	virtual ~EventSender(){};

	virtual void sendAsEventSource(JsonObject& obj)=0;
};

#endif /* LIBRARIES_ENTITYLIB_EVENTSENDER_H_ */
