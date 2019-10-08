/*
 * DisplayPage.h
 *
 *  Created on: Oct 8, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_DISPLAY_DISPLAYPAGE_H_
#define LIBRARIES_ENTITYLIBSIMPLE_DISPLAY_DISPLAYPAGE_H_

#include "Arduino.h"
#include "ArduinoJson.h"
#include "Entity.h"
#include "functional"
#include "EntityJsonRequestResponse.h"
#include "JsonObjectUtil.h"
#include "DeviceUtils.h"
#include <display/PageToDisplayAdapter.h>

#define DISPLAY_DATA "dd"
#define ERROR_KEY "Error"
#define NO_DATA "No Data"

class DisplayPage {
public:
	DisplayPage(Entity* entity, const char* fields[], int fieldsSize, const char* header = nullptr);
	virtual ~DisplayPage(){};

	virtual bool init();

	virtual void render(PageToDisplayAdapter* adapter);

	bool isInitDone();

private:
	bool renderKey(const char* key);

	Entity* entity;
	const char** fields;
	int fieldsSize;
	const char* header;

	bool initDone = false;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_DISPLAY_DISPLAYPAGE_H_ */
