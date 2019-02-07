/*
 * EspSett_ThSpeak.h
 *
 *  Created on: Feb 7, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ESPSETTINGSBOX_EXTRABOXES_ESPSETT_SMARTHOUSE_H_
#define LIBRARIES_ESPSETTINGSBOX_EXTRABOXES_ESPSETT_SMARTHOUSE_H_


#include "Arduino.h"
#include "ESPExtraSettingsBox.h"
#include "projectConsts.h"

const char SMARTHOUSE_SETT_BOX_DESCR[]         PROGMEM ="Настройки Подключения к серверу !!!SmartHouse balabas";

enum SMARTHOUSE_SETT_BOX_KEYS_ID: uint8_t {
	SMARTHOUSE_enabled, SMARTHOUSE_interval, SMARTHOUSE_ip, SMARTHOUSE_serverKey, SMARTHOUSE_ourKey
};

const char* const SMARTHOUSE_SETT_BOX_KEYS_NAME[]          PROGMEM=
{
	"enabled",
	"interval",
	"ip",
	"serverKey",
	"ourKey"
};

const char* const SMARTHOUSE_SETT_BOX_DEFAULTS[]      PROGMEM=
{
	"false",
	"241",
	"192.168.0.2",
	"123456",
	"123456"
};

class EspSett_SmartHouse:public ESPExtraSettingsBox {
public:
	EspSett_SmartHouse(){
		this->keySize=ARRAY_SIZE(SMARTHOUSE_SETT_BOX_KEYS_NAME);;
	}
	virtual ~EspSett_SmartHouse(){};

	virtual uint8_t getId(){
		return ExtraBox_SmartHouse;
	}

	virtual String getName() override{
		return FPSTR(SMARTHOUSE_SETTINGS_BOX_NAME);
	}

	virtual String getDefaultValue(int index){
		return String(getDefaults()[index]);
	}

	virtual const char* const* getDefaults() override{
		return SMARTHOUSE_SETT_BOX_DEFAULTS;
	}
	virtual const char* const* getKeys() override{
		return SMARTHOUSE_SETT_BOX_KEYS_NAME;
	}
	virtual String getDescription() override{
		return FPSTR(SMARTHOUSE_SETT_BOX_DESCR);
	}
	String getKind() override{
		return FPSTR(SETTINGS_KIND_publish);
	}
};

#endif /* LIBRARIES_ESPSETTINGSBOX_EXTRABOXES_SMARTHOUSE_THSPEAK_H_ */
