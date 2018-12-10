/*
 * DisplayHelperPage.h
 *
 *  Created on: Dec 5, 2018
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_HELPERS_DISPLAYHELPERPAGE_H_
#define LIBRARIES_HELPERS_DISPLAYHELPERPAGE_H_

#include <Display/DisplayHelper.h>
#include "Arduino.h"


class DisplayHelperPage {
public:
	virtual ~DisplayHelperPage(){};

	virtual String getName()=0;

	virtual uint8_t getItemCount()=0;

	void printPage(DisplayHelper* helper){
		currentView=0;
		displayCurrentView(helper);
	}

	boolean switchViewDown(DisplayHelper* helper){
		if(totalViews==1 || currentView==0){
			return false;
		}else{
			currentView--;
			return displayCurrentView(helper);
		}
	}

	boolean switchViewUp(DisplayHelper* helper){
		if(totalViews==1 || currentView==totalViews-1){
			return false;
		}else{
			currentView++;
			return displayCurrentView(helper);
		}
	}

	virtual boolean loop(){
		return false;
	};
protected:
	virtual boolean displayCurrentView(DisplayHelper* helper){
		helper->getTotalPages();
		return false;
	}

private:
	uint8_t currentView=0;
	uint8_t totalViews=1;
};


#endif /* LIBRARIES_HELPERS_DISPLAYHELPERPAGE_H_ */
