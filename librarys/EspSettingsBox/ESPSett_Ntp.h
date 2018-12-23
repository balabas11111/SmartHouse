/*
 * NtpSettingsBox.h
 *
 *  Created on: 22 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ESPSETTINGSBOX_ESPSETT_NTP_H_
#define LIBRARIES_ESPSETTINGSBOX_ESPSETT_NTP_H_

#include "Arduino.h"
#include <ESPExtraSettingsBox.h>

/*
const char NTP_enabled[]             PROGMEM ="enabled";
const char NTP_poolServerName[]      PROGMEM ="poolServerName";
const char NTP_timeOffset[]          PROGMEM ="timeOffset";
const char NTP_updateInterval[]      PROGMEM ="updateInterval";
const char NTP_timeTriggerInterval[] PROGMEM ="timeTriggerInterval";

const uint8_t NTP_enabled_ind             PROGMEM =0;
const uint8_t NTP_poolServerName_ind      PROGMEM =1;
const uint8_t NTP_timeOffset_ind          PROGMEM =2;
const uint8_t NTP_updateInterval_ind      PROGMEM =3;
const uint8_t NTP_timeTriggerInterval_ind PROGMEM =4;

const char NTP_enabled_Def[]             PROGMEM ="false";
const char NTP_poolServerName_Def[]      PROGMEM ="europe.pool.ntp.org";
const char NTP_timeOffset_Def[]          PROGMEM ="7200";
const char NTP_updateInterval_Def[]      PROGMEM ="120";
const char NTP_timeTriggerInterval_Def[] PROGMEM ="1000";

const char* const NTP_EXTRA_KEYS[] PROGMEM={
												NTP_enabled,
												NTP_poolServerName,
												NTP_timeOffset,
												NTP_updateInterval,
												NTP_timeTriggerInterval
											};

const char* const NTP_EXTRA_DEFAULTS[] PROGMEM={
												NTP_enabled_Def,
												NTP_poolServerName_Def,
												NTP_timeOffset_Def,
												NTP_updateInterval_Def,
												NTP_timeTriggerInterval_Def
												};
*/
const char NTP_SETTINGS_BOX_DESCR[] PROGMEM ="Синхронизация времени по NTP";

const uint8_t NTP_enabled             PROGMEM =0;
const uint8_t NTP_poolServerName      PROGMEM =1;
const uint8_t NTP_timeOffset          PROGMEM =2;
const uint8_t NTP_updateInterval      PROGMEM =3;
const uint8_t NTP_timeTriggerInterval PROGMEM =4;

const char* const NTP_EXTRA_KEYS[] PROGMEM={
												"enabled",
												"poolServerName",
												"timeOffset",
												"updateInterval",
												"timeTriggerInterval"
											};
const char* const NTP_EXTRA_DEFAULTS[] PROGMEM={
												"true",
												"europe.pool.ntp.org",
												"7200",
												"120",
												"1000"
												};
class ESPSett_Ntp:public ESPExtraSettingsBox {
public:

	ESPSett_Ntp(){
		this->name=FPSTR(NTP_SETTINGS_BOX_NAME);
		this->keySize=ARRAY_SIZE(NTP_EXTRA_KEYS);
	};

	virtual ~ESPSett_Ntp(){};

	virtual const char* const* getKeys() override{
		return NTP_EXTRA_KEYS;
	}
	virtual const char* const* getDefaults() override{
		return NTP_EXTRA_DEFAULTS;
	}
	virtual String getDescription() override{
		return FPSTR(NTP_SETTINGS_BOX_DESCR);
	}
};

#endif /* LIBRARIES_ESPSETTINGSBOX_ESPNTPSETT_H_ */
