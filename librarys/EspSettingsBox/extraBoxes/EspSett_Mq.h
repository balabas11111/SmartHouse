/*
 * EspSett_Mq.h
 *
 *  Created on: Feb 7, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ESPSETTINGSBOX_EXTRABOXES_ESPSETT_MQ_H_
#define LIBRARIES_ESPSETTINGSBOX_EXTRABOXES_ESPSETT_MQ_H_

#include "ESPExtraSettingsBox.h"
#include "projectConsts.h"

const char MQTT_SETT_BOX_DESCR[]         PROGMEM ="MQTT настройки";

enum MQTT_SETT_BOX_KEYS_ID: uint8_t {
	MQTTE_enabled, MQTT_postinterval, MQTT_server, MQTT_user, MQTT_password, MQTT_port
};

const char* const MQTT_SETT_BOX_KEYS_NAME[]          PROGMEM=
{
	"enabled",
	"interval",
	"server",
	"user",
	"password",
	"port"
};

const char* const MQTT_SETT_BOX_DEFAULTS[]      PROGMEM=
{
	"false",
	"241",
	"mqtt.thingspeak.com",
	"user",
	"HM4A48J6A926FL81",
	"1883"
};

class EspSett_Mq:public ESPExtraSettingsBox {
public:
	EspSett_Mq();
	virtual ~EspSett_Mq();

	virtual uint8_t getId(){
		return ExtraBox_mqtt;
	}

	virtual String getName() override{
		return FPSTR(MQTT_SETTINGS_BOX_NAME);
	}

	virtual const char* const* getDefaults() override{
		return MQTT_SETT_BOX_DEFAULTS;
	}
	virtual const char* const* getKeys() override{
		return MQTT_SETT_BOX_KEYS_NAME;
	}
	virtual String getDescription() override{
		return FPSTR(MQTT_SETT_BOX_DESCR);
	}
	String getKind() override{
		return FPSTR(SETTINGS_KIND_publish);
	}
};

#endif /* LIBRARIES_ESPSETTINGSBOX_EXTRABOXES_ESPSETT_MQ_H_ */
