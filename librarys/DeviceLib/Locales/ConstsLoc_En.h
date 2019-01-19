/*
 * ConstsLocRu.h
 *
 *  Created on: 19 ���. 2019 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_LOCALES_CONSTSLOCRU_H_
#define LIBRARIES_DEVICELIB_LOCALES_CONSTSLOCRU_H_

#include "Arduino.h"
/*----------------------------CommonValues--------------------*/
const char* const DAYS_OF_WEEK_SHORT[] PROGMEM={
		"Mo", "Tu", "We", "Th", "Fr", "Sa", "Su"
};

const char* const DAYS_OF_WEEK[] PROGMEM={
		"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"
};

/*----------------------------CommonValues--------------------*/


/*----------------------------StatusMessage--------------------*/
const PROGMEM char MESSAGE_PROV_NOT_FOUND[]  = "Component provider not found:";
const PROGMEM char MESSAGE_PROC_NOT_FOUND[]  = "Component processor not found:";
const PROGMEM char STATUS_UNKNOWN_DESCR[]  = "Uknown status";

const char* const STATUSES_DESCR[] PROGMEM=
{
		"Ok",
		"Error",
		"Not found",
		"Bad length",
		"Parse error",
		"Update error",
		"Parameter not found",
		"Validation error",
		"Item not found",
		"Failed",
		"Send error",
		"Not specified",
		"Accepted",
		"Saved",
		"Deleted",
		"Functionality disabled",
		"Data sent",
		"Nothing to send",
		"Configuration error"
};
/*----------------------------StatusMessage--------------------*/

/*----------------------------ThingSpeak Settings--------------------*/

const PROGMEM char MESSAGE_THINGSPEAK_NO_SENSORS[]="No sensors presented";
const PROGMEM char MESSAGE_THINGSPEAK_PUBLISH_NOT_ALLOWED[]="ThingSpeak publish is not allowed";
const PROGMEM char MESSAGE_THINGSPEAK_NO_USER_SPECIFIED[]="User, password, keys of ThingSpeak are not specified";

/*----------------------------ThingSpeak Settings--------------------*/

#endif /* LIBRARIES_DEVICELIB_LOCALES_CONSTSLOCRU_H_ */
