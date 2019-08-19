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
#include <JsonObjectUtil.h>
#include <functional>

const char MESSAGE[] PROGMEM ="Message";
const char NOT_ALLOWED[] PROGMEM ="Not allowed";
const char NOT_IMPLEMENTED[] PROGMEM ="Not implemented";
const char NOT_FOUND[] PROGMEM ="Not found";
const char BAD_METHOD[] PROGMEM ="Bad method";

class Entity {
public:
	Entity(const char* group, const char* name, const char* descr, const bool hasGet = true, const bool hasPost = false, const bool dispatcher = false);
	virtual ~Entity() {};

	void initialize(int id, std::function<void(int)> eventProcessFunction = nullptr);

	virtual void init(){};

	void setId(int id);
	const char* getName();
	const char* getGroup();
	char* getDescr();

	const bool hasGetMethod();
	const bool hasPostMethod();

	bool isTarget(const char* group, const char* name);

	void executeGet(JsonObject& params, JsonObject& response);
	void executePost(JsonObject& params, JsonObject& response);

protected:
	bool changed = false;
	const bool hasGet;
	const bool hasPost;
	const bool dispatcher;

	int id;
	const char* group;
	const char* name;
	char* descr;

	std::function<void(int)> eventProcessFunction;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_ENTITY_H_ */
