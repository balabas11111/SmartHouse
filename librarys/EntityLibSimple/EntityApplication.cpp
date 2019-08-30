/*
 * EntityApplication.cpp
 *
 *  Created on: 30 рту. 2019 у.
 *      Author: Vitaliy
 */

#include <EntityApplication.h>

EntityApplication::EntityApplication(const char* firmWare, Entity* entities[],
		int entityCount, EntityUpdate* entityUpdate[], int entityUpdateCount,
		WiFiSettingsBox* conf,
		bool initSerial,
		bool initFs,
		bool deleteFs,
		std::function<void(void)> onWiFiConnected,
		std::function<void(void)> onWiFiDisConnected) {

	if(initSerial){
		ObjectUtils::initSerial();
	}

	if (initFs) {
		FileUtils::init();
	}
	if (deleteFs) {
		FileUtils::deleteAllFiles("/data/");
	}

	bool newConf = conf == nullptr;
	if(newConf){
		Serial.println(FPSTR("New WiFISettingsBox will be created"));
	}

	this->conf = (newConf) ? new WiFiSettingsBox(firmWare) : conf;
	this->entityManager = new EntityManager(entities, entityCount);

	if (newConf) {
		this->entityManager->registerAndPreInitEntity(this->conf);
	}

	this->entityUpdateManager = new EntityUpdateManager(entityUpdate,
			entityUpdateCount);

	this->wifiManager = new WiFiManager(this->conf, onWiFiConnected,
			onWiFiDisConnected);
	this->wifiServerManager = new WiFiServerManager(this->entityManager,
			this->conf);

	ObjectUtils::printHeap();
	ObjectUtils::printMillis();
}

void EntityApplication::initI2C(uint8_t clockPin, uint8_t dataPin) {
	I2C_utils::initStatic(clockPin, dataPin);
}

void EntityApplication::init() {
	Serial.println(FPSTR("Init application"));

	entityManager->init();
	entityUpdateManager->init(this->conf->refreshInterval());

	wifiManager->begin();
	wifiServerManager->begin();

	Serial.println(FPSTR("Application Init done"));
}

void EntityApplication::loop() {
	this->entityUpdateManager->loop();
	wifiServerManager->loop();
}
