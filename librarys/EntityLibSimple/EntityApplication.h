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

#include <Entity.h>
#include <EntityManager.h>
#include <EntityUpdate.h>
#include <EntityUpdateManager.h>

#include <ObjectUtils.h>
#include <FileUtils.h>
#include <I2C_utils.h>
#include <Notifiers/EntityManagerNotifier.h>

#include <functional>

#include <Ticker.h>
#include <Notifiers/Notifier.h>

class EntityApplication {
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

	void setOnEntityChanged(std::function<void(void)> onEntityChanged);
	void loop();

	void startWiFi();
	void startServer();

	EntityManager* getEntityManager();
	WiFiManager* getWiFiManager();

	WiFiSettingsBox* getConf();

	void registerTicker(void (*callback)(void));
	void registerTicker(uint32_t milliseconds, void (*callback)(void));

	void updateEntities(bool withCheck = true);

	void initNotifier(Notifier* notifier);
	void initDefaultNotifier(NotificationTarget* target = nullptr);
	EntityManagerNotifier* getDefaultNotifier();

	void notify(char* group = nullptr, char* name = nullptr, NotificationTarget* notifTarget = nullptr);

private:
	WiFiSettingsBox* conf;
	WiFiManager* wifiManager;
	WiFiServerManager* wifiServerManager;

	EntityManager* entityManager;
	EntityUpdateManager* entityUpdateManager;

	EntityManagerNotifier* defaultNotifier;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_ENTITYAPPLICATION_H_ */
