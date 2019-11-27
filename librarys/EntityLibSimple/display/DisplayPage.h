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
#include "utils/JsonObjectUtil.h"
#include "utils/DeviceUtils.h"
#include <display/PageToDisplayAdapter.h>

#define DISPLAY_DATA "dd"
#define ERROR_KEY "Error"
#define NO_DATA "No Data"

class DisplayPage {
public:
	DisplayPage(Entity* entity, const char* fields[], int fieldsSize, const char* header = nullptr);
	DisplayPage(Entity* entity, const char* fields[], const char* fieldsDescr[], const char* fieldsMeasure[], int fieldsSize, const char* header = nullptr);
	virtual ~DisplayPage(){};

	virtual bool init(PageToDisplayAdapter* adapter);

	virtual void render();

	bool isInitialized();

	PageToDisplayAdapter* getAdapter();

protected:
	virtual void construct(Entity* entity, const char* fields[],
			const char* fieldsDescr[], const char* fieldsMeasure[], int fieldsSize,
			const char* header);
	virtual void fillData(JsonObject& from, JsonObject& to);
	virtual JsonObject& putDataToJson(JsonObject& parent, const char* key, const char* name, const char* measure = nullptr);
	PageToDisplayAdapter* adapter = nullptr;

	const char** fields;
	const char** fieldsDescr;
	const char** fieldsMeasure;
private:
	bool renderKey(const char* key);
	void setAndInitDisplayAdapter(PageToDisplayAdapter* adapter);

	Entity* entity;

	int fieldsSize;
	const char* header;

	bool initialized = false;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_DISPLAY_DISPLAYPAGE_H_ */
