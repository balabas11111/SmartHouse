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

	virtual void sendAsEventSourceNow(JsonObject& obj)=0;
	virtual void sendAsEventSourceEntityNow(int entityId)=0;

	virtual void setSendRequired(bool required=true){
		sendRequired=required;
	}

	bool getSendRequired(){
		return sendRequired;
	}
protected:
	bool sendRequired=false;
};

#endif /* LIBRARIES_ENTITYLIB_EVENTSENDER_H_ */
