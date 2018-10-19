/*
 * Bme180Measures.h
 *
 *  Created on: 14 îêò. 2018 ã.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_MEASURER_BH1750_MEASURER_H_
#define LIBRARIES_MEASURER_BH1750_MEASURER_H_

#include "Arduino.h"
#include "Measurer.h"
#include "HtmlWidget.h"

#include "BH1750.h"

const char MEASURER_BH1750_NAME[] PROGMEM ="BH1750_Sensor";
const char MEASURER_BH1750_DESCRIPTION[] PROGMEM ="LuxMeter";
const char MEASURER_BH1750_DESCRIPTION_RU[] PROGMEM ="Освещенность BH1750";

class BH1750_Measurer: public Measurer, public HtmlWidget{

public:

	BH1750_Measurer(String _id,String _name)
			: Measurer(_id, _name, FPSTR(MEASURER_BH1750_NAME),FPSTR(MEASURER_BH1750_DESCRIPTION),FPSTR(MEASURER_BH1750_DESCRIPTION_RU), 1,false){
	}

	~BH1750_Measurer(){
		Serial.println("destructed");
	}

	boolean init(){
		Serial.print("Init luxmeter");
		lightMeter.begin();
		Serial.println("...done");
		initialized=true;
		return true;
	}

	void getExternal() override{
		items[0]=Measureable("light","Освещенность","Lux",String(lightMeter.readLightLevel()),"Люкс");
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
					result+=getFormRow(items[0].descr,(items[0].name+" "+items[0].kind),items[0].val);
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
	BH1750 lightMeter;;
};



#endif /* LIBRARIES_MEASURER_BH1750_MEASURER_H_ */
