/*
 * CompareUtils.h
 *
 *  Created on: 25 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_UTILS_COMPAREUTILS_H_
#define LIBRARIES_ENTITYLIBSIMPLE_UTILS_COMPAREUTILS_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <utility>
#include <typeinfo>

class CompareUtils {
public:
	CompareUtils(){};
	virtual ~CompareUtils(){};

	static bool compareValues(JsonObject& obj1,JsonObject& obj2,const char* key){
		if(obj1.containsKey(key) || !obj2.containsKey(key)){
			return false;
		}
		return compareValues(obj1.get<JsonVariant>(key), obj2.get<JsonVariant>(key));
	}

	static bool compareValues(JsonVariant obj1,JsonVariant obj2){
		if(obj1==NULL || obj2==NULL){
			return false;
		}

		if(obj1.is<char*>()){
			return compareValuesVariants<char*>(obj1, obj2);
		}
		if(obj1.is<const char*>()){
			return compareValuesVariants<const char*>(obj1, obj2);
		}
		if(obj1.is<bool>()){
			return compareValuesVariants<bool>(obj1, obj2);
		}
		if(obj1.is<short>()){
			return compareValuesVariants<short>(obj1, obj2);
		}
		if(obj1.is<int>()){
			return compareValuesVariants<int>(obj1, obj2);
		}
		if(obj1.is<long>()){
			return compareValuesVariants<long>(obj1, obj2);
		}
		if(obj1.is<float>()){
			return compareValuesVariants<float>(obj1, obj2);
		}

		return obj1==obj2;
	}

	template<typename T>
	static bool compareValuesVariants(JsonVariant& obj1,JsonVariant& obj2){
		if(obj1.is<T>() && obj2.is<T>()){
			return compareValues<T>(obj1.as<T>(), obj2.as<T>());
		}
		return false;
	}

	template<typename T, typename = std::enable_if<std::is_base_of<char*, T>::value>>
	static bool compareValues(char* var1,char* var2){
		return strcmp(var1,var2)==0;
	}
	template<typename T, typename = std::enable_if<std::is_base_of<const char*, T>::value>>
	static bool compareValues(const char* var1,const char* var2){
		return strcmp(var1,var2)==0;
	}

	template<typename T, typename = std::enable_if<std::is_base_of<bool, T>::value>>
	static bool compareValues(bool var1,bool var2){
		return var1 == var2;
	}
	template<typename T, typename = std::enable_if<std::is_base_of<short, T>::value>>
	static bool compareValues(short var1,short var2){
		return var1 == var2;
	}
	template<typename T, typename = std::enable_if<std::is_base_of<int, T>::value>>
	static bool compareValues(int var1,int var2){
		return var1 == var2;
	}
	template<typename T, typename = std::enable_if<std::is_base_of<long, T>::value>>
	static bool compareValues(long var1,long var2){
		return var1 == var2;
	}
	template<typename T, typename = std::enable_if<std::is_base_of<float, T>::value>>
	static bool compareValues(float var1,float var2){
		return var1 == var2;
	}
};

#endif /* LIBRARIES_DEVICELIB_UTILS_COMPAREUTILS_H_ */
