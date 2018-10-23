/*
 * Bme180Measures.h
 *
 *  Created on: 14 îêò. 2018 ã.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_MEASURER_BME280_MEASURER_H_
#define LIBRARIES_MEASURER_BME280_MEASURER_H_

#include "Arduino.h"
#include "Initializable.h"
#include "Measurer.h"
#include "HtmlWidget.h"

#include "Adafruit_BME280.h"
#include "Adafruit_Sensor.h"

const char MEASURER_BME280_NAME[] PROGMEM ="Bme280Sensor";
const char MEASURER_BME280_DESCRIPTION[] PROGMEM ="Bme280Sensor";
const char MEASURER_BME280_DESCRIPTION_RU[] PROGMEM ="Климат BME280";

class BME280_Measurer: public Measurer, public HtmlWidget{

public:

	BME280_Measurer(String _id,String _name)
			:Measurer(_id, _name, FPSTR(MEASURER_BME280_NAME),FPSTR(MEASURER_BME280_DESCRIPTION),FPSTR(MEASURER_BME280_DESCRIPTION_RU), 4, false){
	}

	~BME280_Measurer(){
		Serial.println("destructed");
	}

	boolean initialize(boolean _init){
		Serial.println("Begin initialize of BME280_Measurer");
		if(_init){
			Serial.println("-------BME280_Measurer initialize------------");
			Serial.print("Init BME-280");
			bool status=bme.begin();
			Serial.print(" status="+String(status));
			Serial.println("...done");
			Serial.println("---------------------------------------------");
			measure();
		}
		initialized=_init;
		return initialized;
	}

	void getExternal() override{
		items[0]=Measureable(FPSTR(TEMPERATURE),FPSTR(TEMPERATURE_RU),FPSTR(MEASURE_CELSIUS_DEGREES),String(bme.readTemperature()),FPSTR(MEASURE_CELSIUS_DEGREES_RU));
		items[1]=Measureable(FPSTR(PRESSURE),FPSTR(PRESSURE_RU),FPSTR(MEASURE_PASCAL),String(bme.readPressure()),FPSTR(MEASURE_PASCAL_RU));
		items[2]=Measureable(FPSTR(HUMIDITY),FPSTR(HUMIDITY_RU),FPSTR(MEASURE_PERSENT),String(bme.readHumidity()),FPSTR(MEASURE_PERSENT_RU));
		items[3]=Measureable(FPSTR(ALTITUDE),FPSTR(ALTITUDE_RU),FPSTR(MEASURE_METER),String(bme.readAltitude(1013.25)),FPSTR(MEASURE_METER_RU));
	}

	//------------------------HtmlWidgetProcessing---------------------------
	String getName(){
		return Measurer::getName();
	}

	String executeClientAction(String actionName,String remoteId,String remoteVal, String className, String childClass,String clientData){

		printCommand(actionName, remoteId, remoteVal, className, childClass, clientData);

		if(actionName.equals(ACTION_GET_WIDGET_HTML_OR_VAL)
				&&className.equals(CLASS_REFRESHABLE_MeasurerWidgetESP)){

			return FPSTR(HTML_BME280);
		}

		if(actionName.equals(ACTION_GET_WIDGETS_CHILDREN_AS_JSON)
				&&className.equals(CLASS_REFRESHABLE_CHILDREN_MeasurerWidgetESPJson)
				&&childClass.equals(CLASS_REFRESHABLE_CHILD)){

			return Measurer::getChildrenJson();
		}

		return getNotAllowed();
	}
	//----------------------------------------------

private:
	Adafruit_BME280 bme;
};



#endif /* LIBRARIES_MEASURER_BME180MEASURES_H_ */
