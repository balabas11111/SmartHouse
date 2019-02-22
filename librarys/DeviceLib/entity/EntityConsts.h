/*
 * EntityConsts.h
 *
 *  Created on: 19 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_ENTITY_ENTITYCONSTS_H_
#define LIBRARIES_DEVICELIB_ENTITY_ENTITYCONSTS_H_

#include <Arduino.h>

const PROGMEM char ENTITY_CLASS_object[] = "object";
const PROGMEM char ENTITY_CLASS_table[]  = "table";
//JsonKeys
const PROGMEM char JSONKEY_model[] = "model";
const PROGMEM char JSONKEY_data[]  = "data";
const PROGMEM char JSONKEY_web[]   = "web";

const PROGMEM char JSONKEY_id[]      = "id";
const PROGMEM char JSONKEY_name[]    = "name";
const PROGMEM char JSONKEY_descr[]   = "descr";
const PROGMEM char JSONKEY_path[]    = "path";
const PROGMEM char JSONKEY_dao[]     = "dao";
const PROGMEM char JSONKEY_class[]   = "class";
const PROGMEM char JSONKEY_open[]    = "open";
//action object and allowed actions
const PROGMEM char JSONKEY_action[]  = "action";

//fields security
const PROGMEM char JSONKEY_security[]   = "security";

const PROGMEM char JSONKEY_cache[]   = "cache";
const PROGMEM char JSONKEY_list[]    = "list";
const PROGMEM char JSONKEY_set[]     = "set";
const PROGMEM char JSONKEY_put[]     = "put";
const PROGMEM char JSONKEY_save[]    = "save";
const PROGMEM char JSONKEY_load[]    = "load";

//descr -default field values
const PROGMEM char JSONKEY_fields[]  = "fields";

//table constants
const PROGMEM char JSONKEY_table[]   = "table";
const PROGMEM char JSONKEY_empty[]   = "empty";
const PROGMEM char JSONKEY_autoinc[] = "autoinc";
const PROGMEM char JSONKEY_dict[]            = "dict";

//default field values and non cached vars
const PROGMEM char JSONKEY_default[]  = "default";
const PROGMEM char JSONKEY_tvar[]     = "tvar";
const PROGMEM char JSONKEY_var[]      = "var";
const PROGMEM char JSONKEY_persist[]  = "persist";

//base field values
const PROGMEM char JSONKEY_temp[]     = "temp";
const PROGMEM char JSONKEY_humidity[] = "humidity";
const PROGMEM char JSONKEY_pressure[] = "pressure";
const PROGMEM char JSONKEY_checked[]  = "checked";
const PROGMEM char JSONKEY_light[]    = "light";

const PROGMEM char* const JSONKEY_ALL_KEYS[]    = {
	ENTITY_CLASS_object,
	ENTITY_CLASS_table,
	JSONKEY_model,
	JSONKEY_data,
	JSONKEY_web,

	JSONKEY_id,
	JSONKEY_name,
	JSONKEY_descr,
	JSONKEY_path,
	JSONKEY_dao,

	JSONKEY_class,
	JSONKEY_open,
	JSONKEY_action,
	JSONKEY_security,
	JSONKEY_cache,

	JSONKEY_list,
	JSONKEY_set,
	JSONKEY_put,
	JSONKEY_save,
	JSONKEY_load,

	JSONKEY_fields,
	JSONKEY_table,
	JSONKEY_empty,
	JSONKEY_autoinc,
	JSONKEY_dict,

	JSONKEY_default,
	JSONKEY_tvar,
	JSONKEY_var,
	JSONKEY_persist,
	JSONKEY_temp,

	JSONKEY_humidity,
	JSONKEY_pressure,
	JSONKEY_checked,
	JSONKEY_light};

const uint8_t KEY_SIZE=34;

//entity is open for change for abstractServices
const PROGMEM char JSON_SERVICE_ThingSpeakService[]    = "ThingSpeakService";
const PROGMEM char JSON_SERVICE_MinMaxALarmService[]   = "MinMaxALarmService";

//base daos
const PROGMEM char JSON_DAO_sensors[]   = "sensors";
//defaults
const char* JSONKEY_null  = (char*)0;
const char* JSONKEY_PERCENT  = "%";

#endif /* LIBRARIES_DEVICELIB_ENTITY_ENTITYCONSTS_H_ */
