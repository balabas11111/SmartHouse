/*
 * Notifier.h
 *
 *  Created on: 5 ����. 2019 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_NOTIFIER_H_
#define LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_NOTIFIER_H_

#include <Arduino.h>
#include <ArduinoJson.h>

#include <utils/DeviceUtils.h>
#include <utils/JsonObjectUtil.h>

#include <Notifiers/NotificationTarget.h>
#include <Notifiers/DataSelector.h>

#include <EntityManager.h>
#include <EntityJsonRequestResponse.h>

#define SENT_TO_DEFAULT_TARGET "toTarget SERIAL"
#define SENT_TO_PREDEFINED_TARGET "toTarget PREDEFINED"
#define SENT_TO_TARGET "toTarget CUSTOM"

class Notifier {
public:
	Notifier(const char* name, NotificationTarget* target = nullptr, DataSelector* selector = nullptr){
		this->name = name;
		this->target = target;
		this->selector = selector;
	}
	virtual ~Notifier(){}

	virtual void notify(char* group = nullptr, char* name = nullptr, char* param = nullptr, NotificationTarget* notifTarget = nullptr){
		UNUSED(param);

		if(!initialized()){
			Serial.print(FPSTR("Not initialized "));
			Serial.println(this->name);
			return;
		}

		startNotification();

		Serial.println(FPSTR("-----------------------------------------------"));
		Serial.print(FPSTR("Notify "));
		Serial.println(this->name);

		DeviceUtils::printHeap();

		EntityJsonRequestResponse* data = EntityJsonRequestResponse::build();
		if(group!=nullptr){
			data->addRequestParam((char*)DEVICE_FIELD_GROUP, group);
		}
		if(name!=nullptr){
			data->addRequestParam((char*)DEVICE_FIELD_ENTITY_NAME, name);
		}
		if(param!=nullptr){
			data->addRequestParam((char*)PARAM, param);
		}

		selector->selectData(data);
		toTarget(data, notifTarget);

		delete data;
		DeviceUtils::printHeap();
		finishNotification();
	}

	bool initialized(){
		bool result = this->selector != nullptr;
		if(!result){
			Serial.println(FPSTR("No DataSelector was set"));
		}
		return result;
	}

	virtual void toTarget(EntityJsonRequestResponse* data, NotificationTarget* notifTarget = nullptr){

		if(notifTarget!=nullptr){
			Serial.println(SENT_TO_PREDEFINED_TARGET);
			notifTarget->toTarget(data);
			return;
		}

		if(this->target==nullptr){
			JsonObjectUtil::printWithPreffix(SENT_TO_DEFAULT_TARGET,data->getResponse());
		}else{
			Serial.println(SENT_TO_TARGET);
			this->target->toTarget(data);
		}
	}

	virtual void finishNotification(){
		unsigned long totalTime = millis()-start;

		start = 0;

		Serial.print(FPSTR("Notified  time ="));
		Serial.println(totalTime);
		Serial.println(FPSTR("============================================="));

	}

	virtual void startNotification(){
		start = millis();
	}

	virtual DataSelector* getSelector(){
		return selector;
	}

	virtual void setTarget(NotificationTarget* target){
		this->target = target;
	}
protected:
	DataSelector* selector;
	NotificationTarget* target;
private:
	const char* name;
	unsigned long start=0;

};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_NOTIFIER_H_ */
