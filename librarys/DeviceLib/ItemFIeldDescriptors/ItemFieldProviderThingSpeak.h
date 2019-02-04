/*
 * ItemFieldProviderThingSpeak.h
 *
 *  Created on: 2 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_ITEMFIELDDESCRIPTORS_ITEMFIELDPROVIDERTHINGSPEAK_H_
#define LIBRARIES_DEVICELIB_ITEMFIELDDESCRIPTORS_ITEMFIELDPROVIDERTHINGSPEAK_H_

#include <interfaces/ItemFieldDescriptor.h>
#include "Arduino.h"

typedef enum {
	ThingSpeak_fieldId, ThingSpeak_periodicSend, ThingSpeak_autoCreateChannel
} ThingSpeakFields;

const char* const THINGSPEAK_FIELDS[]      PROGMEM=
{
		"fieldId","periodicSend","autoCreateChannel"
};
const char THINGSPEAK_DEFAULT[]      PROGMEM=
		"{\"id\": \"0\",\"name\": \"IFP_ThingSpeak\", \"items\": [\"fieldId\": \"0\",\"periodicSend\": \"1\", \"autoCreateChannel\": \"1\"]}";

class ItemFieldProviderThingSpeak: public ItemFieldDescriptor {
public:
	ItemFieldProviderThingSpeak(){};
	virtual ~ItemFieldProviderThingSpeak(){};

	virtual uint8_t getId(){return IFP_ThingSpeak_ID;}

	virtual String getName(){return IFP_ThingSpeak;}

	virtual uint8_t getSize(){return sizeof(THINGSPEAK_FIELDS);}

	virtual const char* const* getKeys(){return THINGSPEAK_FIELDS;}

	virtual const char* getDefaults(){return THINGSPEAK_DEFAULT;}

};

#endif /* LIBRARIES_DEVICELIB_ITEMFIELDDESCRIPTORS_ITEMFIELDPROVIDERTHINGSPEAK_H_ */
