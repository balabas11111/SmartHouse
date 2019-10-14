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
#include <HashPrint.h>

#include <utility>
#include <CompareUtils.h>

const int ROOT_PATHS_TOTAL PROGMEM = 2;

const char* const ROOT_PATHS[] PROGMEM = {
		"model",
		"deploy"
};

const char* const ROOT_PATHS_GROUPS[] PROGMEM = {
		"sensors",
		"settings",
		"services"
};

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

	static void print(const char* head,JsonVariant json,boolean plnBefore=true){
		if(plnBefore){Serial.println();}
		Serial.println(head);
		json.printTo(Serial);
		Serial.println();
	}

	static void print(const char* head,JsonVariant descriptor,JsonVariant loaded,JsonVariant current){
		Serial.println();
		Serial.println(head);
		Serial.println(FPSTR("Descriptor="));
		descriptor.printTo(Serial);
		Serial.println();
		Serial.println(FPSTR("loaded="));
		loaded.printTo(Serial);
		Serial.println();
		Serial.println(FPSTR("current="));
		current.printTo(Serial);
		Serial.println();
	}

	static void print(JsonVariant json){
		Serial.println();
		json.printTo(Serial);
		Serial.println();
	}

	static void printWithPreffix(const char* const& preffix,JsonVariant value){
		Serial.print(preffix);
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

	static void printAllJson(JsonObject& root){
		Serial.println(FPSTR("-------------------ROOT-------------------"));

		for(int i=0;i<ROOT_PATHS_TOTAL;i++){
			const char* currRoot = ROOT_PATHS[i];

			print(currRoot,root.get<JsonObject>(currRoot));
		}
	}

	static void clone(JsonVariant to,String& prototype, const char* key = ""){
		DynamicJsonBuffer tmp;

		JsonObjectUtil::clone(to, tmp.parse(prototype).as<JsonObject>(), key);
		tmp.clear();
	}

	static JsonVariant clone(JsonVariant to, JsonVariant prototype, const char* protoKey = "")
	{
		/*Serial.println(FPSTR("Prototype="));
		prototype.printTo(Serial);
		Serial.println();
		Serial.println(FPSTR(" jb="));
		to.printTo(Serial);
		Serial.println();
		Serial.println(FPSTR("..."));
*/
		if(to.is<JsonObject>()){
			JsonObject& target = to.as<JsonObject>();

			bool keyEmpty = strcmp(protoKey,"")==0;
			bool hasKey = keyEmpty?true:target.containsKey(protoKey);

			if (prototype.is<JsonObject>()) {
				const JsonObject& protoObj = prototype;
				JsonObject& newObj = getJsonTargetAsObject(keyEmpty,hasKey,protoKey, target);

				for (const auto& kvp : protoObj) {
				  newObj[strdup(kvp.key)] = clone(newObj, kvp.value,kvp.key);
				}
				return newObj;
		    }

			if (prototype.is<JsonArray>()) {
				const JsonArray& protoArr = prototype;
				JsonArray& newArr = getJsonTargetAsArray(keyEmpty,hasKey,protoKey, target);
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

		if(to.is<JsonArray>()){
			JsonArray& target = to.as<JsonArray>();

			target.add(prototype);
			return prototype;
		}

	  return prototype;
	}

	static JsonObject& getJsonTargetAsObject(bool keyEmpty, bool hasKey, const char* protoKey, JsonObject& target){
		if(keyEmpty){
			return target;
		}

		return (!hasKey)?target.createNestedObject(protoKey):target.get<JsonObject>(protoKey);
	}

	static JsonArray& getJsonTargetAsArray(bool keyEmpty, bool hasKey, const char* protoKey, JsonVariant target){
		if(keyEmpty){
			return target;
		}

		return (!hasKey)?target.as<JsonObject>().createNestedArray(protoKey):target.as<JsonObject>().get<JsonArray>(protoKey);
	}

	static int copyArray(JsonArray& a,JsonArray& b){
		int result=0;

		for (const auto& kvp : a) {
			if(!isInArray(kvp, b)){
				result++;
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

	static int getIndexOfKeyInObject(JsonObject& obj,const char* key){
		int ind=0;
		for (const auto& kvp : obj) {
			if(strcmp(kvp.key,key)==0){
				return ind;
			}
			ind++;
		}
		return -1;
	}

	static String getObjectValueAsString(JsonObject& obj,const char* keyName){
		String str=FPSTR("key/NA");

		if(obj.is<char*>(keyName)){
			str = obj.get<char*>(keyName);
		}else if(obj.is<const char*>(keyName)){
			str = obj.get<const char*>(keyName);
		}else if(obj.is<bool>(keyName)){
			str = obj.get<bool>(keyName);
		}else if(obj.is<short>(keyName)){
			str = obj.get<short>(keyName);
		}else if(obj.is<int>(keyName)){
			str = obj.get<int>(keyName);
		}else if(obj.is<long>(keyName)){
			str = obj.get<long>(keyName);
		}else if(obj.is<float>(keyName)){
			str = obj.get<float>(keyName);
		}

		return str;
	}

	static JsonObject& getObjectChildOrCreateNew(JsonObject& obj,String key){
		return (obj.containsKey(key))?obj.get<JsonObject>(key):obj.createNestedObject(key);
	}

	static JsonObject& getObjectChildOrCreateNew(JsonObject& obj,const char* key){
		return (obj.containsKey(key))?obj.get<JsonObject>(key):obj.createNestedObject(strdup(key));
	}

	static JsonObject& getObjectChildOrCreateNewNoKeyDup(JsonObject& obj,const char* key){
		return (obj.containsKey(key))?obj.get<JsonObject>(key):obj.createNestedObject(key);
	}

	static JsonObject& getObjectChildOrCreateNewNoKeyDup(JsonObject& obj, const char* group, const char* name){
		return getObjectChildOrCreateNewNoKeyDup(getObjectChildOrCreateNewNoKeyDup(obj, group), name);
	}

	static JsonArray& getObjectChildArrayOrCreateNew(JsonObject& obj,const char* key){
		return (obj.containsKey(key))?obj.get<JsonArray>(key):obj.createNestedArray(strdup(key));
	}

	static JsonArray& getObjectChildArrayOrCreateNewNoKeyDup(JsonObject& obj,const char* key){
		return (obj.containsKey(key))?obj.get<JsonArray>(key):obj.createNestedArray(key);
	}

	static bool getObjectFieldExistsAndNotEquals(JsonObject& obj,const char* key,JsonVariant val){
		if(!obj.containsKey(key)){
			return false;
		}
		return !CompareUtils::compareValues(obj.get<JsonVariant>(key), val);
	}

	static bool getObjectFieldNotExistsOrNotEquals(JsonObject& obj,const char* key,JsonVariant val){
		if(!obj.containsKey(key)){
			return true;
		}
		return !CompareUtils::compareValues(obj.get<JsonVariant>(key), val);
	}

	static bool getObjectFieldExistsAndEquals(JsonObject& obj,const char* key,JsonVariant val){
		if(!obj.containsKey(key)){
			return false;
		}
		return CompareUtils::compareValues(obj.get<JsonVariant>(key), val);
	}

	static unsigned int getCrc(JsonObject& obj){
		HashPrint hashPrint;
		obj.printTo(hashPrint);
		return hashPrint.hash();
	}

	static bool compareByCrc(JsonObject& obj1,JsonObject& obj2){
		return getCrc(obj1)==getCrc(obj2);
	}

	static JsonObject& getObjectFromString(DynamicJsonBuffer* b,const char* jsonStr){
		b->clear();
		JsonObject& res = b->parseObject(jsonStr);
		return (res.success())?res:b->createObject();
	}

	static JsonObject& getObjectChildFromStringOrCreateNew(DynamicJsonBuffer* b,const char* jsonStr,const char* key){
		return getObjectChildOrCreateNew(getObjectFromString(b,jsonStr),key);
	}

	template<typename T>
	static bool hasField(JsonObject& obj,const char* key){
		return (obj.containsKey(key) && obj.is<T>(key));
	}

	static bool hasFieldWithAnyType(JsonObject& obj,const char* key){
		return (obj.containsKey(key) );
	}

	static bool hasFieldAndValueEquals(JsonObject& obj,const char* key, char* value){
		return (obj.containsKey(key) && strcmp(obj.get<char*>(key),value)==0);
	}

	template<typename T>
	static bool setField(JsonObject& parent,const char* key,T value){
		if(!JsonObjectUtil::getObjectFieldExistsAndEquals(parent,key,value)){
			parent.set<T>(key, value);
			return true;
		}
		return false;
	}
	template<typename T>
	static T getField(JsonObject& parent,const char* key){
		return parent.get<T>(key);
	}

	static JsonObject& getFieldAsObject(JsonObject& parent,const char* key){
		return parent.get<JsonObject>(key);
	}

	template<typename T>
	static T getFieldIfKeyExistsOrDefault(JsonObject& obj,const char* key, T defaultValue){
		if(!hasField<T>(obj,key)){
			return defaultValue;
		}

		return getField<T>(obj, key);
	}

	static bool hasFieldInt(JsonObject& obj,const char* key){
		return hasField<int>(obj, key);
	}
	static bool hasFieldFloat(JsonObject& obj,const char* key){
		return hasField<float>(obj, key);
	}
	static bool hasFieldConstChar(JsonObject& obj,const char* key){
		return hasField<const char*>(obj, key);
	}
	static bool hasFieldChar(JsonObject& obj,const char* key){
		return hasField<char*>(obj, key);
	}

};

#endif /* LIBRARIES_DEVICELIB_UTILS_JSONOBJECTUTIL_H_ */
