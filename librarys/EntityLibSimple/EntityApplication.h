/*
 * EntityApplication.h
 *
 *  Created on: 30 рту. 2019 у.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_ENTITYAPPLICATION_H_
#define LIBRARIES_ENTITYLIBSIMPLE_ENTITYAPPLICATION_H_

#include <Arduino.h>

#include <SettingsStorage.h>
#include <DeviceConfig.h>
#include <WiFi/WiFiManager.h>
#include <WiFi/WiFiServerManager.h>
#include <Entity.h>
#include <EntityManager.h>
#include <EntityUpdate.h>
#include <EntityUpdateManager.h>

#include <FileUtils.h>
#include <I2C_utils.h>
#include <functional>

#include <Ticker.h>
#include <Notifiers/Notifier.h>
#include <Notifiers/DataSelector.h>
#include <Notifiers/DataSelectorEntityManager.h>

#include <DeviceUtils.h>
#include <serve/ServerConnectionManager.h>
#include <serve/DeviceManager.h>

#ifdef SETTINGS_DISPLAY_ENABLED
	#include <display/DisplayManager.h>
	#include <display/DisplayPage.h>
	#include <display/PageToDisplayAdapter.h>
#endif

class EntityApplication {
public:
	EntityApplication(const char* firmWare, char* description, Entity* entities[], int entityCount,
			EntityUpdate* entityUpdate[], int entityUpdateCount,
			SettingsStorage* conf = nullptr,
			std::function<void(void)> onWiFiConnected = nullptr,
			std::function<void(void)> onWiFiDisConnected = nullptr);
	virtual ~EntityApplication() {
	}

	void construct(
#ifdef SETTINGS_DISPLAY_ENABLED
			PageToDisplayAdapter* displayAdapter, DisplayPage* pages[], unsigned char pageCount,
#endif
				const char* firmWare, char* description, Entity* entities[], int entityCount,
				EntityUpdate* entityUpdate[], int entityUpdateCount,
				SettingsStorage* conf = nullptr,
				std::function<void(void)> onWiFiConnected = nullptr,
				std::function<void(void)> onWiFiDisConnected = nullptr
				);

	void initWithWiFi(bool initI2C = false, uint8_t clockPin = SCL, uint8_t dataPin = SDA);
	void initWithoutWiFi(bool initI2C = false, uint8_t clockPin = SCL, uint8_t dataPin = SDA);

	void init(bool initSerial = true, bool initWiFi = false, bool initServer = false, bool initFs = true,
			bool deleteFs = false, bool initI2C = false, uint8_t clockPin = SCL, uint8_t dataPin = SDA);

	void setOnEntitiesChanged(std::function<void(void)> onEntitiesChanged);
	void loop();

	SettingsStorage* getConf();
	EntityManager* getEntityManager();
	EntityUpdateManager* getEntityUpdateManager();
	WiFiManager* getWiFiManager();
	WiFiServerManager* getWifiServerManager();
#ifndef SETTINGS_SERVER_CONNECTION_DISABLED
	ServerConnectionManager* getServerConnectionManager();
	void registerOnServer(bool trigger = true);
#endif
	DataSelector* getDataSelector();
	Notifier* getDefaultNotifier();

	void restart();

	void registerTicker(void (*callback)(void));
	void registerTicker(uint32_t milliseconds, void (*callback)(void));

	void updateEntities(bool force = false);

	void notify(char* group = nullptr, char* name = nullptr, char* param = nullptr, NotificationTarget* notifTarget = nullptr);

private:
	SettingsStorage* conf = nullptr;
	WiFiManager* wifiManager = nullptr;
	WiFiServerManager* wifiServerManager = nullptr;

	EntityManager* entityManager = nullptr;
	EntityUpdateManager* entityUpdateManager = nullptr;

	DataSelector* defaultDataSelector = nullptr;
	Notifier* defaultNotifier = nullptr;
	DeviceManager deviceManager;

#ifndef SETTINGS_SERVER_CONNECTION_DISABLED
	ServerConnectionManager* serverConnectionManager = nullptr;
#endif

#ifdef SETTINGS_DISPLAY_ENABLED
	DisplayManager* displayManager = nullptr;
#endif
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_ENTITYAPPLICATION_H_ */
