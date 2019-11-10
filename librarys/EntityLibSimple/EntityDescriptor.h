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
#include "utils/JsonObjectUtil.h"
#include "Emoji.h"

#define EDC_ENTITY_FIELDS          "f"
#define EDC_FIELD_ENABLED_VALUES   "val"

#define EDC_FIELD_ID           "i"
#define EDC_FIELD_NAME         "n"
#define EDC_FIELD_DESCRIPTION  "d"
#define EDC_FIELD_ACTION_DESCR "ad"
#define EDC_FIELD_DESCR_FIELD  "df"
#define EDC_FIELD_EMOJI        "img"

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

#define EDC_FIELD_BOOLEAN_VALUE_ON  "true"
#define EDC_FIELD_BOOLEAN_VALUE_OFF "false"

#define EDC_DESCR_ON       "Включено"
#define EDC_DESCR_OFF      "Отключено"
#define EDC_DESCR_RESTART  "Перезапускаем"

#define EDC_DESCR_ACTION_ON       "Включить"
#define EDC_DESCR_ACTION_OFF      "Отключить"
#define EDC_DESCR_ACTION_RESTART  "Перезапустить"

//entities descriptions
#define EDC_DESCR_NAMING        "Название"
#define EDC_DESCR_DESCRIPTION   "Описание"
#define EDC_DESCR_SETTINGS      "Настройки"
#define EDC_DESCR_SENSOR        "Датчик"
#define EDC_DESCR_SERVICE       "Сервис"
#define EDC_DESCR_COUNT         "Кол-во датчиков"
#define EDC_DESCR_TEMPERATURE   "Температура"
#define EDC_DESCR_HUMIDITY      "Влажность"
#define EDC_DESCR_ATMPRESSURE   "Атм. давление"
#define EDC_DESCR_LIGHT_LEVEL   "Освещенность"
#define EDC_DESCR_STATE         "Состояние"

#define EDC_DESCR_MOVEMENT      "Движение"
#define EDC_DESCR_MOVEMENT_ON   "ДВИЖЕНИЕ"
#define EDC_DESCR_MOVEMENT_OFF  "Нет движения"

class EntityDescriptor {
public:
	static void appendSwgEntityField(JsonObject& swgJson, const char* key,
			const char* value) {
		swgJson[key] = value;
	}

	static void appendSwgField(JsonObject& swgJson, const char* name,
			const char* description = nullptr, const char* fieldClass =
					EDC_CLASS_VIEW_LABEL, const char* fieldViewClass =
					EDC_CLASS_VIEW_LABEL, bool readOnly = true,
			const char* emoji = nullptr, const char* descriptionField = nullptr,
			int id = -1) {

		JsonObject& fieldsJson =
				JsonObjectUtil::getObjectChildOrCreateNewNoKeyDup(swgJson,
						EDC_ENTITY_FIELDS);
		JsonObject& field = JsonObjectUtil::getObjectChildOrCreateNewNoKeyDup(
				fieldsJson, name);

		if (description != nullptr) {
			field[EDC_FIELD_DESCRIPTION] = description;
		}
		field[EDC_CLASS] = fieldClass;
		field[EDC_CLASS_VIEW] = fieldViewClass;
		if (descriptionField != nullptr) {
			field[EDC_FIELD_DESCR_FIELD] = descriptionField;
		}
		if (readOnly) {
			field[EDC_READ_ONLY] = 1;
		}
		if (emoji != nullptr) {
			field[EDC_FIELD_EMOJI] = emoji;
		}
		if (id > -1) {
			field[EDC_FIELD_ID] = id;
		}
	}

	static void appendSwgEntityParams(JsonObject& swgJson, const char* emoji,
			const char* descriptionField = EDC_FIELD_DESCRIPTION) {
		appendSwgEntityField(swgJson, EDC_FIELD_EMOJI, emoji);

		if (descriptionField != nullptr) {
			appendSwgEntityField(swgJson, EDC_FIELD_DESCR_FIELD,
					descriptionField);
		}
	}

	static void appendSwgFieldBooleanCommand(JsonObject& swgJson,
			const char* name, const char* description = nullptr,
			bool readOnly = false, const char* emoji = nullptr,
			const char* onKey = EDC_FIELD_BOOLEAN_VALUE_ON,
			const char* onDescription = EDC_DESCR_ON,
			const char* onActionDescription = EDC_DESCR_ACTION_ON,
			const char* onEmoji = EMOJI_DIM_BUTTON) {

		appendSwgField(swgJson, name, description, EDC_CLASS_BOOLEAN,
				(readOnly ? EDC_CLASS_VIEW_LABEL : EDC_CLASS_VIEW_BUTTON_COMMAND),
				readOnly, emoji);

		appendEnabledValue(swgJson, name, onKey, onDescription,
				onActionDescription, onEmoji);
	}

