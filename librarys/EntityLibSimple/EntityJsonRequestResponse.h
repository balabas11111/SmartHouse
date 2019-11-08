/*
 * EntityJsonRequestResponse.h
 *
 *  Created on: Aug 23, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_ENTITYJSONREQUESTRESPONSE_H_
#define LIBRARIES_ENTITYLIBSIMPLE_ENTITYJSONREQUESTRESPONSE_H_

#include "Arduino.h"
#include <ArduinoJson.h>
#include <utils/JsonObjectUtil.h>

#define REQUEST "req"
#define RESPONSE "resp"
#define EMPTY_JSON_OBJECT "{}"

class EntityJsonRequestResponse {
private:
	DynamicJsonBuffer buf;

	JsonVariant root;
public:

	EntityJsonRequestResponse(){
		this->root= buf.parse("{}").as<JsonObject>();

		getRoot().createNestedObject(REQUEST);
		getRoot().createNestedObject(RESPONSE);
	};

	EntityJsonRequestResponse(char* json){
		this->root= buf.parse(json).as<JsonObject>();
	};

	virtual ~EntityJsonRequestResponse(){};

	DynamicJsonBuffer* getBuffer(){
		return &this->buf;
	}

	static EntityJsonRequestResponse* build(){
		return new EntityJsonRequestResponse();
	}

	static void destroy(EntityJsonRequestResponse* reqResp){
		delete reqResp;
	}

	void clear(){
		this->buf.clear();
	}

	void construct(){
		//Serial.print(FPSTR("Prepare buffer"));
		if(buf.size()>0){
			buf.clear();
		//Serial.print(FPSTR("..."));
			this->root = buf.parse("{}").as<JsonObject>();

			getRoot().createNestedObject(REQUEST);
			getRoot().createNestedObject(RESPONSE);

		//Serial.println(FPSTR("done"));
		}
	}

	JsonObject& getRoot(){
		return this->root.as<JsonObject>();
	}

	JsonObject& getRequest(){
		return getRoot().get<JsonObject>(REQUEST);
	}

	JsonObject& getResponse(){
		return getRoot().get<JsonObject>(RESPONSE);
	}

	void getResponseAsString(String& str){
		getResponse().printTo(str);
	}

	void print(){
		JsonObjectUtil::print(root);
	}

	void addRequestParam(char* key, char* value){
		JsonObject& param = getRequest();
		if(key==nullptr || value == nullptr){
			return;
		}
		/*Serial.print(FPSTR("Add "));
		Serial.print(key);
		Serial.print(FPSTR(" ="));
		Serial.println(value);
*/
		param.set(key, value);
	}

	void putRequestJsonParam(String str, const char* key){
		Serial.print(FPSTR("parse "));
		Serial.println(str);
		JsonObjectUtil::clone(getRequest(), str, key);

		Serial.print(FPSTR("-result "));
		getRequest().printTo(Serial);
		Serial.println();
	}

	String& printResponseTo(String& str){
		getResponse().printTo(str);
		return str;
	}

	JsonObject& createNestedObject(const char* key){
		return getRoot().createNestedObject(key);
	}

};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_ENTITYJSONREQUESTRESPONSE_H_ */
