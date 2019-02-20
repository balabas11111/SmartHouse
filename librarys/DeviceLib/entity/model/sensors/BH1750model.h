/*
 * BH1750model.h
 *
 *  Created on: Feb 20, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_ENTITY_MODEL_SENSORS_BH1750MODEL_H_
#define LIBRARIES_DEVICELIB_ENTITY_MODEL_SENSORS_BH1750MODEL_H_

#include <Arduino.h>
#include <entity/EntityConsts.h>
#include <entity/model/base/Entity.h>

const char BH1750_NAME[]    PROGMEM ="BH1750";
const char BH1750_DESCR[]  PROGMEM  ="Temp/Humidity/pressure";

const char* const BH1750_FIELDS[] PROGMEM                 = { JSONKEY_name, JSONKEY_descr, JSONKEY_light };
const char* const BH1750_FIELDS_VAR[] PROGMEM             = { JSONKEY_descr,JSONKEY_light };
const char* const BH1750_FIELDS_TVAR[] PROGMEM            = { JSONKEY_light };
const char* const BH1750_FIELDS_PERSIST[] PROGMEM         = { JSONKEY_descr };
const char* const BH1750_FIELDS_DEFAULT_KEYS[] PROGMEM    = { JSONKEY_name, JSONKEY_descr };
const char* const BH1750_FIELDS_DEFAULT_VAlUES[] PROGMEM  = { BH1750_NAME, BH1750_DESCR  };

class BH1750model:public Entity {
public:
	BH1750model():
		Entity(BH1750_NAME, JSON_DAO_sensors,
						BH1750_FIELDS, BH1750_FIELDS_VAR, BH1750_FIELDS_TVAR, BH1750_FIELDS_PERSIST,
						BH1750_FIELDS_DEFAULT_KEYS, BH1750_FIELDS_DEFAULT_VAlUES,
						3, 2, 1, 1, 2,ENTITY_CLASS_object){
			}
	virtual ~BH1750model(){};
};

#endif /* LIBRARIES_DEVICELIB_ENTITY_MODEL_SENSORS_BH1750MODEL_H_ */
