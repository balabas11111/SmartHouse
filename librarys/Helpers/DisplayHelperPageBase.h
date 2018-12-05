/*
 * DisplayHelperPage.h
 *
 *  Created on: Dec 5, 2018
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_HELPERS_DISPLAYHELPERPAGEBASE_H_
#define LIBRARIES_HELPERS_DISPLAYHELPERPAGEBASE_H_

#include "Arduino.h"
#include "EspSettingsBox.h"
#include "DisplayHelper.h"
#include "DisplayHelperPage.h"


class DisplayHelperPageBase:public DisplayHelperPage {
public:
	DisplayHelperPageBase(EspSettingsBox* espSettingsBox){
		this->espSettingsBox=espSettingsBox;
	}
	virtual ~DisplayHelperPageBase(){};

	String getName(){
		return FPSTR("espSettingsBox");
	}

	uint8_t getItemCount(){
		return 0;
	}

	void printPage(DisplayHelper* helper){
		if(!espSettingsBox->isAccesPoint){

			String str="http://"+WiFi.localIP().toString();
			helper->displayLine(str,0,0);

			helper->displayLine(espSettingsBox->DeviceLocation, 1, 0);

			String(helper->getTotalDetectedSensors())+" sensors";
			helper->displayLine(str,4,0);

			str=" on ("+String(helper->getTotalPages())+") pages";

			helper->displayLine(str,5,0);
		}else{
			String ipLine="IP="+WiFi.softAPIP().toString();

			Serial.println(WiFi.softAPIP());
			Serial.println(ipLine);

			helper->displayLine("SETUP MODE",0,0);
			helper->displayLine("CONNECT AND SET",1,0);
			helper->displayLine(espSettingsBox->ssidAP,2,0);
			helper->displayLine(ipLine,4,0);
			helper->displayLine("PORT=80",6,0);
		}
	}
private:
	EspSettingsBox* espSettingsBox;
};

#endif /* LIBRARIES_HELPERS_DISPLAYHELPERPAGE_H_ */
