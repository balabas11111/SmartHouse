/*
 * Bme180Measures.h
 *
 *  Created on: 14 îêò. 2018 ã.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_SENSORS_BH1750_SENSOR_H_
#define LIBRARIES_SENSORS_BH1750_SENSOR_H_

#include "Arduino.h"
#include "interfaces/ItemFieldProvider.h"
#include "interfaces/Initializable.h"

#include "AbstractItem.h"
#include "ESP_Consts.h"

#include "BH1750.h"

#define BH1750_ID 201

const char SENSOR_BH1750_NAME[] PROGMEM ="BH1750_Sensor";
const char SENSOR_BH1750_DESCRIPTION[] PROGMEM ="LuxMeter";
const char SENSOR_BH1750_SIZE[] PROGMEM ="Lux";
const char SENSOR_BH1750_DESCRIPTION_RU[] PROGMEM ="Освещенность BH1750";

const char HEADER_BH1750[] PROGMEM ="-------BH1750_SENSOR initialize------------";
const char INIT_STR_BH1750[] PROGMEM ="Init luxmeter";

class BH1750_Sensor: public AbstractItem, public Initializable{

public:

	BH1750_Sensor(String name)
	: AbstractItem(BH1750_ID,name,FPSTR(SENSOR_BH1750_DESCRIPTION),FPSTR(SENSOR_BH1750_SIZE),FPSTR(SENSOR_BH1750_DESCRIPTION_RU),1){
	}

	~BH1750_Sensor(){}

	boolean initialize(boolean _init){
		if(_init){
			Serial.println(FPSTR(HEADER_BH1750));
			Serial.print(FPSTR(INIT_STR_BH1750));

			initSensor();
			update();

			Serial.println(FPSTR(MESSAGE_DONE));
			Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
		}
		initialized=_init;
		return initialized;
	}

	void initSensor(){
		lightMeter.begin();
		items[0]={0,FPSTR(LIGHT),FPSTR(MEASURE_LIGHT),FPSTR(MEASURE_LIGHT_RU),FPSTR(LIGHT_RU),-127,0,-512,512,"",0};
	}
	//----------------------------------------------

	void update(){
		items[0].val=(float)lightMeter.readLightLevel();
	}

	void print(){
		printValues();
	}

	String getKind() override{
		return KIND_SENSOR;
	}

private:
	BH1750 lightMeter;;
};



#endif /* LIBRARIES_SENSOR_BH1750_SENSOR_H_ */
