/*
 * EntityApplication.cpp
 *
 *  Created on: 30 ���. 2019 �.
 *      Author: Vitaliy
 */

#include <EntityApplication.h>

EntityApplication::EntityApplication(const char* firmWare, char* description,
		Entity* entities[], int entityCount, EntityUpdate* entityUpdate[],
		int entityUpdateCount, const char* emoji,
#ifdef SETTINGS_DISPLAY_ENABLED
		PageToDisplayAdapter* displayAdapter, DisplayPage* pages[], unsigned char pageCount,
#endif
		SettingsStorage* conf,
		std::function<void(void)> onWiFiConnected,
		std::function<void(void)> onWiFiDisConnected) {

	this->construct(
#ifdef SETTINGS_DISPLAY_ENABLED
			displayAdapter, pages, pageCount,
#endif
			firmWare, description, entities, entityCount, entityUpdate,
			entityUpdateCount, emoji, conf, onWiFiConnected, onWiFiDisConnected);
}

void EntityApplication::construct(
#ifdef SETTINGS_DISPLAY_ENABLED
		PageToDisplayAdapter* displayAdapter, DisplayPage* pages[], unsigned char pageCount,
#endif
		const char* firmWare, char* description, Entity* entities[],
		int entityCount, EntityUpdate* entityUpdate[], int entityUpdateCount, const char* emoji,
		SettingsStorage* conf, std::function<void(void)> onWiFiConnected,
		std::function<void(void)> onWiFiDisConnected) {

	bool newConf = conf == nullptr;
	if (newConf) {
		Serial.println(FPSTR("New SettingsStorage will be created"));
	}

	this->conf = (newConf) ? new SettingsStorage(firmWare, description, emoji) : conf;

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

void EntityApplication::begin(bool initI2C) {
	initWithWiFi(initI2C);
	updateEntities(true);
	//checkServerRegistration();
	this->getServerConnector()->triggerCheckRegistrationOnServer();
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

	this->entityManager->init();
	this->entityUpdateManager->init(this->conf->refreshInterval());

#ifdef SETTINGS_DISPLAY_ENABLED
	this->displayManager->init();
#endif

	if (initWiFi) {
		this->wifiManager->begin();
	}
	if (initServer) {
		this->wifiServerManager->begin();
	}


#ifndef SETTINGS_SERVER_MQTT_DISABLED
	if(this->entityManager->isHasTopicConnection()) {
		this->mqttManager = new MqttManager(this->conf);
	}
#endif

	this->serverConnector = new ServerConnector(this->conf, [this]() {onServerRegistered();});

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

	bool shrReceived = this->wifiServerManager->isShrReceived();
	unsigned long lastTime = this->wifiServerManager->getLastSrequestTime();

	bool deviceRegistrationTimedOut =
			this->getServerConnector()->isLastRequestFromServerTimedOut(
					shrReceived,
					lastTime);

	if (deviceRegistrationTimedOut) {
		if(updated) {
			Serial.println(FPSTR("entities Updated"));
		}
		if(!this->getServerConnector()->isTriggeredCheckRegistrationOnServer()) {
			Serial.print(FPSTR("TimeOut shrReceived="));
			Serial.print(shrReceived);
			Serial.print(FPSTR(" lastTime="));
			Serial.print(lastTime);
			Serial.print(FPSTR(" expectUpdate="));
			Serial.print(lastTime);
			Serial.print(FPSTR(" now="));
			Serial.println(millis());
		}
		this->getWifiServerManager()->setShrReceived(false);
		this->getServerConnector()->triggerCheckRegistrationOnServer();
	}

	this->getServerConnector()->loop();

#ifndef SETTINGS_SERVER_MQTT_DISABLED
	if(this->entityManager->isHasTopicConnection()) {
		EntityJsonRequestResponse* buffer = this->mqttManager->getBuffer();

		if (this->mqttManager->isBufferUnsent() || this->entityManager->processChangedEntities(buffer)) {
			this->mqttManager->publishBuffer();
		}

		if(this->mqttManager->isBufferUnsent()) {
			Serial.println(FPSTR("Buffer UNSENT"));
			this->getServerConnector()->dispatchDataChanged();
		}

		this->mqttManager->loop();
	}
#endif

#ifndef SETTINGS_SERVER_GET_DISPATCH_CHANGES_DISABLED
	if(this->entityManager->processChangedEntities()) {
		this->getServerConnector()->dispatchDataChanged();
	}
#endif

#if defined(SETTINGS_SERVER_GET_DISPATCH_CHANGES_DISABLED) && defined(SETTINGS_SERVER_MQTT_DISABLED)
		bool entitiesChanged = this->entityManager->processChangedEntities();
		if(entitiesChanged) {
#ifdef SETTINGS_DISPLAY_ENABLED
	this->displayManager->renderCurrentPage();
#endif
		}
#endif

#ifdef SETTINGS_DISPLAY_ENABLED
	this->displayManager->loop();
#endif

	this->wifiServerManager->loop();
	this->deviceManager.loop();
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

DataSelector* EntityApplication::getDataSelector() {
	return this->defaultDataSelector;
}

void EntityApplication::restart() {
	this->deviceManager.triggerRestart();
}

void EntityApplication::onServerRegistered() {
	//Serial.println(FPSTR("OnServerRegistered"));
#ifndef SETTINGS_SERVER_MQTT_DISABLED
	if(this->entityManager->isHasTopicConnection()) {
		this->mqttManager->init(entityManager->getBuffer());
	}
#endif
}

