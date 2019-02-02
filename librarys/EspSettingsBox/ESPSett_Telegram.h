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
/*
const char TELEGRAM_SETTINGS_BOX_DESCR[] PROGMEM ="��������� ���� Telegram";

const char TELEGRAM_telegramApiKey[]              PROGMEM ="telegramApiKey";
const char TELEGRAM_telegramReceivers[]           PROGMEM ="telegramReceivers";
const char TELEGRAM_telegramSenders[]             PROGMEM ="telegramSenders";
const char TELEGRAM_telegramBotEnabled[]          PROGMEM ="telegramBotEnabled";

const uint8_t TELEGRAM_telegramApiKey_ind         PROGMEM =0;
const uint8_t TELEGRAM_telegramReceivers_ind      PROGMEM =1;
const uint8_t TELEGRAM_telegramSenders_ind        PROGMEM =2;
const uint8_t TELEGRAM_telegramBotEnabled_ind     PROGMEM =3;

const char TELEGRAM_telegramApiKey_Def[]          PROGMEM ="737840576:AAH_9-PM8knquJ3x1GN-sOTX4NGPNdU50iE";
const char TELEGRAM_telegramReceivers_Def[]       PROGMEM ="JPhZDxJ7MBZV9HPkR0YE6A";
const char TELEGRAM_telegramSenders_Def[]         PROGMEM ="JPhZDxJ7MBZV9HPkR0YE6A";
const char TELEGRAM_telegramBotEnabled_Def[]      PROGMEM ="false";

const char* const TELEGRAM_EXTRA_KEYS[] PROGMEM={TELEGRAM_telegramApiKey,
										TELEGRAM_telegramReceivers,
										TELEGRAM_telegramSenders,
										TELEGRAM_telegramBotEnabled};

const char* const TELEGRAM_EXTRA_DEFAULTS[] PROGMEM={TELEGRAM_telegramApiKey_Def,
											TELEGRAM_telegramReceivers_Def,
											TELEGRAM_telegramSenders_Def,
											TELEGRAM_telegramBotEnabled_Def};
*/
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
	virtual String getSettingsKind()override{
		return FPSTR(SETTINGS_KIND_net);
	}
};

#endif /* LIBRARIES_ESPSETTINGSBOX_ESPSETTTELEGRAM_H_ */
