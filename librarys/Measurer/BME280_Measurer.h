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
#include "Measurer.h"
#include "HtmlWidget.h"

#include "Adafruit_BME280.h"
#include "Adafruit_Sensor.h"

const char MEASURER_NAME[] PROGMEM ="Bme280Sensor";
const char MEASURER_DESCRIPTION[] PROGMEM ="Bme280Sensor";

const char MEASURER_DESCRIPTION_RU[] PROGMEM ="БМЕ 280 датчик";

class BME280_Measurer: public Measurer, public HtmlWidget{

public:

	BME280_Measurer(String _id,String _name)
			: Measurer(_id, _name, FPSTR(MEASURER_NAME), FPSTR(MEASURER_DESCRIPTION), 4, false){
	}

	~BME280_Measurer(){
		Serial.println("destructed");
	}

	String getName(){
		return Measurer::getName();
	}

	String getJson(){
		return getMeasurableAsJson();
	}

		const PROGMEM char HTML_DIV_OPEN_W3CARD_25_PERC[]     = "<div class=\"w3-card-4\" style=\"width:25%;  float:left; min-width: 250px; min-height:270px; margin-left: 10px; margin-top: 10px; margin-right: 10px; margin-bottom: 10px;\">";
		const PROGMEM char HTML_DIV_OPEN_W3CONTAINER_W3TEAL[] = "<div class=\"w3-container w3-teal\">";
		const PROGMEM char HTML_FORM_OPEN_W3Container[] = "<form class=\"w3-container\">";
		const PROGMEM char HTML_LABEL_OPEN_WITH_B[] = "<label style=\"max-width: 240px;\"><b>";
		const PROGMEM char HTML_LABEL_CLOSE_WITH_B[] = "</b></label>";
		const PROGMEM char HTML_INPUT_OPEN_TILL_NAME[] = "<input class=\"w3-input w3-border\" style=\"width:95%\" name=\"";
		const PROGMEM char HTML_INPUT_OPEN_AFTER_NAME_TILL_VALUE[] = "\" type=\"text\" value=\"";
		const PROGMEM char HTML_INPUT_CLOSE_WITH_B[] = "\" disabled>";

	String getHtml(){
		String result=FPSTR(HTML_DIV_OPEN_W3CARD_25_PERC)+FPSTR(HTML_DIV_OPEN_W3CONTAINER_W3TEAL)
								+FPSTR(HTML_H2_OPEN)+getName()+FPSTR(HTML_H2_CLOSE)
					 +FPSTR(HTML_DIV_CLOSE)
					 	 +FPSTR(HTML_FORM_OPEN_W3Container)
						 	 +

				<label style=\"max-width: 240px;\"><b>";
				result+=sensorHelper.getValueByKey(dhtTemperatureAddress);
				result+="</b></label>\
				<input class=\"w3-input w3-border\" style=\"width:95%\" name=\"s_tempValue\" type=\"text\" value=\"";
				result+=dht.readTemperature(false,false);
				result+="\" disabled>\
			<p></p>\
			</form>\
		</div>\

		return "";
	}

	String getFormRow(String labelVal,String inputName,String inputVal){
		String result=FPSTR(HTML_LABEL_OPEN_WITH_B)+labelVal+FPSTR(HTML_LABEL_OPEN_WITH_B)
			 +FPSTR(HTML_INPUT_OPEN_TILL_NAME)+inputName+FPSTR(HTML_INPUT_OPEN_AFTER_NAME_TILL_VALUE)+inputVal+FPSTR(HTML_INPUT_CLOSE_WITH_B);

		return result;
	}

	boolean init(){
		Serial.print("Init BME-280");
		bool status=bme.begin();
		Serial.print(" status="+String(status));
		Serial.println("...done");

		initialized=true;
		return status;
	}

	void getExternal() override{
		items[0]=Measureable("0",FPSTR(TEMPERATURE),FPSTR(MEASURE_CELSIUS_DEGREES),String(bme.readTemperature()));
		items[1]=Measureable("1",FPSTR(PRESSURE),FPSTR(MEASURE_PASCAL),String(bme.readPressure()));
		items[2]=Measureable("2",FPSTR(HUMIDITY),FPSTR(MEASURE_PERSENT),String(bme.readHumidity()));
		items[3]=Measureable("3",FPSTR(ALTITUDE),FPSTR(MEASURE_METER),String(bme.readAltitude(1013.25)));
	}



private:
	Adafruit_BME280 bme;
};



#endif /* LIBRARIES_MEASURER_BME180MEASURES_H_ */
