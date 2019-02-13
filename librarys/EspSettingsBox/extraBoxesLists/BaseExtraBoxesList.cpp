/*
 * BaseExtraBoxesList.cpp
 *
 *  Created on: Feb 13, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <extraBoxesLists/BaseExtraBoxesList.h>

BaseExtraBoxesList::BaseExtraBoxesList() {

}

void BaseExtraBoxesList::construct() {
	Serial.print(FPSTR("Construct BaseExtraBoxesList"));

	ESPExtraSettingsBox* extraBoxes[]={&espSett_Own, &espSett_Ntp,&espSett_Dev,&espSett_Ths};

	boolean ok=true;

	for(uint8_t i=0;i<ARRAY_SIZE(extraBoxes);i++){
		ok=ok && checkNonNull(extraBoxes[i]);
	}

	if(ok){
		constructList(extraBoxes, ARRAY_SIZE(extraBoxes));

		Serial.print(FPSTR("...done size="));
		Serial.print(getSize());
		Serial.print(FPSTR(" boxes("));

		for(uint8_t i=0;i<getSize();i++){
			Serial.print(getItem(i)->getName());
			Serial.print(FPSTR(" "));
		}
		Serial.println(FPSTR(")"));
	}
}
