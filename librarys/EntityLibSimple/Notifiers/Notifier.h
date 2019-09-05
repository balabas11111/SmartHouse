/*
 * Notifier.h
 *
 *  Created on: 5 сент. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_NOTIFIER_H_
#define LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_NOTIFIER_H_

#include <Arduino.h>
#include <ArduinoJson.h>

#include <JsonObjectUtil.h>

class Notifier {
public:
	Notifier(const char* name);
	virtual ~Notifier(){}

	virtual void notify(JsonObject& json){
		Serial.print(FPSTR("Notify "));
		Serial.println(this->name);

		JsonObjectUtil::print(json);
	}
private:
	const char* name;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_NOTIFIER_H_ */
