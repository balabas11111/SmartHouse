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
#include <Notifiers/NotificationTarget.h>

#include <EntityManager.h>

#define SENT_TO_DEFAULT_TARGET "toSerialTarget"
#define SENT_TO_TARGET "toTarget"

class Notifier {
public:
	Notifier(const char* name, NotificationTarget* target = nullptr){
		this->name = name;
		this->target = target;
	}
	virtual ~Notifier(){}

	virtual void begin(EntityManager* manager){
		this->manager = manager;
	}

	virtual void notify(){
		Serial.println(FPSTR("-----------------------------------------------"));
		Serial.print(FPSTR("Notify "));
		Serial.println(this->name);
	}

	virtual void toTarget(JsonObject& json, NotificationTarget* notifTarget = nullptr){
		if(notifTarget!=nullptr){
			JsonObjectUtil::printWithPreffix(SENT_TO_TARGET,json);
			notifTarget->toTarget(json);
			return;
		}

		if(this->target==nullptr){
			JsonObjectUtil::printWithPreffix(SENT_TO_DEFAULT_TARGET,json);
		}else{
			JsonObjectUtil::printWithPreffix(SENT_TO_TARGET,json);
			this->target->toTarget(json);
		}
	}

	virtual void finishNotification(unsigned long startTime){
		unsigned long totalTime = millis()-startTime;

		Serial.print(FPSTR("Notified  time ="));
		Serial.println(totalTime);
		Serial.println(FPSTR("============================================="));

	}

	virtual unsigned long startNotification(){
		Notifier::notify();
		return millis();
	}

	virtual EntityManager* getEntityManager(){
		return manager;
	}
protected:
	EntityManager* manager;
private:
	const char* name;
	NotificationTarget* target;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_NOTIFIER_H_ */
