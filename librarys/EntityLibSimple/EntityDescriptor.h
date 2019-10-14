/*
 * EntityDescriptorConstants.h
 *
 *  Created on: 13 ���. 2019 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_ENTITYDESCRIPTOR_H_
#define LIBRARIES_ENTITYLIBSIMPLE_ENTITYDESCRIPTOR_H_

#include "Arduino.h"
#include "ArduinoJson.h"
#include "JsonObjectUtil.h"

#define EDC_ENTITY_FIELDS          "f"
#define EDC_FIELD_ENABLED_VALUES   "val"

#define EDC_READ_ONLY    "ro"
#define EDC_TIME_TO_LIVE "ttl"

#define EDC_HAS_TOPIC       "hmq"

#define EDC_CLASS "c"
#define EDC_CLASS_BOOLEAN "bool"
#define EDC_CLASS_INTEGER "int"
#define EDC_CLASS_LONG "long"
#define EDC_CLASS_FLOAT "float"
#define EDC_CLASS_STRING "char*"
#define EDC_CLASS_OBJECT "obj"

#define EDC_CLASS_VIEW "cv"

#define EDC_CLASS_VIEW_DEFAULT          "def"
#define EDC_CLASS_VIEW_NONE             "non"
#define EDC_CLASS_VIEW_LIST             "lst"
#define EDC_CLASS_VIEW_TABLE            "tbl"
#define EDC_CLASS_VIEW_MAP              "map"
#define EDC_CLASS_VIEW_LABEL            "lbl"
#define EDC_CLASS_VIEW_INPUT            "inp"
#define EDC_CLASS_VIEW_PASSWORD_INPUT   "pwd"
#define EDC_CLASS_VIEW_IP_INPUT         "ipi"
#define EDC_CLASS_VIEW_COMBOBOX         "cmb"
#define EDC_CLASS_VIEW_DATE_TIME        "dtm"
#define EDC_CLASS_VIEW_OPTIONS          "opt"
#define EDC_CLASS_VIEW_BUTTON           "btn"
#define EDC_CLASS_VIEW_BUTTON_COMMAND   "btc"
#define EDC_CLASS_VIEW_BUTTON_BOOLEAN   "btb"
#define EDC_CLASS_VIEW_BUTTON_OPTIONS   "bto"

#define EDC_FIELD_ID          "i"
#define EDC_FIELD_NAME        "n"
#define EDC_FIELD_DESCRIPTION "d"

#define EDC_FIELD_BOOLEAN_VALUE_ON  "true"
#define EDC_FIELD_BOOLEAN_VALUE_OFF "false"

#define EDC_FIELD_BOOLEAN_VALUE_ON_DESCR       "Включить"
#define EDC_FIELD_BOOLEAN_VALUE_OFF_DESCR      "Отключить"
#define EDC_FIELD_BOOLEAN_VALUE_RESTART_DESCR  "Перезапустить"

class EntityDescriptor{
public:
	static void appendSwgField(JsonObject& fieldsJson, const char* name,
			const char* fieldClass,
			const char* fieldViewClass = EDC_CLASS_VIEW_LABEL, bool readOnly = true, int id =-1) {
		JsonObject& field =
					JsonObjectUtil::getObjectChildOrCreateNewNoKeyDup(fieldsJson, name);
		if(id>-1){
			JsonObjectUtil::setField(field, EDC_FIELD_ID, id);
		}
		JsonObjectUtil::setField(field, EDC_CLASS, fieldClass);
		JsonObjectUtil::setField(field, EDC_CLASS_VIEW, fieldViewClass);
		if (readOnly){
			JsonObjectUtil::setField(field, EDC_READ_ONLY, 1);
		}
	}

	static void appendSwgFieldBooleanCommand(JsonObject& fieldsJson,
				const char* name, bool readOnly = false,
				const char* onKey = EDC_FIELD_BOOLEAN_VALUE_ON,
				const char* onValue = EDC_FIELD_BOOLEAN_VALUE_ON_DESCR) {

		appendSwgField(fieldsJson, name, EDC_CLASS_BOOLEAN,
				(readOnly?EDC_CLASS_VIEW_LABEL:EDC_CLASS_VIEW_BUTTON_COMMAND), readOnly);

		appendEnabledValue(fieldsJson, name, onKey, onValue);
	}

	static void appendSwgFieldBooleanOnOff(JsonObject& fieldsJson,
				const char* name, bool readOnly = false,
				const char* onKey = EDC_FIELD_BOOLEAN_VALUE_ON,
				const char* offKey = EDC_FIELD_BOOLEAN_VALUE_OFF,
				const char* onValue = EDC_FIELD_BOOLEAN_VALUE_ON_DESCR,
				const char* offValue = EDC_FIELD_BOOLEAN_VALUE_OFF_DESCR) {
		appendSwgField(fieldsJson, name, EDC_CLASS_BOOLEAN,
				(readOnly?EDC_CLASS_VIEW_LABEL:EDC_CLASS_VIEW_BUTTON_BOOLEAN), readOnly);

		appendEnabledValue(fieldsJson, name, onKey, onValue);
		appendEnabledValue(fieldsJson, name, offKey, offValue);
	}

	static void appendSwgFieldInteger(JsonObject& fieldsJson,
					const char* name, bool readOnly = true) {
		appendSwgField(fieldsJson, name, EDC_CLASS_INTEGER,
				(readOnly?EDC_CLASS_VIEW_LABEL:EDC_CLASS_VIEW_INPUT), readOnly);
	}

	static void appendSwgFieldFloat(JsonObject& fieldsJson,
			const char* name, bool readOnly = true) {
		appendSwgField(fieldsJson, name, EDC_CLASS_FLOAT,
				(readOnly?EDC_CLASS_VIEW_LABEL:EDC_CLASS_VIEW_INPUT), readOnly);
	}

	static void appendSwgFieldString(JsonObject& fieldsJson,
			const char* name, bool readOnly = false) {
		appendSwgField(fieldsJson, name, EDC_CLASS_STRING,
				(readOnly?EDC_CLASS_VIEW_LABEL:EDC_CLASS_VIEW_INPUT), readOnly);
	}

	static void appendEnabledValue(JsonObject& fieldsJson, const char* name, const char* key, const char* value) {
		JsonObject& enabledValuesJson =
				JsonObjectUtil::getObjectChildOrCreateNewNoKeyDup(fieldsJson, name, EDC_FIELD_ENABLED_VALUES);

		enabledValuesJson[key] = value;
	}
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_ENTITYDESCRIPTOR_H_ */
