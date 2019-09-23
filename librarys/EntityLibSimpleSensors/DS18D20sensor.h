/*
 * DS18D20sensor.h
 *
 *  Created on: Mar 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSENSORS_DS18D20SENSOR_H_
#define LIBRARIES_ENTITYLIBSENSORS_DS18D20SENSOR_H_

#include <ArduinoJson.h>
#include <Entity.h>
#include <EntityUpdate.h>

#include <OneWire.h>
#include "DallasTemperature.h"
#include "ObjectUtils.h"

#include <OneWireMock.h>
#include "DallasTemperatureMock.h"

//--------------------------------
#define DS18D20_NAME "ds18d20"
#define DS18D20_DESCRIPTION "Temperature DallasTemperature"

#define DS18D20_SENSOR_COUNT "c"
#define DS18D20_SENSOR_ID "i"
#define DS18D20_SENSOR_NAME "n"
#define DS18D20_SENSOR_TEMPERATURE "t"

#define DS18D20_SENSOR_ITEMS "si"

class DS18D20item {
public:
	DeviceAddress uid;
	char* uidStr;
	char* descr;
	float temp;
};

class DS18D20sensor: public Entity, public EntityUpdate {
public:
	DS18D20sensor(int pin) :
			Entity(GROUP_SENSORS, DS18D20_NAME, (char*)DS18D20_DESCRIPTION) {
		construct(pin);
	}

	virtual ~DS18D20sensor() {
	}

	virtual void init() override {
		dallasTemperature->begin();
		itemCount = dallasTemperature->getDeviceCount();

		items = new DS18D20item[itemCount];

		for (uint8_t i = 0; i < itemCount; i++) {
			DS18D20item item = DS18D20item();

			item.uidStr = strdup(dallasTemperature->getAddress(item.uid, i).c_str());
			item.descr = item.uidStr;
			item.temp = 127;

			items[i] = item;
		}
		readTemperatures(false);
		print();
	}

	virtual void doUpdate() override {
		readTemperatures(true);
	}

	void print() {
		Serial.println(FPSTR("DS18D20 Entity"));
		Serial.print(FPSTR(" name = "));
		Serial.print(name);
		Serial.print(FPSTR(" itemCount = "));
		Serial.println(itemCount);
		Serial.println(FPSTR("-----"));
		for (uint8_t i = 0; i < itemCount; i++) {
			Serial.print(FPSTR("i="));
			Serial.print(i);
			Serial.print(FPSTR(" address="));
			Serial.print(getDeviceAddress(i));

			Serial.print(FPSTR(" uidStr="));
			Serial.print(items[i].uidStr);
			Serial.print(FPSTR(" descr="));
			Serial.print(items[i].descr);
			Serial.print(FPSTR(" val="));
			Serial.println(items[i].temp);
		}
		Serial.println(FPSTR("-----"));
	}

	virtual void doGet(JsonObject& params, JsonObject& response) override {
		UNUSED(params);
		itemsToJson(response, true);
	}

	virtual void doPost(JsonObject& params, JsonObject& response) override {
		UNUSED(response);

		bool descrChanged = false;

		for (int i = 0; i<itemCount; i++) {
			String nameKey = String(items[i].uidStr)+ ".";

			nameKey +=DS18D20_SENSOR_NAME;

			Serial.print(nameKey);

			if (JsonObjectUtil::hasField<char*>(params, nameKey.c_str())) {
				Serial.print(FPSTR(" - found"));

				if (getKeyValueIfExistsAndNotEquals(params, nameKey.c_str(), &items[i].descr)) {
					Serial.print(FPSTR(" - updated ->"));
					Serial.println(items[i].descr);
					descrChanged = true;
				}else{
					Serial.println();
				}

			}
		}
		markEntityAsChangedIfTrue(descrChanged);
		markEntityAsSaveRequiredIfTrue(descrChanged);
	}

	virtual void doLoad(JsonObject& jsonFromFile) override {
		jsonFromFileToItems(jsonFromFile);
	}

	virtual void doSave(JsonObject& jsonToFile) override {
		itemsToJson(jsonToFile, false);
	}

protected:
	int itemCount = 0;
	OneWireMock* oneWire;
	DallasTemperatureMock* dallasTemperature;

	DS18D20item* items;

	void construct(int pin) {
		this->oneWire = new OneWireMock(pin);
		this->dallasTemperature = new DallasTemperatureMock(oneWire);
	}

	virtual void readTemperatures(bool dispatchChange) {
		dallasTemperature->requestTemperatures();
		bool tChanged = false;

		for (uint8_t i = 0; i < itemCount; i++) {
			float t = dallasTemperature->getTempCByIndex(i);

			if (t != items[i].temp) {
				tChanged = true;
			}
			items[i].temp = t;
		}

		if (dispatchChange) {
			markEntityAsChangedIfTrue(tChanged);
		}
	}

	String getDeviceAddress(uint8_t index) {
		DeviceAddress deviceAddress;
		return dallasTemperature->getAddress(deviceAddress, index);
	}

	bool jsonFromFileToItems(JsonObject& json) {
		bool chg = false;

		if (JsonObjectUtil::hasField<JsonObject>(json, DS18D20_SENSOR_ITEMS)) {
			JsonObject& sensors = JsonObjectUtil::getFieldAsObject(json, DS18D20_SENSOR_ITEMS);

			for (int i = 0; i<itemCount; i++) {
				if (JsonObjectUtil::hasField<JsonObject>(sensors, items[i].uidStr)) {

					JsonObject& sensor = JsonObjectUtil::getFieldAsObject(json, items[i].uidStr);

					if (getKeyValueIfExistsAndNotEquals(sensor,	DS18D20_SENSOR_NAME, &items[i].descr)) {
						chg = true;
					}
				}
			}
		}

		return chg;
	}

	void itemsToJson(JsonObject& json, bool addTemp) {
		setJsonField(json, DS18D20_SENSOR_COUNT, this->itemCount);
		JsonObject& sensors = JsonObjectUtil::getObjectChildOrCreateNewNoKeyDup(
				json, DS18D20_SENSOR_ITEMS);


		for (int i = 0; i < itemCount; i++) {
			JsonObject& sensor =
					JsonObjectUtil::getObjectChildOrCreateNewNoKeyDup(sensors,
							items[i].uidStr);

			setJsonField(sensor, (DS18D20_SENSOR_NAME), items[i].descr);
			if (addTemp) {
				setJsonField(sensor, (DS18D20_SENSOR_TEMPERATURE),
						items[i].temp);
			}

		}

	}
};

#endif /* LIBRARIES_ENTITYLIBSENSORS_DS18D20sensor_H_ */
