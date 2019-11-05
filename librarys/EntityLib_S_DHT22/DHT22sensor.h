/*
 * Bme280sensor.h
 *
 *  Created on: Mar 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSENSORS_DHT22SENSOR_H_
#define LIBRARIES_ENTITYLIBSENSORS_DHT22SENSOR_H_

#include <Entity.h>
#include <EntityUpdate.h>
#ifndef SENSORS_MOCKED
#include <DHT.h>
#else
#define DHT22 22
#define AM2301 21
#include <DHT22Mock.h>
#endif
#include "Emoji.h"
#include <DeviceConfig.h>

//--------------------------------
#define DHT22_NAME "dht22"
#define DHT22_DESCRIPTION "Temperature/Humidity"

#define DHT22_HUMIDITY "h"
#define DHT22_TEMPERATURE "t"

class DHT22sensor: public Entity, public EntityUpdate {
public:
	DHT22sensor(uint8_t pin, char* description = (char*)DHT22_DESCRIPTION, uint8_t type = DHT22, const char* name = DHT22_NAME) :
			Entity(GROUP_SENSORS, name, description) {
#ifndef SENSORS_MOCKED
		this->dht = new DHT(pin, type);
#else
		this->dht = new DHT22Mock(pin, DHT22);
#endif
	}

	virtual ~DHT22sensor() {
	}

	virtual void init() override {
		dht->begin();
#ifndef SENSORS_MOCKED
		bool result = dht->read(true);
		Serial.print(FPSTR("DHT22 read(force)="));
		Serial.println(result);

		for(int i=0; i<15; i++) {
			float h = dht->readHumidity();
			float t = dht->readTemperature();

			if(!(isnan(h) || isnan(t))){
				this->hum = h;
				this->temp = t;

				break;
			}
		}
#endif
	}

	virtual void doUpdate() override {
		float h = dht->readHumidity();
		float t = dht->readTemperature();

		for(int i=0; i<2; i++) {
			if(!(isnan(h) || isnan(t))){
				bool hchg = h!=this->hum;
				bool tchg = t!=this->temp;

				markEntityAsChangedIfTrue(hchg || tchg);

				this->hum = h;
				this->temp = t;

				if(hchg){putToBuffer(DHT22_HUMIDITY, this->hum);}
				if(tchg){putToBuffer(DHT22_TEMPERATURE, this->temp);}

				break;
			}
		}
	}

	virtual void doGet(JsonObject& params, JsonObject& response) override {
		UNUSED(params);
		float h = isnan(this->hum)?-279:this->hum;
		float t = isnan(this->temp)?-279:this->temp;

		response[DHT22_HUMIDITY] = h;
		response[DHT22_TEMPERATURE] = t;
	}

	virtual void doAppendFieldsSwg(JsonObject& swgJson) override{
		EntityDescriptor::appendSwgEntityParams(swgJson, EMOJI_SUN_CLOUD);

		EntityDescriptor::appendSwgFieldFloat(swgJson, DHT22_HUMIDITY, EDC_DESCR_HUMIDITY, EMOJI_DROPLET);
		EntityDescriptor::appendSwgFieldFloat(swgJson, DHT22_TEMPERATURE, EDC_DESCR_TEMPERATURE, EMOJI_THERMOMETER);
	}

protected:
#ifndef SENSORS_MOCKED
	DHT* dht = nullptr;;
#else
	DHT22Mock* dht;
#endif

	float hum = -100;
	float temp = -100;
};

#endif /* LIBRARIES_ENTITYLIBSENSORS_BME280SENSOR_H_ */
