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

enum DISPLAY_SETT_BOX_KEYS_ID: uint8_t {
	DISPLAY_alvaysOn, DISPLAY_autochangeIntervalSec
};

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
		this->keySize=ARRAY_SIZE(DISPLAY_EXTRA_KEYS);
	}
	virtual ~EspSett_Display(){};

	virtual int getId(){
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
