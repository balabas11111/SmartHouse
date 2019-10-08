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
#define DISPLAY_TURN_OFF_TIME 15000
#endif

class PageToDisplayAdapter {
public:
	PageToDisplayAdapter(){};
	virtual ~PageToDisplayAdapter(){};

	virtual bool init(){return true;};

	virtual void renderPage(const char* header, JsonVariant pageData);

	virtual bool isPowerOn();

	virtual void setPowerOn(bool powerOn = true);
	virtual void clear(){};

	virtual void loop();
private:
	bool initialized = false;
	bool powerOn = false;

	unsigned long turnOnTime=0;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_DISPLAY_PAGETODISPLAYADAPTER_H_ */
