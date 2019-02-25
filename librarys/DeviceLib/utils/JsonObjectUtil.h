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
#include <utils/CompareUtils.h>

class JsonObjectUtil {
public:
	JsonObjectUtil();
	virtual ~JsonObjectUtil();

	//copies all values  from to to target
	static int copy(JsonObject& from,JsonObject& to,boolean overwrite){
		int total = 0;

		for (JsonObject::iterator it = from.begin(); it != from.end(); ++it ){

			const char* key=it->key;

			if(it->value.is<int>()){
				total+=copyField<int>(from,to,key,overwrite);
			}else if(it->value.is<const char*>()){
				total+=copyField<const char*>(from,to,key,overwrite);
			}else if(it->value.is<JsonObject>()){
				if(!to.containsKey(key)){
					to.createNestedObject(key);
				}
				total+=copy(it->value,to.get<JsonObject>(key),overwrite);
			}
		}

		return total;
	}

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

	//
	virtual bool sync(JsonObject& obj1,JsonObject& obj2,boolean overwrite){
		bool result = false;



		return result;
	}
};

#endif /* LIBRARIES_DEVICELIB_UTILS_JSONOBJECTUTIL_H_ */
