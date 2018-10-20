/*
 * WidgetHelper.h
 *
 *  Created on: 20 окт. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_HELPERS_WIDGETHELPER_H_
#define LIBRARIES_HELPERS_WIDGETHELPER_H_

#include "HtmlWidget.h"
#include "EspSettingsBox.h"

class WidgetHelper {
public:
	WidgetHelper();
	virtual ~WidgetHelper();

	static String getWidgetsJson(EspSettingsBox *espSettingsBox,HtmlWidget* widgetsArray[],uint8_t size){
		String result="{\"deviceId\":\""+espSettingsBox->DeviceId+"\",\"count\":"+String(size)+
							",\"widgets\":[";
		for(uint8_t i=0;i<size;i++){
			result+=widgetsArray[i]->getJson();
			if(i!=size-1){
				result+=",";
			}
		}
		result+="]}";
		Serial.println("-------------Widgets--------------------------");
		Serial.println(result);
		Serial.println("----------------------------------------------");
		return result;
	}

	static String getHtmlWidgetHtml(EspSettingsBox *espSettingsBox,String wiName,HtmlWidget* widgetsArray[],uint8_t size){
		String result="";

			Serial.println("PorcessWidget="+wiName);

			if(wiName.equals("espSettingsBox.DeviceId")){
				return espSettingsBox->DeviceId;
			}

			for(uint8_t i=0;i<size;i++){
				if(widgetsArray[i]->getName().equals(wiName)){
					return widgetsArray[i]->getHtml();
				}
			}
			return result;
	}
};
#endif /* LIBRARIES_HELPERS_WIDGETHELPER_H_ */
