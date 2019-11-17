/*
 * EntityApplication.h
 *
 *  Created on: 30 ���. 2019 �.
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
#include <serve/ServerConnector.h>

#include <utils/DeviceUtils.h>
#include <utils/FileUtils.h>
#include <utils/I2C_utils.h>
#include <functional>

#include <Ticker.h>
#include <Notifiers/Notifier.h>
#include <Notifiers/DataSelector.h>
#include <Notifiers/DataSelectorEntityManager.h>


#include <serve/DeviceManager.h>
#include <serve/MqttManager.h>

#ifdef SETTINGS_DISPLAY_ENABLED
	#include <display/DisplayManager.h>
	#include <display/DisplayPage.h>
	#include <display/PageToDisplayAdapter.h>
#endif

class EntityApplication {
public:
	EntityApplication(const char* firmWare, char* description, Entity* entities[], int entityCount,
			EntityUpdate* entityUpdate[], int entityUpdateCount, const char* emoji = EMOJI_FIRE,
#ifdef SETTINGS_DISPLAY_ENABLED
			PageToDisplayAdapter* displayAdapter = nullptr, DisplayPage* pages[] = nullptr, unsigned char pageCount=0,
#endif
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
				EntityUpdate* entityUpdate[], int entityUpdateCount, const char* emoji,
				SettingsStorage* conf = nullptr,
				std::function<void(void)> onWiFiConnected = nullptr,
				std::function<void(void)> onWiFiDisConnected = nullptr
				);

	void begin(bool initI2C = false);

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
	ServerConnector* getServerConnector();
#endif

	DataSelector* getDataSelector();
	Notifier* getDefaultNotifier();

	void restart();

	void registerTicker(void (*callback)(void));
	void registerTicker(uint32_t milliseconds, void (*callback)(void));

	void updateEntities(bool force = false);

	void notify(char* group = nullptr, char* name = nullptr, char* param = nullptr, NotificationTarget* notifTarget = nullptr);

	void onServerRegistered();
#ifdef SETTINGS_DISPLAY_ENABLED
	DisplayManager* getDisplayManager() {
		return this->displayManager;
	}

	void switchToPage(int page) {
		this->displayManager->switchToPage(page);
	}

	void switchToNextNonStatusPage() {
		this->displayManager->switchToNextNonStatusPage();
	}

	void switchPagesWithInterval(unsigned long interval) {
		this->displayManager->switchToNextPageWithInterval(interval);
	}

	unsigned long interval = 0;
#endif
private:

	SettingsStorage* conf = nullptr;
	WiFiManager* wifiManager = nullptr;
	WiFiServerManager* wifiServerManager = nullptr;

	EntityManager* entityManager = nullptr;
	EntityUpdateManager* entityUpdateManager = nullptr;

	DataSelector* defaultDataSelector = nullptr;
	Notifier* defaultNotifier = nullptr;
	DeviceManager deviceManager;

#ifndef SETTINGS_SERVER_MQTT_DISABLED
	MqttManager* mqttManager = nullptr;
#endif

#ifndef SETTINGS_SERVER_CONNECTION_DISABLED
	ServerConnector* serverConnector = nullptr;
#endif

#ifdef SETTINGS_DISPLAY_ENABLED
	DisplayManager* displayManager = nullptr;
#endif
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_ENTITYAPPLICATION_H_ */
