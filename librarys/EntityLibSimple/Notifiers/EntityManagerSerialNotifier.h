/*
 * EntityManagerSerialNotifier.h
 *
 *  Created on: 5 сент. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_ENTITYMANAGERSERIALNOTIFIER_H_
#define LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_ENTITYMANAGERSERIALNOTIFIER_H_

#include <Arduino.h>
#include <EntityManager.h>
#include <JsonObjectUtil.h>

#include <ObjectUtils.h>
#include <Notifiers/Notifier.h>

#include <EntityJsonRequestResponse.h>

class EntityManagerSerialNotifier: public Notifier  {
public:
	EntityManagerSerialNotifier(NotificationTarget* target = nullptr):Notifier("EntityManagerSerialNotifier", target){

	}
	virtual ~EntityManagerSerialNotifier();

	void begin(EntityManager* manager){
		this->manager = manager;
	}

	virtual void notify(char* group = nullptr, char* name = nullptr){
		if(this->manager == nullptr){
			return;
		}

		unsigned long start = startNotification();
		EntityJsonRequestResponse* json = this->manager->createEntityJsonRequestResponse();

		json->addRequestParam(GROUP, group);
		json->addRequestParam(NAME, name);

		this->manager->get(json);

		toTarget(json->getResponse());
		this->manager->deleteEntityJsonRequestResponse(json);

		finishNotification(start);
	}

private:
	EntityManager* manager;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_ENTITYMANAGERSERIALNOTIFIER_H_ */
