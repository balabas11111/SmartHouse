/*
 * EntityManagerSerialNotifier.h
 *
 *  Created on: 5 сент. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_ENTITYMANAGERSERIALNOTIFIER_H_
#define LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_ENTITYMANAGERSERIALNOTIFIER_H_

#include <Arduino.h>
#include <EntityUpdate.h>
#include <EntityManager.h>
#include <JsonObjectUtil.h>

#include <ObjectUtils.h>

#include <EntityJsonRequestResponse.h>

class EntityManagerSerialNotifier: public EntityUpdate {
public:
	EntityManagerSerialNotifier();
	virtual ~EntityManagerSerialNotifier();

	void begin(EntityManager* manager){
		this->manager = manager;
	}

	virtual void doUpdate() override{
		if(this->manager == nullptr){
			return;
		}


		unsigned long start= millis();
		Serial.println(FPSTR("-----------------------------------------------"));
		Serial.println(FPSTR("update EntityManagerSerialNotifier"));
		ObjectUtils::printHeap();
		EntityJsonRequestResponse* json = new EntityJsonRequestResponse();
		/*json->print();

		Serial.println(FPSTR("EntityManagerSerialNotifier root = "));
		JsonObjectUtil::print(json->getRoot());

		Serial.println(FPSTR("request = "));
		JsonObjectUtil::print(json->getRequest());

		Serial.println(FPSTR("response = "));
		JsonObjectUtil::print(json->getResponse());
*/
		this->manager->get(json);
		Serial.print(FPSTR("RESULT = "));
		JsonObjectUtil::print(json->getResponse());

		delete json;

		ObjectUtils::printHeap();

		unsigned long totalTime = millis()-start;

		Serial.print(FPSTR("DONE EntityManagerSerialNotifier time ="));
		Serial.println(totalTime);
		Serial.println(FPSTR("============================================="));
	}


private:
	EntityManager* manager;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_ENTITYMANAGERSERIALNOTIFIER_H_ */
