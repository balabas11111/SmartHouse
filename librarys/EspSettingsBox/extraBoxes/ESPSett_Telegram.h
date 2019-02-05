/*
 * ESPTelegramSett.h
 *
 *  Created on: 22 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ESPSETTINGSBOX_ESPSett_Telegram_H_
#define LIBRARIES_ESPSETTINGSBOX_ESPSett_Telegram_H_

#include "Arduino.h"
#include "ESPExtraSettingsBox.h"

const char TELEGRAM_SETTINGS_BOX_DESCR[] PROGMEM ="Настройка бота Telegram";

const uint8_t TELEGRAM_apiKey                PROGMEM =0;
const uint8_t TELEGRAM_enabledReceivers      PROGMEM =1;
const uint8_t TELEGRAM_enabledSenders        PROGMEM =2;
const uint8_t TELEGRAM_botEnabled            PROGMEM =3;

const char* const TELEGRAM_EXTRA_KEYS[] PROGMEM=
{
	"apiKey",
	"enabledReceivers",
	"enabledSenders",
	"botEnabled"
};
const char* const TELEGRAM_EXTRA_DEFAULTS[] PROGMEM=
{
	"737840576:AAH_9-PM8knquJ3x1GN-sOTX4NGPNdU50iE",
	"JPhZDxJ7MBZV9HPkR0YE6A",
	"JPhZDxJ7MBZV9HPkR0YE6A",
	"false"
};

class ESPSett_Telegram:public ESPExtraSettingsBox {
public:
	ESPSett_Telegram(){
		//this->name=FPSTR(TELEGRAM_SETTINGS_BOX_NAME);
		//this->keySize=ARRAY_SIZE(TELEGRAM_EXTRA_KEYS);
	};
	virtual ~ESPSett_Telegram(){};

	virtual uint8_t getId(){
		return ExtraBox_Ntp;
	}

	virtual String getName() override{
		return FPSTR(TELEGRAM_SETTINGS_BOX_NAME);
	}

	virtual const char* const* getKeys() override{
		return TELEGRAM_EXTRA_KEYS;
	}
	virtual const char* const* getDefaults() override{
		return TELEGRAM_EXTRA_DEFAULTS;
	}
	virtual String getDescription() override{
		return FPSTR(TELEGRAM_SETTINGS_BOX_DESCR);
	}
	virtual String getKind()override{
		return FPSTR(SETTINGS_KIND_net);
	}
};

#endif /* LIBRARIES_ESPSETTINGSBOX_ESPSETTTELEGRAM_H_ */
