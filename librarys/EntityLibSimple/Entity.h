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

const char DESCR[] PROGMEM ="descr";

const char MESSAGE[] PROGMEM ="Message";

const char NOT_ALLOWED[] PROGMEM ="Not allowed";
const char NOT_IMPLEMENTED[] PROGMEM ="Not implemented";
const char NOT_FOUND[] PROGMEM ="Not found";
const char BAD_METHOD[] PROGMEM ="Bad method";

class Entity {
public:
	Entity(const char* group, const char* name, char* descr,
			bool hasGet = true, bool hasPost = false, bool dispatcher = false,
			bool canLoad = true, bool canSave = true);
	virtual ~Entity() {};

	void initialize(int id, std::function<void(int)> eventProcessFunction = nullptr);

	virtual void init(){};

	bool isChanged();
	void setChanged(bool changed);

	const char* getName();
	const char* getGroup();
	char* getDescr();

	bool hasGetMethod();
	bool hasPostMethod();

	bool canDispatchChangeEvent();
	bool canLoadState();
	bool canSaveState();

	void print();

	bool isTarget(const char* group, const char* name);

	virtual void executeGet(JsonObject& params, JsonObject& response) = 0;
	virtual void executePost(JsonObject& params, JsonObject& response) = 0;

	virtual void executeLoad(JsonObject& jsonFromFile) = 0;
	virtual void executeSave(JsonObject& jsonToFile) = 0;

protected:
	bool changed;

	bool hasGet;
	bool hasPost;
	bool dispatcher;

	bool canLoad;
	bool canSave;

	int id;
	const char* group;
	const char* name;
	char* descr;

	std::function<void(int)> eventProcessFunction;

	//void dispatchChangeEvent();
	void dispatchChangeEvent(bool clause);

	template<typename T>
	T getJsonField(JsonObject& json, const char* key){
		return JsonObjectUtil::getField<T>(json, key);
	}

	template<typename T>
	bool setJsonField(JsonObject& json, const char* key,T value){
		return JsonObjectUtil::setField(json, key, value);
	}

	bool isKeyExistsInJsonAndNotEqValue(JsonObject& json, const char* key,JsonVariant val){
		return JsonObjectUtil::getObjectFieldExistsAndNotEquals(json, key, val);
	}
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_ENTITY_H_ */
