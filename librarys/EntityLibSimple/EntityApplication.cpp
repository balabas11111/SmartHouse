/*
 * EntityApplication.cpp
 *
 *  Created on: 30 ���. 2019 �.
 *      Author: Vitaliy
 */

#include <EntityApplication.h>

EntityApplication::EntityApplication(const char* firmWare, Entity* entities[],
		int entityCount, EntityUpdate* entityUpdate[], int entityUpdateCount,
		WiFiSettingsBox* conf, std::function<void(void)> onWiFiConnected,
		std::function<void(void)> onWiFiDisConnected) {

	bool newConf = conf == nullptr;
	if (newConf) {
		Serial.println(FPSTR("New WiFISettingsBox will be created"));
	}

	this->conf = (newConf) ? new WiFiSettingsBox(firmWare) : conf;
	this->entityManager = new EntityManager(entities, entityCount);

	this->smartHouseServerHelper = new SmartHouseServerHelper(this->conf);

	if (newConf) {
		this->entityManager->registerAndPreInitEntity(this->conf);
	}

	this->entityUpdateManager = new EntityUpdateManager(entityUpdate,
			entityUpdateCount);

	this->wifiManager = new WiFiManager(this->conf, onWiFiConnected,
			onWiFiDisConnected);
	this->wifiServerManager = new WiFiServerManager(this->entityManager,
			this->conf);

}

void EntityApplication::init(bool initSerial,
		bool initWiFi, bool initServer,
		bool initFs,
		bool deleteFs,
		bool initI2C, uint8_t clockPin, uint8_t dataPin) {

	if (initSerial) {
		ObjectUtils::initSerial();
	}

	if (initFs) {
		FileUtils::init();
	}
	if (deleteFs) {
		FileUtils::deleteAllFiles("/data/");
	}

	if(initI2C){
		I2C_utils::initStatic(clockPin, dataPin);
	}

	Serial.println(FPSTR("--------------------"));
	Serial.print(FPSTR("Init application "));
	Serial.println(this->conf->deviceFirmWare());

	ObjectUtils::printHeap();
	ObjectUtils::printMillis();

	this->entityManager->init(this->conf, this->smartHouseServerHelper);
	this->entityUpdateManager->init(this->conf->refreshInterval());

	if(initWiFi){
		startWiFi();
	}
	if(initServer){
		startServer();
	}

	this->defaultDataSelector = new DataSelectorEntityManager(this->entityManager);
	this->defaultNotifier = new Notifier("Default Notifier", nullptr, this->getDataSelector());

	Serial.println(FPSTR("Application Init done"));

	ObjectUtils::printHeap();
	ObjectUtils::printMillis();
	Serial.println(FPSTR("===================================="));
}

void EntityApplication::initWithWiFi(bool deleteFs, bool initI2C,
		uint8_t clockPin, uint8_t dataPin) {
	init(true, true, true, true,
				deleteFs, initI2C, clockPin, dataPin);
}

void EntityApplication::initWithoutWiFi(bool deleteFs, bool initI2C,
		uint8_t clockPin, uint8_t dataPin) {
	init(true, false, false, true,
					deleteFs, initI2C, clockPin, dataPin);
}

void EntityApplication::loop() {
	this->entityManager->loop();
	this->entityUpdateManager->loop();
	this->wifiServerManager->loop();
	this->smartHouseServerHelper->loop();
}

void EntityApplication::startWiFi(){
	this->wifiManager->begin();
}
void EntityApplication::startServer(){
	this->wifiServerManager->begin();
}

EntityManager* EntityApplication::getEntityManager() {
	return this->entityManager;
}

WiFiSettingsBox* EntityApplication::getConf(){
	return this->conf;
}

void EntityApplication::registerTicker(void (*callback)(void)){

	uint32_t tickInterval = 30000;

	if(this->conf != nullptr && this->conf->refreshInterval()>0){
		tickInterval = conf->refreshInterval() * 1000;
	}


	registerTicker(tickInterval, callback);
}

void EntityApplication::registerTicker(uint32_t milliseconds, void (*callback)(void)){
	Serial.print(FPSTR("register ticker interval = "));
	Serial.print(milliseconds);
	Ticker* ticker = new Ticker();
	ticker->attach_ms(milliseconds, callback);
	Serial.println(FPSTR(" done"));
}

void EntityApplication::updateEntities(bool withCheck){
	this->entityUpdateManager->updateEntities(withCheck);
}

Notifier* EntityApplication::getDefaultNotifier() {
	return this->defaultNotifier;
}

void EntityApplication::notify(char* group, char* name,
		NotificationTarget* notifTarget) {
	if(this->defaultNotifier == nullptr){
		return;
	}
	getDefaultNotifier()->notify(group, name, nullptr, notifTarget);
}

void EntityApplication::setOnEntityChanged(
		std::function<void(void)> onEntityChanged) {
	this->getEntityManager()->setOnEntityChanged(onEntityChanged);
}

WiFiManager* EntityApplication::getWiFiManager() {
	return this->wifiManager;
}

EntityUpdateManager* EntityApplication::getEntityUpdateManager() {
	return this->entityUpdateManager;
}

WiFiServerManager* EntityApplication::getWifiServerManager() {
	return this->wifiServerManager;
}

SmartHouseServerHelper* EntityApplication::getSmartHouseServerHelper() {
	return this->smartHouseServerHelper;
}

DataSelector* EntityApplication::getDataSelector() {
	return this->defaultDataSelector;
}
