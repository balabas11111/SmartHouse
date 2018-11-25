/*
 * Bme180Measures.h
 *
 *  Created on: 14 îêò. 2018 ã.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_SENSOR_BME280_SENSOR_H_
#define LIBRARIES_SENSOR_BME280_SENSOR_H_

#include "Arduino.h"
#include "Initializable.h"
#include "AbstractItem.h"
#include "Initializable.h"
#include "Measurable.h"

#include "Adafruit_BME280.h"
#include "Adafruit_Sensor.h"
#include "ESP_Consts.h"

const char SENSOR_BME280_NAME[] PROGMEM ="Bme280Sensor";
const char SENSOR_BME280_DESCRIPTION[] PROGMEM ="Bme280Sensor";
const char SENSOR_BME280_SIZE[] PROGMEM ="C/%/Pa/m";
const char SENSOR_BME280_DESCRIPTION_RU[] PROGMEM ="Климат BME280";

const char HEADER_BME280[] PROGMEM ="-------BME280_SENSOR initialize------------";
const char INIT_STR_BME280[] PROGMEM ="Init BME-280";

class BME280_Sensor: public AbstractItem, public Initializable{

public:

	BME280_Sensor(String name,uint8_t id)
		: AbstractItem(id,name,FPSTR(SENSOR_BME280_DESCRIPTION),FPSTR(SENSOR_BME280_SIZE),FPSTR(SENSOR_BME280_DESCRIPTION_RU),4){
	}

	~BME280_Sensor(){}

	boolean initialize(boolean _init){
		if(_init){
			Serial.println(FPSTR(HEADER_BME280));
			Serial.print(FPSTR(INIT_STR_BME280));

			initSensor();
			update();

			Serial.println(FPSTR(MESSAGE_DONE));
			Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));

		}
		initialized=_init;
		return initialized;
	}

	void initSensor(){

		bool status=bme.begin();
		Serial.print(FPSTR(MESSAGE_STATUS_EQUAL));
		Serial.println(status);

		items[0]={0,FPSTR(TEMPERATURE),FPSTR(MEASURE_CELSIUS_DEGREES),FPSTR(MEASURE_CELSIUS_DEGREES_RU),FPSTR(TEMPERATURE_RU),-127,0,-512,512,"",0};
		items[1]={1,FPSTR(PRESSURE),FPSTR(MEASURE_PASCAL),FPSTR(MEASURE_PASCAL_RU),FPSTR(PRESSURE_RU),-127,0,-512,120000,"",0};
		items[2]={2,FPSTR(HUMIDITY),FPSTR(MEASURE_PERSENT),FPSTR(MEASURE_PERSENT_RU),FPSTR(HUMIDITY_RU),-127,0,-512,512,"",0};
		items[3]={3,FPSTR(ALTITUDE),FPSTR(MEASURE_METER),FPSTR(MEASURE_METER_RU),FPSTR(ALTITUDE_RU),-127,0,-512,10000,"",0};

	}

	void update(){
		items[0].val=(float)bme.readTemperature();
		items[1].val=(float)bme.readPressure();
		items[2].val=(float)bme.readHumidity();
		items[3].val=(float)bme.readAltitude(1013.25);
	}
	void print(){
		printValues();
	}

private:
	Adafruit_BME280 bme;
};



#endif /* LIBRARIES_SENSOR_BME180MEASURES_H_ */
