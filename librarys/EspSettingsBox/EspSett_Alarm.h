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
/*
const char ALARM_alamSendInterval[]           PROGMEM ="alamSendInterval";

const uint8_t ALARM_alamSendInterval_ind      PROGMEM =0;

const char ALARM_alamSendInterval_def[]       PROGMEM ="737840576:AAH_9-PM8knquJ3x1GN-sOTX4NGPNdU50iE";


const char* const ALARM_EXTRA_KEYS[]          PROGMEM={ALARM_alamSendInterval};

const char* const ALARM_EXTRA_DEFAULTS[]      PROGMEM={ALARM_alamSendInterval_def};
*/
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
		//this->name=FPSTR(ALARM_SETTINGS_BOX_NAME);
		//this->keySize=ARRAY_SIZE(ALARM_EXTRA_KEYS);
	}
	virtual ~EspSett_Alarm(){};

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
	virtual String getSettingsKind()override{
		return FPSTR(SETTINGS_KIND_device);
	}
};

#endif /* LIBRARIES_ESPSETTINGSBOX_ESPDISPLAYSETT_H_ */
