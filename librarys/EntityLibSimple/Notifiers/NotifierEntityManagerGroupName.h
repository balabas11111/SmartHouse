/*
 * EntityManagerSerialNotifier.h
 *
 *  Created on: 5 сент. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_NOTIFIERENTITYMANAGERGROUPNAME_H_
#define LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_NOTIFIERENTITYMANAGERGROUPNAME_H_

#include <Arduino.h>
#include <EntityManager.h>
#include <JsonObjectUtil.h>

#include <ObjectUtils.h>
#include <Notifiers/Notifier.h>

#include <EntityJsonRequestResponse.h>

class NotifierEntityManagerGroupName: public Notifier  {
public:
	NotifierEntityManagerGroupName(NotificationTarget* target = nullptr, EntityManager* manager = nullptr):Notifier("EntityManagerNotifier", target, manager){

	}
	virtual ~NotifierEntityManagerGroupName(){};

	virtual void notify(char* group = nullptr, char* name = nullptr, NotificationTarget* notifTarget = nullptr){
		if(!initialized()){
			Serial.println(FPSTR("No entity Manager was set"));
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

#endif /* LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_NOTIFIERENTITYMANAGERGROUPNAME_H_ */
