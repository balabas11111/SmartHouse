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

#include <Ticker.h>

class EntityApplication {
public:
	EntityApplication(const char* firmWare, Entity* entities[], int entityCount,
			EntityUpdate* entityUpdate[], int entityUpdateCount,
			std::function<void(void)> onEntityChanged = nullptr,
			WiFiSettingsBox* conf = nullptr,
			std::function<void(void)> onWiFiConnected = nullptr,
			std::function<void(void)> onWiFiDisConnected = nullptr);
	virtual ~EntityApplication() {
	}
	;

	void initWithWiFi(bool deleteFs = false, bool initI2C = false, uint8_t clockPin = SCL, uint8_t dataPin = SDA);
	void initWithoutWiFi(bool deleteFs = false, bool initI2C = false, uint8_t clockPin = SCL, uint8_t dataPin = SDA);

	void init(bool initSerial = true, bool initWiFi = false, bool initServer = false, bool initFs = true,
			bool deleteFs = false, bool initI2C = false, uint8_t clockPin = SCL, uint8_t dataPin = SDA);

	void loop();

	void startWiFi(){
		wifiManager->begin();
	}
	void startServer(){
		wifiServerManager->begin();
	}

	EntityManager* getEntityManager() {
		return this->entityManager;
	}

	WiFiSettingsBox* getConf(){
		return this->conf;
	}

	void registerTicker(void (*callback)(void)){

		uint32_t tickInterval = 30000;

		if(this->conf != nullptr && this->conf->refreshInterval()>0){
			tickInterval = conf->refreshInterval() * 1000;
		}


		registerTicker(tickInterval, callback);
	}

	void registerTicker(uint32_t milliseconds, void (*callback)(void)){
		Serial.print(FPSTR("register ticker interval = "));
		Serial.print(milliseconds);
		Ticker* ticker = new Ticker();
		ticker->attach_ms(milliseconds, callback);
		Serial.println(FPSTR(" done"));
	}

	void updateEntityUpdate(){
		entityUpdateManager->loop();
	}

private:
	WiFiSettingsBox* conf;
	WiFiManager* wifiManager;
	WiFiServerManager* wifiServerManager;

	EntityManager* entityManager;
	EntityUpdateManager* entityUpdateManager;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_ENTITYAPPLICATION_H_ */
