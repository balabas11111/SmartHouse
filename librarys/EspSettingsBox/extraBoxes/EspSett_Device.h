/*
 * EspSett_Device.h
 *
 *  Created on: Feb 7, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ESPSETTINGSBOX_EXTRABOXES_ESPSETT_DEVICE_H_
#define LIBRARIES_ESPSETTINGSBOX_EXTRABOXES_ESPSETT_DEVICE_H_

#include "Arduino.h"
#include "ESPExtraSettingsBox.h"
#include "projectConsts.h"

const char DEVICE_SETT_BOX_DESCR[]         PROGMEM ="Детали устройства";

enum DEVICE_SETT_BOX_KEYS_ID: uint8_t {
	DEVICE_id, DEVICE_firmware, DEVICE_kind, DEVICE_descr, DEVICE_location, DEVICE_locale
};

const char* const DEVICE_SETT_BOX_KEYS_NAME[]          PROGMEM=
{
	"id",
	"firmvare",
	"kind",
	"descr",
	"location",
	"locale"
};

const char* const DEVICE_SETT_BOX_DEFAULTS[]      PROGMEM=
{
	DEVICE_ID,
	DEVICE_FIRMWARE_VER,
	DEVICE_KIND,
	DEVICE_DESCR,
	DEVICE_LOCATION,
	DEVICE_LOCALE
};

class EspSett_Device:public ESPExtraSettingsBox {
public:
	EspSett_Device(){
		this->keySize=ARRAY_SIZE(DEVICE_SETT_BOX_KEYS_NAME);
	}
	virtual ~EspSett_Device(){};

	virtual int getId(){
		return ExtraBox_Device;
	}

	virtual String getName() override{
		return FPSTR(DEVICE_SETTINGS_BOX_NAME);
	}

	virtual String getDefaultValue(int index) override{
		if(index==DEVICE_id){
			return "Sens_"+String(ESP.getChipId());
		}
		return String(getDefaults()[index]);
	}

	virtual const char* const* getDefaults() override{
		return DEVICE_SETT_BOX_DEFAULTS;
	}
	virtual const char* const* getKeys() override{
		return DEVICE_SETT_BOX_KEYS_NAME;
	}
	virtual String getDescription() override{
		return FPSTR(DEVICE_SETT_BOX_DESCR);
	}
	String getKind() override{
		return FPSTR(SETTINGS_KIND_device);
	}
};

#endif /* LIBRARIES_ESPSETTINGSBOX_EXTRABOXES_ESPSETT_DEVICE_H_ */
