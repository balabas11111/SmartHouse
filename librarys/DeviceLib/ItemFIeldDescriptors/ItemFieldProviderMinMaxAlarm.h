/*
 * ItemFieldProviderThingSpeak.h
 *
 *  Created on: 2 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_ABSTRACTITEMFIELDPROVIDERS_ItemFieldProviderMinMaxAlarm_H_
#define LIBRARIES_DEVICELIB_ABSTRACTITEMFIELDPROVIDERS_ItemFieldProviderMinMaxAlarm_H_

#include <interfaces/ItemFieldDescriptor.h>
#include "Arduino.h"

typedef enum {
	MinMaxAlarm_alarmEnabled, MinMaxAlarm_minVal, MinMaxAlarm_maxVal
} MinMaxAlarmFields;

const char* const MIN_MAX_ALARM_FIELDS[]      PROGMEM=
{
		"alarmEnabled","minVal","maxVal"
};
const char MIN_MAX_ALARM_DEFAULTS[]      PROGMEM=
		"{\"id\": \"1\",\"name\": \"IFP_MinMaxAlarm\", \"items\": [\"alarmEnabled\": \"0\",\"minVal\": \"-512\", \"maxVal\": \"512\"]}";

class ItemFieldProviderMinMaxAlarm: public ItemFieldDescriptor {
public:
	ItemFieldProviderMinMaxAlarm(){};
	virtual ~ItemFieldProviderMinMaxAlarm(){};

	virtual uint8_t getId(){return IFP_MinMaxAlarm_ID;}

	virtual String getName(){return IFP_MinMaxAlarm;}

	virtual uint8_t getSize(){return 3;}

	virtual const char* const* getKeys(){return MIN_MAX_ALARM_FIELDS;}

	virtual const char* getDefaults(){return MIN_MAX_ALARM_DEFAULTS;}

};

#endif /* LIBRARIES_DEVICELIB_ABSTRACTITEMFIELDPROVIDERS_ITEMFIELDPROVIDERTHINGSPEAK_H_ */
