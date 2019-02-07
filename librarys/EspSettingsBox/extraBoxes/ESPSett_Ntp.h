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

#define MIN_UPDATE_INTERVAL 36000000

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
		this->keySize=ARRAY_SIZE(NTP_EXTRA_KEYS);
	};

	virtual ~ESPSett_Ntp(){};

	virtual uint8_t getId(){
		return ExtraBox_Ntp;
	}

	virtual String getName() override{
		return FPSTR(NTP_SETTINGS_BOX_NAME);
	}

	virtual const char* const* getDefaults() override{
		return NTP_EXTRA_DEFAULTS;
	}
	virtual const char* const* getKeys() override{
		return NTP_EXTRA_KEYS;
	}
	virtual String getDescription() override{
		return FPSTR(NTP_SETTINGS_BOX_DESCR);
	}
	String getKind() override{
		return FPSTR(SETTINGS_KIND_net);
	}

	virtual boolean isInMemory() override{
		return false;
	}
};

#endif /* LIBRARIES_ESPSETTINGSBOX_ESPNTPSETT_H_ */
