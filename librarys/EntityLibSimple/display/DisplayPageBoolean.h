/*
 * DisplayPageBoolean.h
 *
 *  Created on: 27 нояб. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_DISPLAY_DISPLAYPAGEBOOLEAN_H_
#define LIBRARIES_ENTITYLIBSIMPLE_DISPLAY_DISPLAYPAGEBOOLEAN_H_

#include "display/DisplayPage.h"
#include "utils/JsonObjectUtil.h"

class DisplayPageBoolean: public DisplayPage {
public:
	DisplayPageBoolean(Entity* entity, const char* fields[], const char* fieldsDescr[], int fieldsSize, const char* header = nullptr):
		DisplayPage(entity, fields, fieldsDescr, nullptr, fieldsSize, header){};
	virtual ~DisplayPageBoolean(){};
protected:

	virtual void fillData(JsonObject& from, JsonObject& to) override{
		if (from.size() > 0) {
			const char* key = this->fields[0];

			if (from.containsKey(key)) {
				int index = (JsonObjectUtil::getFieldBooleanValue(from, key))?1:0;

				JsonObject& dataEntityToDisplay = putDataToJson(to, key," ","");
				dataEntityToDisplay[COL_VALUE] = fieldsDescr[index];
			}
		}
	}


};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_DISPLAY_DISPLAYPAGEBOOLEAN_H_ */
