/*
 * EspDisplaySett.h
 *
 *  Created on: 23 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ESPSETTINGSBOX_EspSett_Alarm_H_
#define LIBRARIES_ESPSETTINGSBOX_EspSett_Alarm_H_

#include "Arduino.h"
#include "ESPExtraSettingsBox.h"

const char ALARM_SETTINGS_BOX_DESCR[]         PROGMEM ="Режим тревоги";

const uint8_t ALARM_sendInterval_ind            PROGMEM =0;
const uint8_t ALARM_beepOnAlert_ind             PROGMEM =1;
const uint8_t ALARM_sendNotifAlertStart_ind     PROGMEM =2;
const uint8_t ALARM_sendNotifAlertStop_ind      PROGMEM =3;

const char* const ALARM_EXTRA_KEYS[]          PROGMEM=
{
	"sendInterval",
	"beepOnAlert",
	"sendNotifAlertStart",
	"sendNotifAlertStop"
};

const char* const ALARM_EXTRA_DEFAULTS[]      PROGMEM=
{
	"30000",
	"true",
	"false",
	"false"
};

class EspSett_Alarm:public ESPExtraSettingsBox {
public:
	EspSett_Alarm(){
		keySize=ARRAY_SIZE(ALARM_EXTRA_KEYS);
	}

	virtual ~EspSett_Alarm(){};

	virtual int getId(){
		return ExtraBox_Alarm;
	}

	virtual String getName() override{
		return FPSTR(ALARM_SETTINGS_BOX_NAME);
	}

	virtual const char* const* getKeys() override{
		return ALARM_EXTRA_KEYS;
	}

	virtual const char* const* getDefaults() override{
		return ALARM_EXTRA_DEFAULTS;
	}

	virtual String getDescription() override{
		return FPSTR(ALARM_SETTINGS_BOX_DESCR);
	}
	virtual String getKind()override{
		return FPSTR(SETTINGS_KIND_device);
	}
};

#endif /* LIBRARIES_ESPSETTINGSBOX_ESPDISPLAYSETT_H_ */
