/*
 * projectconsts.h
 *
 *  Created on: Jan 2, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef PROJECTCONSTS_H_
#define PROJECTCONSTS_H_

#define DEVICE_LOCALE_RU "Ru"
//#define DEVICE_LOCALE_EN "En"

#ifdef DEVICE_LOCALE_RU
	#include <projectConstsLoc_Ru.h>
	#include <Locales/ConstsLoc_Ru.h>
#endif

#ifdef DEVICE_LOCALE_EN
	#include <projectConstsLoc_En.h>
	#include <ConstsLoc_En.h>
#endif

#define DEVICE_FIRMWARE "NTPclock Alarm BME-280 PIR 4_display ThingSpeak"

#ifndef DEVICE_LOCALE
	#define DEVICE_DESCR "NTP часы, будильник, Температура/Влажность, Датчик движения"
	#define DEVICE_LOCATION "Размещение часов"
#endif

#define GET_FILES_ALLOWED 3

/* NTP Clock Device with:
//-------------------------------------------------------------
 4 digits display;
 NTP synchronisation;
 BME-280
 PIR sensor;
//-------------------------------------------------------------
 Connection

  PirSensor          D4

  SignalLed PIR      D0
  I2C                D1, D2
  DS18D20            D3

  TM1637 - Display   D6 D7

  Beeper             D5
  Button             D8
 */


//specific settings for project
//#define HAS_LOCAl_CLOCK 1
//---------------------TimeIntervalService parameters-------------------------
#define CUSTOM_TIME_SERVICE_PARAMS 1
#define CUSTOM_TIME_SERVICE_LOC_PARAMS 1

#define ALARM_SIZE 10

#define RESCHEDULE_INTERVAL_INDEX 5
#define MULTIDAILY_INTERVAL_INDEX 3
#define PERIODIC_INTERVAL_INDEX 1

#define TIME_SERV_SERV_VALS 1

const char TimeIntervalService_ServVals[] PROGMEM="\
\"id\":\"123\",\"name\":\"TimeIntervalService\",\"type\":\"ScheduleService\",\"size\":\"1\"\
,\"descr\":\"TimeIntervalService_DESCR\"\
,\"rescheduleIndex\":\"5\",\"multidailyIndex\":\"3\",\"periodicIndex\":\"1\"\
,\"intervalKindId\": [\"ALARM\", \"SOCKET_1_ON\"]";

#define INTERVAL_KIND_ALARM 0
#define INTERVAL_KIND_SOCKET_1_ON 1

const char* const IntervalKind_Ids[] PROGMEM={
		"ALARM", "SOCKET_1_ON"
};
//---------------------------------------------------------------------------

#endif /* PROJECTCONSTS_H_ */
