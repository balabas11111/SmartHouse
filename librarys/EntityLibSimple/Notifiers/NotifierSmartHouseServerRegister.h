/*
 * SmartHouseServerNotifier.h
 *
 *  Created on: Sep 20, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_NOTIFIERSMARTHOUSESERVERREGISTER_H_
#define LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_NOTIFIERSMARTHOUSESERVERREGISTER_H_

#include <Arduino.h>
#include <EntityManager.h>
#include <JsonObjectUtil.h>

#include <ObjectUtils.h>
#include <HttpUtils.h>
#include <Notifiers/Notifier.h>

#include <EntityJsonRequestResponse.h>

class NotifierSmartHouseServerRegister: public Notifier {
public:
	NotifierSmartHouseServerRegister(NotificationTarget* target = nullptr, EntityManager* manager = nullptr):Notifier("NotifierSmartHouseServer", target, manager){

	}
	virtual ~NotifierSmartHouseServerRegister(){};

	virtual void notify() override{
		Notifier::notify();

		if(!initialized()){
			return;
		}

		notifyServer();
	}
protected:
	void notifyServer(){

		unsigned long start = startNotification();
		EntityJsonRequestResponse* req = this->manager->createEntityJsonRequestResponse();

		manager->getConf()->createRegisterRequest(req->getRequest());

		//toTarget(json->getRequest(), notifTarget);
		HttpUtils::executePostRequest(req);

		this->manager->deleteEntityJsonRequestResponse(req);

		finishNotification(start);
	}
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_NOTIFIERSMARTHOUSESERVERREGISTER_H_ */
