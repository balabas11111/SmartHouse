/*
 * EntityApplication.h
 *
 *  Created on: 30 рту. 2019 у.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_ENTITYAPPLICATION_H_
#define LIBRARIES_ENTITYLIBSIMPLE_ENTITYAPPLICATION_H_

#include <Arduino.h>

#include <WiFi/WiFiSettingsBox.h>

#include <WiFi/WiFiManager.h>
#include <WiFi/WiFiServerManager.h>
#include <server/SecurityManager.h>

#include <Entity.h>
#include <EntityManager.h>
#include <EntityUpdate.h>
#include <EntityUpdateManager.h>

#include <ObjectUtils.h>
#include <FileUtils.h>
#include <I2C_utils.h>
#include <functional>

#include <Ticker.h>
#include <Notifiers/Notifier.h>
#include <Notifiers/DataSelector.h>
#include <Notifiers/DataSelectorEntityManager.h>

#include <ApplicationContext.h>

class EntityApplication : public ApplicationContext {
public:
	EntityApplication(const char* firmWare, Entity* entities[], int entityCount,
			EntityUpdate* entityUpdate[], int entityUpdateCount,
			WiFiSettingsBox* conf = nullptr,
			std::function<void(void)> onWiFiConnected = nullptr,
			std::function<void(void)> onWiFiDisConnected = nullptr);
	virtual ~EntityApplication() {
	}

	void initWithWiFi(bool deleteFs = false, bool initI2C = false, uint8_t clockPin = SCL, uint8_t dataPin = SDA);
	void initWithoutWiFi(bool deleteFs = false, bool initI2C = false, uint8_t clockPin = SCL, uint8_t dataPin = SDA);

	void init(bool initSerial = true, bool initWiFi = false, bool initServer = false, bool initFs = true,
			bool deleteFs = false, bool initI2C = false, uint8_t clockPin = SCL, uint8_t dataPin = SDA);

	void setOnEntitiesChanged(std::function<void(void)> onEntitiesChanged);
	void loop();

	void startWiFi();
	void startServer();

	WiFiSettingsBox* getConf();
	EntityManager* getEntityManager();
	EntityUpdateManager* getEntityUpdateManager();
	WiFiManager* getWiFiManager();
	WiFiServerManager* getWifiServerManager();
	SecurityManager* getSecurityManager();

	DataSelector* getDataSelector();
	Notifier* getDefaultNotifier();

	void triggerRegisterOnServer(bool trigger = true);

	void registerTicker(void (*callback)(void));
	void registerTicker(uint32_t milliseconds, void (*callback)(void));

	void updateEntities(bool force = false);

	void notify(char* group = nullptr, char* name = nullptr, char* param = nullptr, NotificationTarget* notifTarget = nullptr);

private:

	WiFiSettingsBox* conf;
	WiFiManager* wifiManager;
	WiFiServerManager* wifiServerManager;

	EntityManager* entityManager;
	EntityUpdateManager* entityUpdateManager;

	SecurityManager* securityManager = nullptr;

	DataSelector* defaultDataSelector = nullptr;
	Notifier* defaultNotifier = nullptr;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_ENTITYAPPLICATION_H_ */
