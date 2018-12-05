/*
 * DisplayHelperPage.h
 *
 *  Created on: Dec 5, 2018
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_HELPERS_DISPLAYHELPERPAGE_H_
#define LIBRARIES_HELPERS_DISPLAYHELPERPAGE_H_

#include <DisplayHelper.h>
#include "Arduino.h"


class DisplayHelperPage {
public:
	virtual ~DisplayHelperPage(){};

	virtual String getName()=0;

	virtual uint8_t getItemCount()=0;

	virtual void printPage(DisplayHelper* helper)=0;
};

#endif /* LIBRARIES_HELPERS_DISPLAYHELPERPAGE_H_ */
