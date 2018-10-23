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

	boolean initialize(boolean _init){
		if(_init){
			Serial.println("-------BH1750_Measurer initialize------------");
			Serial.print("Init luxmeter");
			lightMeter.begin();
			Serial.println("...done");
			Serial.println("---------------------------------------------");
			measure();
		}
		initialized=_init;
		return initialized;
	}

	void getExternal() override{
		items[0]=Measureable("light","Освещенность","Lux",String(lightMeter.readLightLevel()),"Люкс");
	}
	//------------------------HtmlWidgetProcessing---------------------------

		String getName(){
			return Measurer::getName();
		}

		String executeClientAction(String actionName,String remoteId,String remoteVal, String className, String childClass,String clientData){

			printCommand(actionName, remoteId, remoteVal, className, childClass, clientData);

			if(actionName.equals(ACTION_GET_WIDGET_HTML_OR_VAL)
					&&className.equals(CLASS_REFRESHABLE_MeasurerWidgetESP)){

				return FPSTR(HTML_BH1750);
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
	BH1750 lightMeter;;
};



#endif /* LIBRARIES_MEASURER_BH1750_MEASURER_H_ */
