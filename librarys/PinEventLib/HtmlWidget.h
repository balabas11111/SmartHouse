/*
 * HtmlWidget.h
 *
 *  Created on: Oct 18, 2018
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_PINEVENTLIB_HTMLWIDGET_H_
#define LIBRARIES_PINEVENTLIB_HTMLWIDGET_H_

#define UNUSED(x) do{(void)x;}while(0)

#include "Arduino.h"
#include <ArduinoJson.h>
#include <ESP_Consts.h>

const PROGMEM char WIDGET_NOT_ALLOWED[]               = "ACTION NOT ALLOWWED";

struct HtmlWidgetAction {
  String actionName;
  String remoteId;
  String remoteVal;
  String className;
  String childClass;
  String clientData;
  boolean valid;
};

class HtmlWidget {
public:

	virtual ~HtmlWidget(){};

	virtual String printCommand(String actionName,String remoteId,String remoteVal, String className, String childClass,String clientData){
		Serial.println("Process actionName="+actionName+" remoteId="+remoteId+" remoteVal="+remoteVal+" className="+className
						+" childClass="+childClass);
		Serial.println(" clientData="+clientData);
		return "";
	}

	virtual boolean isTargetOfAction(String actionName,String remoteId,String remoteVal, String className, String childClass,String clientData){

		UNUSED(remoteVal);
		UNUSED(className);
		UNUSED(childClass);
		UNUSED(clientData);

		if(getName()!=remoteId){
			return false;
		}
/*
		if(actionName!=FPSTR(ACTION_GET_WIDGETS_CHILDREN_AS_JSON)
			||actionName!=FPSTR(ACTION_GET_WIDGET_HTML_OR_VAL)
			||actionName!=FPSTR(ACTION_GET_WIDGET_JSON)){
			return false;
		}
*/
		return true;
	}

	virtual String getNotAllowed(){return FPSTR(WIDGET_NOT_ALLOWED);};

	virtual String executeClientAction(String actionName,String remoteId,String remoteVal, String className, String childClass,String clientData)=0;

	virtual String getName()=0;

protected:

};

#endif /* LIBRARIES_PINEVENTLIB_HTMLWIDGET_H_ */
