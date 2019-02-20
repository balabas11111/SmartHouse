/*
 * BME280model.h
 *
 *  Created on: Feb 20, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_ENTITY_MODEL_SENSORS_BME280MODEL_H_
#define LIBRARIES_DEVICELIB_ENTITY_MODEL_SENSORS_BME280MODEL_H_

#include <Arduino.h>
#include <entity/EntityConsts.h>
#include <entity/model/base/Entity.h>

const char BME280_NAME[]    PROGMEM ="BME280";
const char BME280_DESCR[]  PROGMEM  ="Temp/Humidity/pressure";

const char* const BME280_FIELDS[] PROGMEM                 = { JSONKEY_name, JSONKEY_descr, JSONKEY_temp, JSONKEY_humidity, JSONKEY_pressure };
const char* const BME280_FIELDS_VAR[] PROGMEM             = { JSONKEY_descr,JSONKEY_temp,  JSONKEY_humidity, JSONKEY_pressure };
const char* const BME280_FIELDS_TVAR[] PROGMEM            = { JSONKEY_temp, JSONKEY_humidity, JSONKEY_pressure };
const char* const BME280_FIELDS_PERSIST[] PROGMEM         = { JSONKEY_descr };
const char* const BME280_FIELDS_DEFAULT_KEYS[] PROGMEM    = { JSONKEY_name, JSONKEY_descr };
const char* const BME280_FIELDS_DEFAULT_VAlUES[] PROGMEM  = { BME280_NAME, BME280_DESCR  };


class BME280model:public Entity {
public:
	BME280model():
		Entity(BME280_NAME, JSON_DAO_sensors,
				BME280_FIELDS, BME280_FIELDS_VAR, BME280_FIELDS_TVAR, BME280_FIELDS_PERSIST,
				BME280_FIELDS_DEFAULT_KEYS, BME280_FIELDS_DEFAULT_VAlUES,
				5, 4, 3, 1, 2,ENTITY_CLASS_object){
	}
	virtual ~BME280model(){};
};

#endif /* LIBRARIES_DEVICELIB_ENTITY_MODEL_SENSORS_BME280MODEL_H_ */
