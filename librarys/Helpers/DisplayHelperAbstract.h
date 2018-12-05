/*
 * DisplayHelper.h
 *
 *  Created on: 20 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DISPLAYHELPER_DISPLAYHELPERBASE_H_
#define LIBRARIES_DISPLAYHELPER_DISPLAYHELPERBASE_H_

#include <DisplayHelper.h>
#include <DisplayHelperPage.h>
#include "Arduino.h"
#include "EspSettingsBox.h"
#include <Initializable.h>
#include <TimeTrigger.h>

#define TURN_OFF_SECS 15

class DisplayHelperAbstract: public Loopable,public DisplayHelper {

public:
	DisplayHelperAbstract(boolean){
		construct(nullptr,0,espSettingsBox,nullptr,nullptr,0,0,0,false,false);
	}

	DisplayHelperAbstract(EspSettingsBox* espSettingsBox){
		construct(nullptr,0,espSettingsBox,nullptr,nullptr,0,0,0,false,false);
	}

	virtual ~DisplayHelperAbstract(){};

	virtual boolean initialize(DisplayHelperPage** pages,uint8_t itemsCount,boolean _init){

		initialized=_init;

		if(_init){
			this->pages= pages;
			this->itemsCount=itemsCount;
			totalPages=itemsCount+1;

			initialized=handleDisplayBegin();

			displayDeviceInitPage();
			initTimeTriggers();

			initSensorsCount();
		}

		return initialized;
	}

	virtual boolean turnOnOffDisplay(boolean on){
		if(!initialized){
			return false;
		}
		preprocessDisplayPower(on);

		if(!displayOn){
			return false;
		}

		return handleDisplayPower();
	}

	virtual boolean loop(){
		if(!initialized){
			return false;
		}
		boolean result=false;

		if(turnOffTrigger!=nullptr){
			result= turnOffTrigger->loop() || result;
		}

		if(switchPageTrigger!=nullptr){
			result= switchPageTrigger->loop() || result;
		}

		return result;
	}

	virtual boolean clearDisplay(){
		if(!initialized){
			return false;
		}
		return handleDisplayClear();
	}

	virtual boolean addStringToDisplay(String str,int row,int col,String sender){
		if(!initialized){
			return false;
		}
		return handleDisplayLine(str,row,col,sender);
	}

	virtual void printCurrentPage(){
		if(!initialized){
			return;
		}
		clearDisplay();

		displayPage(currentpage-1);
	}

	virtual boolean displayLine(String str,int row,int col){
		if(!initialized){
			return false;
		}
		return handleDisplayLine(str,row,col,"");
	}

	virtual uint8_t getStartRow(){
		return 3;
	}

	virtual uint8_t getRowIncrement(){
		return 1;
	}

	uint8_t getTotalDetectedSensors(){
		return totalDetectedSensors;
	}

	uint8_t getTotalPages(){
		return totalPages;
	}

private:
	DisplayHelperPage** pages;
	uint8_t itemsCount;
	EspSettingsBox* espSettingsBox;

	TimeTrigger* turnOffTrigger;
	TimeTrigger* switchPageTrigger;
	//0 base info page,2++ abstract itemspages
	uint8_t totalPages;
	uint8_t currentpage;

	uint8_t totalDetectedSensors;
	boolean displayOn;
	boolean initialized;

	void construct(DisplayHelperPage** pages,uint8_t itemsCount,EspSettingsBox* espSettingsBox,
				TimeTrigger* turnOffTrigger,TimeTrigger* switchPageTrigger,uint8_t totalPages,
				uint8_t currentpage,uint8_t totalDetectedSensors,boolean displayOn,boolean initialized){
		this->pages=pages;
		this->itemsCount=itemsCount;
		this->espSettingsBox=espSettingsBox;
		this->turnOffTrigger=turnOffTrigger;
		this->switchPageTrigger=switchPageTrigger;
		this->totalPages=totalPages;
		this->currentpage=currentpage;
		this->totalDetectedSensors=totalDetectedSensors;
		this->displayOn=displayOn;
		this->initialized=initialized;
	}

protected:
//--------------------------------------------
//override this for specified display
	virtual boolean handleDisplayBegin(){
		return true;
	}

	virtual boolean handleDisplayClear(){
		Serial.println(FPSTR("---display cleared"));

		return true;
	}

	virtual boolean handleDisplayPower(){
		Serial.print(FPSTR("--display power="));Serial.println(displayOn);

		return displayOn;
	}

	virtual boolean handleDisplayLine(String str,int row,int col,String sender){
		Serial.print(FPSTR("---display r="));Serial.print(row);
		Serial.print(FPSTR(" c="));Serial.print(col);
		Serial.print(FPSTR(" str="));Serial.print(str);
		Serial.print(FPSTR(" sender="));Serial.println(sender);
		return true;
	}
//-------------------------------------------

	virtual void displayDeviceInitPage(){
		clearDisplay();

		displayLine("Balabas-soft",1,0);
		displayLine("Loading....",3,0);
		displayLine("Hold Button to",5,0);
		displayLine("reset device",6,0);

		displayOn=true;

		delay(1000);

		startTimeTriggers();
	}

	virtual void displayPage(uint8_t index){
		pages[index]->printPage(this);
	}

	virtual void preprocessDisplayPower(boolean on){
		displayOn=on;

		if(espSettingsBox->displayAlvaysOn){
			displayOn=true;
		}

		if(turnOffTrigger!=nullptr){
			if(displayOn){
				turnOffTrigger->stop();
			}else{
				turnOffTrigger->start();
			}
		}
	}

	virtual boolean initTimeTriggers(){
		boolean result=false;

		if(espSettingsBox!=nullptr){
			if(!espSettingsBox->displayAlvaysOn){
				turnOffTrigger=new TimeTrigger(0,TURN_OFF_SECS*1000,false,[this](){turnOnOffDisplay(false);});
			}
			if(espSettingsBox->displayAlvaysOn && !espSettingsBox->displayAutochange!=0){
				switchPageTrigger=new TimeTrigger(0,espSettingsBox->displayAutochange,false,[this](){switchPageUp();});
			}
		}

		return result;
	}

	virtual void startTimeTriggers(){
		if(turnOffTrigger!=nullptr){
			turnOffTrigger->start();
		}
		if(switchPageTrigger!=nullptr){
			switchPageTrigger->start();
		}
	}

	virtual void initSensorsCount(){
		totalDetectedSensors=0;

		for(uint8_t i=0;i<itemsCount;i++){
			totalDetectedSensors+=pages[i]->getItemCount();
		}
	}

	virtual void switchPageUp(){
		if(currentpage>=totalPages-1){
			currentpage=0;
		}else{
			currentpage++;
		}

		Serial.print(FPSTR("--display currentpage="));Serial.println(currentpage);
	}

};

#endif /* LIBRARIES_DISPLAYHELPER_DISPLAYHELPER_H_ */