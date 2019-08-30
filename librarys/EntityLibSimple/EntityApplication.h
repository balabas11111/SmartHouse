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

#include <functional>

class EntityApplication {
public:
	EntityApplication(const char* firmWare,
			Entity* entities[], int entityCount,
			EntityUpdate* entityUpdate[], int entityUpdateCount,
			WiFiSettingsBox* conf = nullptr ,
			bool initSerial = true,
			bool initFs = true,
			bool deleteFs = false,
			std::function<void(void)> onWiFiConnected = nullptr,
			std::function<void(void)> onWiFiDisConnected = nullptr);
	virtual ~EntityApplication(){};

	void initI2C(uint8_t clockPin = SCL,uint8_t dataPin = SDA);

	void init();

	void loop();

private:
	WiFiSettingsBox* conf;
	WiFiManager* wifiManager;
	WiFiServerManager* wifiServerManager;

	EntityManager* entityManager;
	EntityUpdateManager* entityUpdateManager;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_ENTITYAPPLICATION_H_ */
