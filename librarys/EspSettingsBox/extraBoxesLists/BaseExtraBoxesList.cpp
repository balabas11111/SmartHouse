/*
 * BaseExtraBoxesList.cpp
 *
 *  Created on: Feb 13, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <extraBoxesLists/BaseExtraBoxesList.h>

BaseExtraBoxesList::BaseExtraBoxesList() {
	Serial.print(FPSTR("Construct BaseExtraBoxesList"));

	ESPExtraSettingsBox* extraBoxes[]={&espSettOwn, &espSett_Ntp,&espSett_Dev,&espSett_Ths};

	constructList(extraBoxes, ARRAY_SIZE(extraBoxes));

	Serial.print(FPSTR("...done size="));
	Serial.println(getSize());
}

