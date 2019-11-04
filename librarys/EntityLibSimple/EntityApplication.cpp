/*
 * EntityApplication.cpp
 *
 *  Created on: 30 рту. 2019 у.
 *      Author: Vitaliy
 */

#include <EntityApplication.h>

EntityApplication::EntityApplication(const char* firmWare, char* description,
		Entity* entities[], int entityCount, EntityUpdate* entityUpdate[],
		int entityUpdateCount, SettingsStorage* conf,
		std::function<void(void)> onWiFiConnected,
		std::function<void(void)> onWiFiDisConnected) {

	this->construct(
#ifdef SETTINGS_DISPLAY_ENABLED
			nullptr, nullptr, 0,
#endif
			firmWare, description, entities, entityCount, entityUpdate,
			entityUpdateCount, conf, onWiFiConnected, onWiFiDisConnected);
}

void EntityApplication::construct(
#ifdef SETTINGS_DISPLAY_ENABLED
		PageToDisplayAdapter* displayAdapter, DisplayPage* pages[], unsigned char pageCount,
#endif
		const char* firmWare, char* description, Entity* entities[],
		int entityCount, EntityUpdate* entityUpdate[], int entityUpdateCount,
		SettingsStorage* conf, std::function<void(void)> onWiFiConnected,
		std::function<void(void)> onWiFiDisConnected) {

	bool newConf = conf == nullptr;
	if (newConf) {
		Serial.println(FPSTR("New SettingsStorage will be created"));
	}

	this->conf = (newConf) ? new SettingsStorage(firmWare, description) : conf;

	this->entityManager = new EntityManager(entities, entityCount, this->conf);

	if (newConf) {
		this->entityManager->registerAndPreInitEntity(this->conf);
	}
	this->entityManager->registerAndPreInitEntity(&this->deviceManager);

#ifdef SETTINGS_DISPLAY_ENABLED
	this->displayManager = new DisplayManager(displayAdapter, pages, pageCount, this->conf);
#endif
	this->entityUpdateManager = new EntityUpdateManager(entityUpdate,
			entityUpdateCount);
	this->entityUpdateManager->registerEntity(&this->deviceManager);

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
		DeviceUtils::initSerial();
	}

	if (initFs) {
		FileUtils::init();
	}
	if (deleteFs) {
		FileUtils::deleteAllFiles("/data/");
	}

	if (initI2C) {
		I2C_utils::initStatic(clockPin, dataPin);
	}

	this->conf->initDeviceId();

	DeviceUtils::printHeap();
	DeviceUtils::printMillis();
#ifdef SETTINGS_DISPLAY_ENABLED
	this->displayManager->init();
#endif

	this->entityManager->init();
	this->entityUpdateManager->init(this->conf->refreshInterval());

	if (initWiFi) {
		this->wifiManager->begin();
	}
	if (initServer) {
		this->wifiServerManager->begin();
	}

#ifndef SETTINGS_SERVER_MQTT_DISABLED
	this->mqttManager = new MqttManager(this->conf);
#endif

#ifndef SETTINGS_SERVER_CONNECTION_DISABLED
	this->serverConnector = new ServerConnector(this->conf, [this]() {onServerRegistered();});
#endif
	this->defaultDataSelector = new DataSelectorEntityManager(
			this->entityManager);
	this->defaultNotifier = new Notifier("Default Notifier", nullptr,
			this->getDataSelector());

	Serial.println(FPSTR("Init done"));

	DeviceUtils::printHeap();
	DeviceUtils::printMillis();
	Serial.println(FPSTR("===================================="));
}

void EntityApplication::initWithWiFi(bool initI2C, uint8_t clockPin,
		uint8_t dataPin) {
	init(true, true, true, true,
	false, initI2C, clockPin, dataPin);
}

void EntityApplication::initWithoutWiFi(bool initI2C, uint8_t clockPin,
		uint8_t dataPin) {
	init(true, false, false, true,
	false, initI2C, clockPin, dataPin);
}

void EntityApplication::loop() {
	bool updated = this->entityUpdateManager->updateEntities();

#ifndef SETTINGS_SERVER_CONNECTION_DISABLED

#ifdef SETTINGS_SERVER_SEND_DATA_METHOD_GET

	if(this->entityManager->processChangedEntities(this->serverConnectionManager->getBuffer())) {
		this->serverConnectionManager->triggerDataChangedDoSend();
	}

#else
#ifndef SETTINGS_SERVER_HTTP_DATA_UPDATE_DISPATCH_DISABLED
	if(this->entityManager->processChangedEntities()) {
		this->serverConnectionManager->triggerDataChanged();
	}
#endif
#endif
	if (updated) {
		this->getServerConnector()->triggerCheckConnection();
		this->getServerConnector()->loop();
	}

#endif
#ifndef SETTINGS_SERVER_MQTT_DISABLED

#ifdef SETTINGS_SERVER_HTTP_DATA_UPDATE_DISPATCH_DISABLED
	if (this->entityManager->processChangedEntities()) {

		this->mqttManager->publishBuffer();
	}
#endif

	this->mqttManager->loop();
	this->wifiServerManager->loop();

#else
	this->entityManager->processChangedEntities();
	this->wifiServerManager->loop();
#endif

}

EntityManager* EntityApplication::getEntityManager() {
	return this->entityManager;
}

SettingsStorage* EntityApplication::getConf() {
	return this->conf;
}

void EntityApplication::registerTicker(void (*callback)(void)) {

	uint32_t tickInterval = 30000;

	if (this->conf != nullptr && this->conf->refreshInterval() > 0) {
		tickInterval = conf->refreshInterval() * 1000;
	}

	registerTicker(tickInterval, callback);
}

void EntityApplication::registerTicker(uint32_t milliseconds,
		void (*callback)(void)) {
	Serial.print(FPSTR("register ticker interval = "));
	Serial.print(milliseconds);
	Ticker* ticker = new Ticker();
	ticker->attach_ms(milliseconds, callback);
	Serial.println(FPSTR(" done"));
}

void EntityApplication::updateEntities(bool force) {
	Serial.println(FPSTR("Entity update triggered by app"));
	this->entityUpdateManager->updateEntities(force);
}

Notifier* EntityApplication::getDefaultNotifier() {
	return this->defaultNotifier;
}

void EntityApplication::notify(char* group, char* name, char* param,
		NotificationTarget* notifTarget) {
	if (this->defaultNotifier == nullptr) {
		return;
	}
	getDefaultNotifier()->notify(group, name, param, notifTarget);
}

void EntityApplication::setOnEntitiesChanged(
		std::function<void(void)> onEntitiesChanged) {
	this->getEntityManager()->setOnEntitiesChanged(onEntitiesChanged);
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

#ifndef SETTINGS_SERVER_CONNECTION_DISABLED
ServerConnector* EntityApplication::getServerConnector() {
	return this->serverConnector;
}

#endif

void EntityApplication::checkServerRegistration() {
#ifndef SETTINGS_SERVER_CONNECTION_DISABLED
	this->getServerConnector()->checkConnection();
#endif
}

DataSelector* EntityApplication::getDataSelector() {
	return this->defaultDataSelector;
}

void EntityApplication::restart() {
	this->deviceManager.triggerRestart();
}

void EntityApplication::onServerRegistered() {
	//Serial.println(FPSTR("OnServerRegistered"));
	this->mqttManager->init(entityManager->getBuffer());
}
