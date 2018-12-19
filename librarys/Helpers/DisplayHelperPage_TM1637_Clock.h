/*
 * DisplayHelperPage.h
 *
 *  Created on: Dec 5, 2018
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_HELPERS_DisplayHelperPage_TM1637_Clock_H_
#define LIBRARIES_HELPERS_DisplayHelperPage_TM1637_Clock_H_

#include <DisplayHelper.h>
#include "Arduino.h"
#include "NtpTimeClientService.h"
#include "DisplayHelperPage.h"


class DisplayHelperPage_TM1637_Clock:public DisplayHelperPage {
public:
	DisplayHelperPage_TM1637_Clock(NtpTimeClientService* timeClient){
		this->timeClient=timeClient;
		lasttime=millis();
	}
	virtual ~DisplayHelperPage_TM1637_Clock(){};

	String getName(){
		return FPSTR("TM1637_Clock");
	}

	uint8_t getItemCount(){
		return 1;
	}

	virtual boolean loop(DisplayHelper* helper){
		if(startBlink==false && timeClient->isTimeReceived){
			startBlink=true;
			lasttime=millis();
		}

		if(lasttime+1000<millis()){
			if(timeClient->isTimeReceived()){
				dotVal=!dotVal;
				helper->displayActivity(dotVal);

				int8_t* time=timeClient->getCurrentTime();
				helper->displayLine(time, 4);
			}
		}
		return false;
	};


protected:
	boolean displayCurrentView(DisplayHelper* helper){
		if(!timeClient->isTimeReceived){
			displayLoad(helper);
			return false;
		}

		int8_t time[4]=timeClient->getCurrentTime();

		helper->displayLine(time, 4);

		return true;
	}
private:
	NtpTimeClientService* timeClient;
	long lasttime;
	boolean startBlink=false;
	boolean dotVal=false;

	void displayLoad(DisplayHelper* helper){
		int8_t data[4]={1,0,10,13};
		helper->displayLine(data, 4);
	}

	void refreshDot(){

	}
};

#endif /* LIBRARIES_HELPERS_DISPLAYHELPERPAGE_H_ */
