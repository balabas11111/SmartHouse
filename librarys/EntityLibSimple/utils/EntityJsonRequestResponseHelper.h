/*
 * EntityJsonRequestResponseHelper.h
 *
 *  Created on: 13 ����. 2019 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_UTILS_ENTITYJSONREQUESTRESPONSEUTIL_H_
#define LIBRARIES_ENTITYLIBSIMPLE_UTILS_ENTITYJSONREQUESTRESPONSEUTIL_H_

#include <Arduino.h>
#include <ArduinoJson.h>

#include <EntityJsonRequestResponse.h>

class EntityJsonRequestResponseUtil {
public:
	EntityJsonRequestResponseUtil(){};
	virtual ~EntityJsonRequestResponseUtil(){};

	static EntityJsonRequestResponse* create(){
		return new EntityJsonRequestResponse();
	}

	static void drop(EntityJsonRequestResponse* json){
		delete json;
	}

};

#endif /* LIBRARIES_UTILS_ENTITYJSONREQUESTRESPONSEHELPER_H_ */
