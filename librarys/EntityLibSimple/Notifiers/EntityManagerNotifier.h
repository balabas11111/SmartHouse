/*
 * EntityManagerSerialNotifier.h
 *
 *  Created on: 5 сент. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_ENTITYMANAGERNOTIFIER_H_
#define LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_ENTITYMANAGERNOTIFIER_H_

#include <Arduino.h>
#include <EntityManager.h>
#include <JsonObjectUtil.h>

#include <ObjectUtils.h>
#include <Notifiers/Notifier.h>

#include <EntityJsonRequestResponse.h>

class EntityManagerNotifier: public Notifier  {
public:
	EntityManagerNotifier(NotificationTarget* target = nullptr):Notifier("EntityManagerNotifier", target){

	}
	virtual ~EntityManagerNotifier(){};

	virtual void notify(char* group = nullptr, char* name = nullptr, NotificationTarget* notifTarget = nullptr){
		if(this->manager == nullptr ){
			return;
		}

		unsigned long start = startNotification();
		EntityJsonRequestResponse* json = this->manager->createEntityJsonRequestResponse();

		json->addRequestParam((char*)GROUP, group);
		json->addRequestParam((char*)NAME, name);

		this->manager->get(json);

		toTarget(json->getResponse(), notifTarget);
		this->manager->deleteEntityJsonRequestResponse(json);

		finishNotification(start);
	}

};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_ENTITYMANAGERNOTIFIER_H_ */
