/*
 * DHT22Sensor.h
 *
 *  Created on: 3 нояб. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_SENSORSLIB_DHT22_SENSOR_H_
#define LIBRARIES_SENSORSLIB_DHT22_SENSOR_H_

#include "Arduino.h"
#include "Initializable.h"
#include "AbstractItem.h"
#include "Initializable.h"
#include "Measurable.h"

#include "DHT.h"
#include "ESP_Consts.h"

const char SENSOR_DHT22_NAME[] PROGMEM ="Dht22Sensor";
const char SENSOR_DHT22_DESCRIPTION[] PROGMEM ="Dht22 Sensor";
const char SENSOR_DHT22_SIZE[] PROGMEM ="C/%";
const char SENSOR_DHT22_DESCRIPTION_RU[] PROGMEM ="Климат DHT 22";

const char HEADER_DHT22[] PROGMEM ="-------DHT_SENSOR initialize------------";
const char INIT_STR_DHT22[] PROGMEM ="Init DHT22";

class DHT22_Sensor: public AbstractItem, public Initializable{
public:
	DHT22_Sensor(String name, uint8_t pin, uint8_t type)
				:AbstractItem(pin,name,FPSTR(SENSOR_DHT22_DESCRIPTION),FPSTR(SENSOR_DHT22_SIZE),FPSTR(SENSOR_DHT22_DESCRIPTION_RU),2){

		dht=new DHT(pin,type);
	};
	virtual ~DHT22_Sensor(){};

	boolean initialize(boolean _init){
		if(_init){
			Serial.println(FPSTR(HEADER_DHT22));
			Serial.print(FPSTR(INIT_STR_DHT22));

			initSensor();
			update();

			Serial.println(FPSTR(MESSAGE_DONE));
			Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
		}
		initialized=_init;
		return initialized;
	}

	void initSensor(){
		dht->begin();

		items[0]={0,FPSTR(TEMPERATURE_DHT22),FPSTR(MEASURE_CELSIUS_DEGREES),FPSTR(MEASURE_CELSIUS_DEGREES_RU),FPSTR(TEMPERATURE_RU),-256,0,-512,512,"",0};
		items[1]={1,FPSTR(HUMIDITY_DHT22),FPSTR(MEASURE_PERSENT),FPSTR(MEASURE_PERSENT_RU),FPSTR(HUMIDITY_RU),-256,0,-512,512,"",0};

	}

	void update(){
		yield();
		bool timeOut=false;
		ulong startTime=millis();

		float temperature=-256;
		float humidity=-256;

		while(!timeOut){
			temperature=dht->readTemperature(false, true);

			timeOut=!isnan(temperature);

			if(!timeOut && (startTime+5000>millis())){timeOut=true;}

			if(!timeOut){delay(1000);}
		}

		timeOut=false;
		startTime=millis();

		while(!timeOut){
			humidity=dht->readHumidity(true);

			timeOut=!isnan(humidity);

			if(!timeOut && (startTime+5000>millis())){timeOut=true;}

			if(!timeOut){delay(1000);}
		}

		items[0].val=temperature;
		items[1].val=humidity;
	}

	void print(){
		printValues();
	}
private:
	DHT* dht;
};

#endif /* LIBRARIES_SENSORSLIB_DHT22_SENSOR_H_ */
