/*
 * JsonObjectUtil.h
 *
 *  Created on: 24 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_UTILS_JSONOBJECTUTIL_H_
#define LIBRARIES_DEVICELIB_UTILS_JSONOBJECTUTIL_H_

#include <Arduino.h>
#include <ArduinoJson.h>

#include <utility>
#include <CompareUtils.h>

class JsonObjectUtil {
public:
	JsonObjectUtil();
	virtual ~JsonObjectUtil();

	template<typename T>
	static bool copyField(JsonObject& from,JsonObject& to,const char* key,bool overwrite){
		if(!from.containsKey(key)){
			return false;
		}
		if(!to.is<T>(key)){
			if(overwrite){
				to.set(key, from.get<T>(key));
				return true;
			}else{
				return false;
			}
		}else{
			if(overwrite){
				to.set(key, from.get<T>(key));
				return true;
			}else{
				T valf = from.get<T>(key);
				T valt = to.get<T>(key);

				if(CompareUtils::compareValues<T>(valf,valt)){
					return false;
				}else{
					to.set(key, valf);
					return true;
				}
			}
		}

	}

	static void print(const char* head,JsonVariant json){
		Serial.println();
		Serial.println(head);
		json.printTo(Serial);
		Serial.println();
	}

	static void print(JsonVariant json){
		Serial.println();
		json.printTo(Serial);
		Serial.println();
	}

	static void printKeyVal(const char* const& key,JsonVariant value){
		Serial.print(key);
		Serial.print(FPSTR("="));
		if(value.is<JsonObject>() || value.is<JsonArray>()){
			value.printTo(Serial);
		}else{
			if(value.is<char*>() || value.is<const char*>()){
				Serial.print(value.as<char*>());
			}else if(value.is<int>()){
				Serial.print(value.as<int>());
			}else if(value.is<float>()){
				Serial.print(value.as<float>());
			}
		}
		Serial.println(FPSTR(";"));
	}

	static JsonVariant clone(JsonVariant jb, JsonVariant prototype, const char* protoKey)
	{
		Serial.println(FPSTR("Prototype="));
		prototype.printTo(Serial);
		Serial.println();
		Serial.println(FPSTR(" jb="));
		jb.printTo(Serial);
		Serial.println();
		Serial.println(FPSTR("..."));

		if(jb.is<JsonObject>()){
			JsonObject& target = jb.as<JsonObject>();
			bool hasKey = target.containsKey(protoKey);

			if (prototype.is<JsonObject>()) {
				const JsonObject& protoObj = prototype;
				JsonObject& newObj = (!hasKey)?target.createNestedObject(protoKey):target.get<JsonObject>(protoKey);
				for (const auto& kvp : protoObj) {
				  newObj[strdup(kvp.key)] = clone(newObj, kvp.value,kvp.key);
				}
				return newObj;
		    }

			if (prototype.is<JsonArray>()) {
				const JsonArray& protoArr = prototype;
				JsonArray& newArr = (!hasKey)?target.createNestedArray(protoKey):target.get<JsonArray>(protoKey);
				for (const auto& elem : protoArr) {
				  newArr.add(clone(newArr, elem,NULL));
				}
				return newArr;
		    }

			if (prototype.is<char*>()) {
				  target.set(protoKey, prototype.as<const char*>());
				return target.get<const char*>(protoKey);
			  }

		}

		if(jb.is<JsonArray>()){
			JsonArray& target = jb.as<JsonArray>();

			target.add(prototype);
			return prototype;
		}

	  return prototype;
	}

	static bool copyArray(JsonArray& a,JsonArray& b){
		bool result=false;

		for (const auto& kvp : a) {
			if(!isInArray(kvp, b)){
				Serial.print(FPSTR("add "));
				Serial.println(kvp.as<char*>());

				b.add(strdup(kvp));
			}
		}

		return result;
	}

	static bool isInArray(const char* key,JsonArray& a){
		bool result=false;

		for (const auto& kvp : a) {
			if(strcmp(kvp.as<const char*>(),key)==0){
				return true;
			}

		}
		return result;
	}


};

#endif /* LIBRARIES_DEVICELIB_UTILS_JSONOBJECTUTIL_H_ */
