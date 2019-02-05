/*
 * EspDisplaySett.h
 *
 *  Created on: 23 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ESPSETTINGSBOX_ESPSETT_DISPLAY_H_
#define LIBRARIES_ESPSETTINGSBOX_ESPSETT_DISPLAY_H_

#include "Arduino.h"
#include "ESPExtraSettingsBox.h"

const char DISPLAY_SETTINGS_BOX_DESCR[] PROGMEM ="Экран";

const uint8_t DISPLAY_alvaysOn                 PROGMEM =0;
const uint8_t DISPLAY_autochangeIntervalSec    PROGMEM =1;

const char* const DISPLAY_EXTRA_KEYS[] PROGMEM=
{
	"alwaysOn",
	"autochangeIntervalSec"
};
const char* const DISPLAY_EXTRA_DEFAULTS[] PROGMEM=
{
	"true",
	"0"
};

class EspSett_Display:public ESPExtraSettingsBox {
public:
	EspSett_Display(){
		/*this->name=FPSTR(DISPLAY_SETTINGS_BOX_NAME);
		this->keySize=ARRAY_SIZE(DISPLAY_EXTRA_KEYS);*/
	}
	virtual ~EspSett_Display(){};

	virtual uint8_t getId(){
		return ExtraBox_Display;
	}

	virtual String getName() override{
		return FPSTR(DISPLAY_SETTINGS_BOX_NAME);
	}

	virtual const char* const* getKeys() override{
		return DISPLAY_EXTRA_KEYS;
	}

	virtual const char* const* getDefaults() override{
		return DISPLAY_EXTRA_DEFAULTS;
	}

	virtual String getDescription() override{
		return FPSTR(DISPLAY_SETTINGS_BOX_DESCR);
	}
	virtual String getKind()override{
		return FPSTR(SETTINGS_KIND_device);
	}
};

#endif /* LIBRARIES_ESPSETTINGSBOX_ESPSETT_DISPLAY_H_ */
