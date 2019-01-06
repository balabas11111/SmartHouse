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
/*
const char DISPLAY_SETTINGS_BOX_DESCR[] PROGMEM ="��������� ������";

const char DISPLAY_displayAlvaysOn[]           PROGMEM ="displayAlvaysOn";
const char DISPLAY_displayAutochange[]         PROGMEM ="displayAutochange";

const uint8_t DISPLAY_displayAlvaysOn_ind      PROGMEM =0;
const uint8_t DISPLAY_displayAutochange_ind    PROGMEM =1;

const char DISPLAY_displayAlvaysOn_def[]       PROGMEM ="737840576:AAH_9-PM8knquJ3x1GN-sOTX4NGPNdU50iE";
const char DISPLAY_displayAutochange_def[]     PROGMEM ="JPhZDxJ7MBZV9HPkR0YE6A";

const char* const DISPLAY_EXTRA_KEYS[] PROGMEM={DISPLAY_displayAlvaysOn,
											DISPLAY_displayAutochange};

const char* const DISPLAY_EXTRA_DEFAULTS[] PROGMEM={DISPLAY_displayAlvaysOn_def,
												DISPLAY_displayAutochange_def};
*/
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
		this->name=FPSTR(DISPLAY_SETTINGS_BOX_NAME);
		this->keySize=ARRAY_SIZE(DISPLAY_EXTRA_KEYS);
	}
	virtual ~EspSett_Display();

	virtual const char* const* getKeys() override{
		return DISPLAY_EXTRA_KEYS;
	}

	virtual const char* const* getDefaults() override{
		return DISPLAY_EXTRA_DEFAULTS;
	}

	virtual String getDescription() override{
		return FPSTR(DISPLAY_SETTINGS_BOX_DESCR);
	}
	virtual String getSettingsKind()override{
		return FPSTR(SETTINGS_KIND_device);
	}
};

#endif /* LIBRARIES_ESPSETTINGSBOX_ESPSETT_DISPLAY_H_ */
