/*
 * Entity.h
 *
 *  Created on: Aug 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_ENTITY_H_
#define LIBRARIES_ENTITYLIBSIMPLE_ENTITY_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ObjectUtils.h>
#include <JsonObjectUtil.h>
#include <functional>

#define GROUP_SENSORS "sensors"
#define GROUP_SETTINGS "settings"
#define GROUP_SERVICES "services"

const char MESSAGE[] PROGMEM ="Message";

const char NOT_ALLOWED[] PROGMEM ="Not allowed";
const char NOT_IMPLEMENTED[] PROGMEM ="Not implemented";
const char NOT_FOUND[] PROGMEM ="Not found";
const char BAD_METHOD[] PROGMEM ="Bad method";

class Entity {
public:
	Entity(const char* group, const char* name, char* descr,
			const char* descrField = "d",
			bool hasGet = true, bool hasPost = false, bool dispatcher = false,
			bool canLoad = true, bool canSave = true);
	virtual ~Entity() {
	}

	void preInitialize(int id, std::function<void(int)> eventProcessFunction =
			nullptr);

	virtual void init() {};

	bool isChanged();
	void setChanged(bool changed);

	const char* getName();
	const char* getGroup();
	char* getDescr();

	bool hasGetMethod();bool hasPostMethod();

	bool canDispatchChangeEvent();bool canLoadState();bool canSaveState();

	void print();

	bool isTarget(const char* group, const char* name);

	void executeGet(JsonObject& params, JsonObject& response);
	void executePost(JsonObject& params, JsonObject& response);

	void executeLoad(JsonObject& jsonFromFile);
	void executeSave(JsonObject& jsonToFile);

	virtual void doGet(JsonObject& params, JsonObject& response) = 0;
	virtual void doPost(JsonObject& params, JsonObject& response) = 0;

	virtual void doLoad(JsonObject& jsonFromFile) = 0;
	virtual void doSave(JsonObject& jsonToFile) = 0;
protected:
	bool changed;

	bool hasGet;bool hasPost;bool dispatcher;

	bool canLoad;bool canSave;

	int id;
	const char* group;
	const char* name;
	char* descr;

	const char* descrField;

	std::function<void(int)> eventProcessFunction;

	void dispatchChangeEvent(bool clause);

	template<typename T>
	T getJsonField(JsonObject& json, const char* key) {
		return JsonObjectUtil::getField<T>(json, key);
	}

	template<typename T>
	bool setJsonField(JsonObject& json, const char* key, T value) {
		return JsonObjectUtil::setField(json, key, value);
	}

	bool getKeyValueIfExistsAndNotEquals(JsonObject& json, const char* key,	char** val);
	bool getKeyValueIfExistsAndNotEquals(JsonObject& json, const char* key, int* val);
	bool getKeyValueIfExistsAndNotEquals(JsonObject& json, const char* key, uint8_t* val);
	bool getKeyValueIfExistsAndNotEquals(JsonObject& json, const char* key, uint16_t* val);
	bool getKeyValueIfExistsAndNotEquals(JsonObject& json, const char* key, bool* val);
	bool getKeyValueIfExistsAndNotEquals(JsonObject& json, const char* key, IPAddress* val);

	bool setKeyValueIfNotExistOrNotEqual(JsonObject& json, const char* key, JsonVariant val);

	bool isKeyExistsInJsonAndNotEqValue(JsonObject& json, const char* key, JsonVariant val) {
		return JsonObjectUtil::getObjectFieldExistsAndNotEquals(json, key, val);
	}

	bool isKeyNotExistsInJsonOrNotEqValue(JsonObject& json, const char* key,
			JsonVariant val) {
		return JsonObjectUtil::getObjectFieldNotExistsOrNotEquals(json, key,
				val);
	}

};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_ENTITY_H_ */
