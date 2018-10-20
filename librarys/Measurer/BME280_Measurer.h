/*
 * Bme180Measures.h
 *
 *  Created on: 14 îêò. 2018 ã.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_MEASURER_BME280_MEASURER_H_
#define LIBRARIES_MEASURER_BME280_MEASURER_H_

#include <memory>
#include "Arduino.h"
//#include "Wire.h"
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
			: Measurer(_id, _name, FPSTR(MEASURER_BME280_NAME),FPSTR(MEASURER_BME280_DESCRIPTION),FPSTR(MEASURER_BME280_DESCRIPTION_RU), 4, false){
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

	String constructJson(){
		return getMeasurableAsJson();
	}

	String constructHtml(){
		String result=FPSTR(OPEN_HEADER);
			result+=getName();
			result+=FPSTR(ClOSE_HEAD_OPEN_CONT);
			for(uint8_t i=0;i<itemsCount;i++){
				result+=getFormRow(items[i].descr,(items[i].name+" "+items[i].kind),items[i].val);
			}
			result+=FPSTR(HTML_P_FORM_DIV_CLOSE);
		return result;
	}

	String getFormRow(String labelVal,String inputName,String inputVal){
		String result = FPSTR(HTML_LABEL_OPEN_WITH_B);
				result+=labelVal;
				result+=FPSTR(HTML_LABEL_CLOSE_WITH_B_INPUT);
				result+=inputName;
				result+=FPSTR(HTML_INPUT_OPEN_AFTER_NAME_TILL_VALUE);
				result+=inputVal;
				result+=FPSTR(HTML_INPUT_CLOSE_WITH_B);

		return result;
	}
	//----------------------------------------------

private:
	Adafruit_BME280 bme;
};



#endif /* LIBRARIES_MEASURER_BME180MEASURES_H_ */
