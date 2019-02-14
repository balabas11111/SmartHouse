/*
 * jsonSetGetAble.h
 *
 *  Created on: Feb 14, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_INTERFACES_JSONSETGETABLE_H_
#define LIBRARIES_DEVICELIB_INTERFACES_JSONSETGETABLE_H_

#include <ArduinoJson.h>

class JsonSetGetAble {
public:
	//sets object properties from JsonObject
	virtual int set(JsonObject& item)=0;
	//get object properties to Jsonobject
	virtual int get(JsonObject& item)=0;

	//
	virtual int setStatic(JsonObject& item){
		return set(item);
	}

	virtual int getStatic(JsonObject& item){
		return get(item);
	}

	//additional interface members. For possible set override
	virtual int setVal(JsonObject& item){
		return set(item);
	}

	virtual int getVal(JsonObject& item){
		return get(item);
	}
};

#endif /* LIBRARIES_DEVICELIB_INTERFACES_JSONSETGETABLE_H_ */
