/*
 * BaseExtraBoxesList.h
 *
 *  Created on: Feb 13, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ESPSETTINGSBOX_EXTRABOXESLISTS_BASEEXTRABOXESLIST_H_
#define LIBRARIES_ESPSETTINGSBOX_EXTRABOXESLISTS_BASEEXTRABOXESLIST_H_

#include <Arduino.h>
#include <interfaces/ArrayList.h>
#include <ESPExtraSettingsBox.h>
#include <extraBoxes/ESPSett_Ntp.h>
#include <extraBoxes/ESPSett_Device.h>
#include <extraBoxes/ESPSett_ThSpeak.h>
#include <extraBoxes/ESPSett_Own.h>

class BaseExtraBoxesList: public ArrayList<ESPExtraSettingsBox> {
public:
	BaseExtraBoxesList();
	virtual ~BaseExtraBoxesList(){};

private:
	ESPSett_Ntp espSett_Ntp;
	EspSett_Device espSett_Dev;
	EspSett_ThSpeak espSett_Ths;
	EspSett_Own espSettOwn;
};

#endif /* LIBRARIES_ESPSETTINGSBOX_EXTRABOXESLISTS_BASEEXTRABOXESLIST_H_ */
