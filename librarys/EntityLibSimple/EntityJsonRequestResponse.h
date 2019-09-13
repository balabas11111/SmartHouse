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
	/*JsonVariant request;
	JsonVariant response;
	*/
public:
	EntityJsonRequestResponse(){
		this->root= buf.parse("{}").as<JsonObject>();
		/*this->request = getRoot().createNestedObject(REQUEST);
		this->response = getRoot().createNestedObject(RESPONSE);
		*/
	};
	virtual ~EntityJsonRequestResponse(){};

	void clear(){
		this->buf.clear();
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

};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_ENTITYJSONREQUESTRESPONSE_H_ */
