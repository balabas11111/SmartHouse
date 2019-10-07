/*
 * ApplicationContext.h
 *
 *  Created on: 21 сент. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_APPLICATIONCONTEXT_H_
#define LIBRARIES_ENTITYLIBSIMPLE_APPLICATIONCONTEXT_H_

//#include <EntityManager.h>
#include <EntityUpdateManager.h>
#include <WiFi/WiFiSettingsBox.h>
#include <WiFi/WiFiManager.h>
#include <WiFi/WiFiServerManager.h>
#include <Notifiers/Notifier.h>
#include <Notifiers/NotificationTarget.h>
#include <serve/ServerConnectionManager.h>

class ApplicationContext {
public:
	ApplicationContext() {
	}
	virtual ~ApplicationContext() {
	}

	//virtual EntityManager* getEntityManager() = 0;
	virtual WiFiManager* getWiFiManager() = 0;

	virtual WiFiSettingsBox* getConf() = 0;
	virtual ServerConnectionManager* getServerConnectionManager() = 0;

	virtual void notify(char* group = nullptr, char* name = nullptr, char* param = nullptr, NotificationTarget* notifTarget = nullptr) = 0;


	virtual Notifier* getDefaultNotifier() =0;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_APPLICATIONCONTEXT_H_ */
