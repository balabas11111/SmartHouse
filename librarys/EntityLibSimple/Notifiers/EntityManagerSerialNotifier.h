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

		Serial.println(FPSTR("update EntityManagerSerialNotifier"));
		ObjectUtils::printHeap();
		EntityJsonRequestResponse* json = new EntityJsonRequestResponse();

		this->manager->executeHttpMethod(json->getRequest(), json->getResponse(), REQUEST_GET);
		JsonObjectUtil::print(json->getResponse());

		delete json;

		ObjectUtils::printHeap();

		Serial.println(FPSTR("DONE update EntityManagerSerialNotifier"));
	}


private:
	EntityManager* manager;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_ENTITYMANAGERSERIALNOTIFIER_H_ */
