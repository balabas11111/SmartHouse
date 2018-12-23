/*
 * NtpSettingsBox.h
 *
 *  Created on: 22 дек. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ESPSETTINGSBOX_ESPNTPSETT_H_
#define LIBRARIES_ESPSETTINGSBOX_ESPNTPSETT_H_

#define ARRAY_SIZE(x) sizeof(x)/sizeof(x[0])
#define VAR_NAME(var) #var

#include "Arduino.h"
#include <ESPExtraSettingsBox.h>

const char NTP_SETTINGS_BOX_NAME[] PROGMEM ="NtpSettingsBox";

const char NTP_ntpEnabled[]              PROGMEM ="ntpEnabled";
const char NTP_NTP_poolServerName[]      PROGMEM ="NTP_poolServerName";
const char NTP_NTP_timeOffset[]          PROGMEM ="NTP_timeOffset";
const char NTP_NTP_updateInterval[]      PROGMEM ="NTP_updateInterval";
const char NTP_NTP_timeTriggerInterval[] PROGMEM ="NTP_timeTriggerInterval";

const uint8_t NTP_ntpEnabled_ind              PROGMEM =0;
const uint8_t NTP_NTP_poolServerName_ind      PROGMEM =1;
const uint8_t NTP_NTP_timeOffset_ind          PROGMEM =2;
const uint8_t NTP_NTP_updateInterval_ind      PROGMEM =3;
const uint8_t NTP_NTP_timeTriggerInterval_ind PROGMEM =4;

const char NTP_ntpEnabled_Def[]              PROGMEM ="false";
const char NTP_NTP_poolServerName_Def[]      PROGMEM ="europe.pool.ntp.org";
const char NTP_NTP_timeOffset_Def[]          PROGMEM ="7200";
const char NTP_NTP_updateInterval_Def[]      PROGMEM ="120";
const char NTP_NTP_timeTriggerInterval_Def[] PROGMEM ="1000";

const char* const EXTRA_KEYS[] PROGMEM={NTP_ntpEnabled,
							NTP_NTP_poolServerName,
							NTP_NTP_timeOffset,
							NTP_NTP_updateInterval,
							NTP_NTP_timeTriggerInterval};

const char* const EXTRA_DEFAULTS[] PROGMEM={NTP_ntpEnabled_Def,
								NTP_NTP_poolServerName_Def,
								NTP_NTP_timeOffset_Def,
								NTP_NTP_updateInterval_Def,
								NTP_NTP_timeTriggerInterval_Def};

class ESPNtpSett:public ESPExtraSettingsBox {
public:

	ESPNtpSett(){
		this->name=FPSTR(NTP_SETTINGS_BOX_NAME);
		this->keySize=ARRAY_SIZE(EXTRA_KEYS);
	};

	virtual ~ESPNtpSett(){};

	virtual const char* const* getDefaults(){
		return EXTRA_DEFAULTS;
	}

	virtual const char* const* getKeys(){
		return EXTRA_KEYS;
	}
};

#endif /* LIBRARIES_ESPSETTINGSBOX_ESPNTPSETT_H_ */
