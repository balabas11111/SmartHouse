/*
 * EspSett_ThSpeak.h
 *
 *  Created on: Feb 7, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ESPSETTINGSBOX_EXTRABOXES_ESPSETT_THSPEAK_H_
#define LIBRARIES_ESPSETTINGSBOX_EXTRABOXES_ESPSETT_THSPEAK_H_


#include "Arduino.h"
#include "ESPExtraSettingsBox.h"
#include "projectConsts.h"

const char THINGSPEAK_SETT_BOX_DESCR[]         PROGMEM ="Настройки ThingSpeak";

enum THINGSPEAK_SETT_BOX_KEYS_ID: uint8_t {
	THINGSPEAK_enabled, THINGSPEAK_interval, THINGSPEAK_userKey, THINGSPEAK_writeKey, THINGSPEAK_readKey, THINGSPEAK_sendKey,
	THINGSPEAK_channel, THINGSPEAK_url
};

const char* const THINGSPEAK_SETT_BOX_KEYS_NAME[]          PROGMEM=
{
	"enabled",
	"interval",
	"userKey",
	"writeKey",
	"readKey",
	"sendKey",
	"channel",
	"url"
};

const char* const THINGSPEAK_SETT_BOX_DEFAULTS[]      PROGMEM=
{
	"false",
	"241",
	"YV1SA7H87PS8RFMU",
	"V8V5G1W2CACCQOMV",
	"SPH0UG4JJZC7GDDH",
	"ZSFBN9V1K2QMB9M1",
	"612324",
	""
};

class EspSett_ThSpeak:public ESPExtraSettingsBox {
public:
	EspSett_ThSpeak(){
		this->keySize=ARRAY_SIZE(THINGSPEAK_SETT_BOX_KEYS_NAME);;
	}
	virtual ~EspSett_ThSpeak();

	virtual uint8_t getId(){
		return ExtraBox_thingSpeak;
	}

	virtual String getName() override{
		return FPSTR(THINGSPEAK_SETTINGS_BOX_NAME);
	}

	virtual String getDefaultValue(int index){
		if(index==THINGSPEAK_url){
			//https://thingspeak.com/channels/"+thSkChId+"/private_show
			String result=FPSTR(MESSAGE_THINGSPEAK_CURRENT_CHANNEL_URL);
					result+=getDefaultValue(THINGSPEAK_readKey);
					result+=FPSTR(MESSAGE_THINGSPEAK_PRIVATE_SHOW);
			return result;
		}
		return String(getDefaults()[index]);
	}

	virtual const char* const* getDefaults() override{
		return THINGSPEAK_SETT_BOX_DEFAULTS;
	}
	virtual const char* const* getKeys() override{
		return THINGSPEAK_SETT_BOX_KEYS_NAME;
	}
	virtual String getDescription() override{
		return FPSTR(THINGSPEAK_SETT_BOX_DESCR);
	}
	String getKind() override{
		return FPSTR(SETTINGS_KIND_publish);
	}
};

#endif /* LIBRARIES_ESPSETTINGSBOX_EXTRABOXES_ESPSETT_THSPEAK_H_ */
