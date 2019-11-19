/*
 * PageRenderer.h
 *
 *  Created on: Oct 8, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_DISPLAY_PAGETODISPLAYADAPTER_H_
#define LIBRARIES_ENTITYLIBSIMPLE_DISPLAY_PAGETODISPLAYADAPTER_H_

#include "Arduino.h"
#include "ArduinoJson.h"

#ifndef DISPLAY_TURN_OFF_TIME
#define DISPLAY_TURN_OFF_TIME 30000
#endif

#define COL_KEY     "k"
#define COL_NAME    "n"
#define COL_VALUE   "v"
#define COL_MEASURE "m"

class PageToDisplayAdapter {
public:
	PageToDisplayAdapter(){};
	virtual ~PageToDisplayAdapter(){};

	virtual bool init();

	virtual void renderPage(const char* header, JsonVariant pageData);

	virtual bool isPowerOn();

	virtual void setPowerOn(bool powerOn = true);
	virtual void clear(){};

	virtual void loop();

	bool isInitialized();

protected:
	virtual void toStartPosition() {
		yPosition = 0;
	}
	virtual void print(const char* str){
		if(str!=nullptr) {
			Serial.print(str);
		}
	};
	virtual void printNextLine(){
		yPosition ++;
		Serial.println();
	};

	virtual bool initHardware(){
		return true;
	}

	virtual void turnOnHardware(){

	}
	virtual void turnOffHardware(){
		this->clear();
	}

	bool initialized = false;
	int yPosition = 0;
private:

	bool powerOn = false;

	unsigned long turnOnTime=0;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_DISPLAY_PAGETODISPLAYADAPTER_H_ */
