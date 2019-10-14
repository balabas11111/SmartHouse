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
#include <DeviceUtils.h>
#include <DeviceConstants.h>
#include <EntityDescriptor.h>
#include <JsonObjectUtil.h>
#include <functional>

const char MESSAGE[] PROGMEM ="Message";

const char NOT_ALLOWED[] PROGMEM ="Not allowed";
const char NOT_IMPLEMENTED[] PROGMEM ="Not implemented";
const char NOT_FOUND[] PROGMEM ="Not found";
const char BAD_METHOD[] PROGMEM ="Bad method";

class Entity {
public:
	Entity(const char* group, const char* name, char* descr,
			std::function<void(void)> onSetChangedEventFunction = nullptr,
			bool applicationDispatcher = true,
			bool hasGet = true, bool hasPost = false,
			bool canLoad = true, bool canSave = true);
	virtual ~Entity() {
	}

	virtual bool preValidate(){return true;}
	void preInitialize(int id, std::function<void()> eventProcessFunction =
			nullptr,
			std::function<void(const char*, const char*, const char*, JsonVariant)> putToBuffer = nullptr);

	virtual bool validate(){return true;}
	virtual void init() {};

	bool isSaveRequired();
	void setSaveRequired(bool saveRequired);

	const char* getName();
	const char* getGroup();
	char* getDescr();

	bool hasGetMethod();bool hasPostMethod();

	bool isApplicationDispatcher();
	void setApplicationDispatcher(bool applicationDispatcher);

	bool canLoadState();bool canSaveState();

	void print();

	bool isTarget(const char* group, const char* name);

	void executeGet(JsonObject& params, JsonObject& response);
	void executePost(JsonObject& params, JsonObject& response);

	void appendSwg(JsonObject& swgJson);

	void setJsonToEntity(JsonObject& jsonFromFile);
	void getJsonToSave(JsonObject& jsonToFile);

	virtual void doGet(JsonObject& params, JsonObject& response) {
		UNUSED(params);
		UNUSED(response);
	};
	virtual void doAppendFieldsSwg(JsonObject& fieldsJson){
		UNUSED(fieldsJson);
	}
	virtual void doPost(JsonObject& params, JsonObject& response) {
		UNUSED(params);
		UNUSED(response);
	};

	virtual void doLoad(JsonObject& jsonFromFile) {
		UNUSED(jsonFromFile);
	};
	virtual void doSave(JsonObject& jsonToFile) {
		UNUSED(jsonToFile);
	};

	void dispatchEventProcessFunctionIfChanged();

	bool isMarkedAsChanged();

	void markEntityAsChangedIfTrue(bool value, bool forceChanged = false);
	void markEntityAsUnChanged();

	void markEntityAsSaveRequiredIfTrue(bool value);

	bool hasTopic(){
		return this->hasTopicConnection;
	}

protected:
	bool forceChanged;
	bool changed;
	bool saveRequired;

	bool hasGet;bool hasPost;bool applicationDispatcher;
	bool hasTopicConnection = true;
	bool canLoad;bool canSave;

	int id;
	const char* group;
	const char* name;
	char* descr;

	std::function<void(void)> eventProcessFunction;
	std::function<void(void)> onSetChangedEventFunction;

	std::function<void(const char*, const char*, const char*, JsonVariant)> funcPutToChangedBuffer;


	void appendEnabledValue(JsonObject& fieldJson, const char* value);

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

	void putToBuffer(const char* key, JsonVariant value);
private:
	void setChanged(bool changed, bool forceChanged = false);

};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_ENTITY_H_ */
