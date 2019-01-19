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
		"Nothing to send"
};
/*----------------------------StatusMessage--------------------*/

#endif /* LIBRARIES_DEVICELIB_LOCALES_CONSTSLOCRU_H_ */
