/*
 * EntityJsonRequestResponse.h
 *
 *  Created on: Aug 23, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_ENTITYJSONREQUESTRESPONSE_H_
#define LIBRARIES_ENTITYLIBSIMPLE_ENTITYJSONREQUESTRESPONSE_H_

#include <ArduinoJson.h>
#include <JsonObjectUtil.h>

#define REQUEST "req"
#define RESPONSE "resp"

class EntityJsonRequestResponse {
private:
	DynamicJsonBuffer buf;

	JsonVariant request;
	JsonVariant response;
public:
	EntityJsonRequestResponse(){
		JsonObject& root= buf.createObject();

		request = root.createNestedObject(REQUEST);
		response = root.createNestedObject(RESPONSE);
	};
	virtual ~EntityJsonRequestResponse(){};

	void clear(){
		buf.clear();
	}

	JsonObject & getRequest(){
		return request;
	}

	JsonObject & getResponse(){
		return response;
	}

	void putRequestJsonParam(String str, const char* key){
		Serial.print(FPSTR("parse "));
		Serial.println(str);
		JsonObjectUtil::clone(request, str, key);

		Serial.print(FPSTR("-result "));
		request.printTo(Serial);
		Serial.println();
	}

	String& printResponseTo(String& str){
		response.printTo(str);
		return str;
	}

};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_ENTITYJSONREQUESTRESPONSE_H_ */
