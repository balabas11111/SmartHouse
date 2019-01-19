/*
 * ProjectConstsRu.h
 *
 *  Created on: 19 ���. 2019 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ESP8266_NTP_PIR_PROJECTCONSTSLOC_EN_H_
#define LIBRARIES_ESP8266_NTP_PIR_PROJECTCONSTSLOC_EN_H_

#include "Arduino.h"

#define DEVICE_LOCALE 1

//---------------------Device parameters-------------------------
#define DEVICE_DESCR "NTP clock, Alarm clock, Temperature/Humidity, Movement sensor"
#define DEVICE_LOCATION "Device place to be specified"
//---------------------Device parameters-------------------------

//---------------------TimeIntervalService parameters-------------------------
const char TimeIntervalService_ServLocalVals[] PROGMEM=
",\"intervalKind\":[\"AlarmClock\", \"Socket1  - on\"]\
,\"intervalType\":[\"Once\", \"Periodic\", \"Every day\", \"By days\", \"Weekly\", \"Monthly\", \"Quaterly\"]\
,\"intervalState\":[\"New\", \"Wait\", \"Active\", \"Finished\", \"To delete\", \"Inactive\"]\
,\"dayOfWeekShort\":[\"Mo\", \"Tu\", \"We\", \"Th\", \"Fr\", \"Sa\", \"Su\"]\
,\"dayOfWeek\":[\"Monday\",\"Tuesday\",\"Wednesday\",\"Thursday\",\"Friday\",\"Saturday\",\"Sunday\"]";

const char* const IntervalKind_Names[] PROGMEM={
		"Alarm Clock", "Socket 1 - on"
};

const char* const IntervalType_Names[] PROGMEM={
		"Once", "Periodic", "Every day", "By days", "Weekly", "Monthly", "Quaterly"
};
const char* const IntervalState_Names[] PROGMEM={
		"New", "Wait", "Active", "Finished", "To delete", "Inactive"
};
//-------------------TimeIntervalService parameters--------------------------

#endif /* LIBRARIES_ESP8266_NTP_PIR_PROJECTCONSTS_RU_H_ */
