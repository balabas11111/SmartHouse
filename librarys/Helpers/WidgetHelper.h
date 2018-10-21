/*
 * WidgetHelper.h
 *
 *  Created on: 20 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_HELPERS_WIDGETHELPER_H_
#define LIBRARIES_HELPERS_WIDGETHELPER_H_

#include "HtmlWidget.h"
//#include "HtmlWidgetAction.h"
#include "ESP_Consts.h"
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

	static String executeClientAction(EspSettingsBox *espSettingsBox,HtmlWidget* widgetsArray[],uint8_t size,
		String actionName,String remoteId,String remoteVal, String className, String childClass,String clientData){

		Serial.println("Process actionName="+actionName+" remoteId="+remoteId+" remoteVal="+remoteVal+" className="+className
				+" childClass="+childClass);
		Serial.println(" clientData="+clientData);

		String result="";

		//TODO: create espSettings htmlWidget
		String espSettAction=FPSTR(ACTION_GET_STATIC_SETTINGS_DATA);

		if(actionName.equals(espSettAction)){
			result=espSettingsBox->getParamVal(remoteVal);
			Serial.println("remoteVal("+remoteVal+","+result+")");
			return result;
		}

		for(uint8_t i=0;i<size;i++){
			if((widgetsArray[i]->getName()).equals(remoteId)){
				return widgetsArray[i]->executeClientAction(actionName, remoteId, remoteVal, className, childClass, clientData);
			}
		}

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