	static void appendSwgFieldBooleanOnOff(JsonObject& swgJson,
			const char* name, const char* description = nullptr,
			const char* emoji = nullptr,
			bool readOnly = false, const char* onKey =
					EDC_FIELD_BOOLEAN_VALUE_ON, const char* offKey =
					EDC_FIELD_BOOLEAN_VALUE_OFF, const char* onDescription =
					EDC_DESCR_ON, const char* offDescription = EDC_DESCR_OFF,
			const char* onActionDescription = EDC_DESCR_ACTION_ON,
			const char* offActionDescription = EDC_DESCR_ACTION_OFF,
			const char* onEmoji = EMOJI_DIM_BUTTON, const char* offEmoji =
					EMOJI_RADIO_BUTTON) {

		appendSwgFieldBooleanOnOffWithClass(swgJson, name, description, emoji, readOnly,
				(readOnly ? EDC_CLASS_VIEW_LABEL : EDC_CLASS_VIEW_BUTTON_BOOLEAN),
				onKey, offKey, onDescription, offDescription, onActionDescription, offActionDescription, onEmoji, offEmoji);

	}

	static void appendSwgFieldBooleanOnOffWithClass(JsonObject& swgJson,
				const char* name, const char* description = nullptr,
				const char* emoji = nullptr,
				bool readOnly = false, const char* viewClass = EDC_CLASS_VIEW_BUTTON, const char* onKey =
						EDC_FIELD_BOOLEAN_VALUE_ON, const char* offKey =
						EDC_FIELD_BOOLEAN_VALUE_OFF, const char* onDescription =
						EDC_DESCR_ON, const char* offDescription = EDC_DESCR_OFF,
				const char* onActionDescription = EDC_DESCR_ACTION_ON,
				const char* offActionDescription = EDC_DESCR_ACTION_OFF,
				const char* onEmoji = EMOJI_DIM_BUTTON, const char* offEmoji =
						EMOJI_RADIO_BUTTON) {

			appendSwgField(swgJson, name, description, EDC_CLASS_BOOLEAN,
					viewClass,
					readOnly, emoji);

			appendEnabledValue(swgJson, name, onKey, onDescription,
					onActionDescription, onEmoji);
			appendEnabledValue(swgJson, name, offKey, offDescription,
					offActionDescription, offEmoji);
		}

	static void appendSwgFieldInteger(JsonObject& swgJson, const char* name,
			const char* description = nullptr, const char* emoji = nullptr,
			bool readOnly = true, const char* descriptionField = nullptr) {

		appendSwgField(swgJson, name, description, EDC_CLASS_INTEGER,
				(readOnly ? EDC_CLASS_VIEW_LABEL : EDC_CLASS_VIEW_INPUT),
				readOnly, emoji, descriptionField);
	}

	static void appendSwgFieldFloat(JsonObject& swgJson, const char* name,
			const char* description = nullptr, const char* emoji = nullptr,
			bool readOnly = true, const char* descriptionField = nullptr) {

		appendSwgField(swgJson, name, description, EDC_CLASS_FLOAT,
				(readOnly ? EDC_CLASS_VIEW_LABEL : EDC_CLASS_VIEW_INPUT),
				readOnly, emoji, descriptionField);
	}

	static void appendSwgFieldFloatNameDescrFieldOnly(JsonObject& swgJson, const char* name,
				const char* descriptionField = nullptr) {

		appendSwgField(swgJson, name, nullptr, EDC_CLASS_FLOAT,
				EDC_CLASS_VIEW_LABEL,
				true, nullptr, descriptionField);
	}


	static void appendSwgFieldString(JsonObject& swgJson, const char* name,
			const char* description = nullptr, const char* emoji = nullptr,
			bool readOnly = false, const char* descriptionField = nullptr) {

		appendSwgField(swgJson, name, description, EDC_CLASS_STRING,
				(readOnly ? EDC_CLASS_VIEW_LABEL : EDC_CLASS_VIEW_INPUT),
				readOnly, emoji, descriptionField);
	}

	static void appendEnabledValue(JsonObject& swgJson, const char* name,
			const char* key, const char* description,
			const char* actionDescription, const char* emoji = nullptr) {

		JsonObject& enabledValue =
				JsonObjectUtil::getObjectChildOrCreateNewNoKeyDup(
						JsonObjectUtil::getObjectChildOrCreateNewNoKeyDup(
								swgJson, EDC_ENTITY_FIELDS, name),
						EDC_FIELD_ENABLED_VALUES, key);

		if (description != nullptr) {
			enabledValue[EDC_FIELD_DESCRIPTION] = description;
		}
		enabledValue[EDC_FIELD_ACTION_DESCR] = actionDescription;
		if (emoji != nullptr) {
			enabledValue[EDC_FIELD_EMOJI] = emoji;
		}
	}
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_ENTITYDESCRIPTOR_H_ */
