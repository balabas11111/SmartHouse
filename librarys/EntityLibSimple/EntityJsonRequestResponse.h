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

	JsonVariant root;
	JsonVariant request;
	JsonVariant response;
public:
	EntityJsonRequestResponse(){
		root= buf.parse("{}").asObject();
		request = root.asObject().createNestedObject(REQUEST);
		response = root.asObject().createNestedObject(RESPONSE);
/*
		Serial.println(FPSTR("New ENtityJsonRequestResponse created"));

		Serial.println(FPSTR("Create request section"));

		Serial.println(FPSTR("Create response section"));


		JsonObjectUtil::print(root);
		Serial.println(FPSTR("New ENtityJsonRequestResponse done"));
		*/
	};
	virtual ~EntityJsonRequestResponse(){};

	void clear(){
		buf.clear();
	}

	JsonObject& getRoot(){
		return root.asObject();
	}

	JsonObject& getRequest(){
		return root.asObject().get<JsonObject>(REQUEST);
	}

	JsonObject& getResponse(){
		return root.asObject().get<JsonObject>(RESPONSE);
	}

	void getResponseAsString(String& str){
		getResponse().printTo(str);
	}

	void print(){
		JsonObjectUtil::print(root);
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
