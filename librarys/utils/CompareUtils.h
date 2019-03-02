/*
 * CompareUtils.h
 *
 *  Created on: 25 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_UTILS_COMPAREUTILS_H_
#define LIBRARIES_DEVICELIB_UTILS_COMPAREUTILS_H_

#include <Arduino.h>
#include <utility>
#include <typeinfo>

class CompareUtils {
public:
	CompareUtils(){};
	virtual ~CompareUtils(){};

	template<typename T, typename = std::enable_if<std::is_base_of<const char*, T>::value>>
	static bool compareValues(const char* var1,const char* var2){
		return strcmp(var1,var2)==0;
	}

	template<typename T, typename = std::enable_if<std::is_base_of<int*, T>::value>>
	static bool compareValues(int var1,int var2){
		return var1 == var2;
	}
	template<typename T, typename = std::enable_if<std::is_base_of<float*, T>::value>>
	static bool compareValues(float var1,float var2){
		return var1 == var2;
	}
};

#endif /* LIBRARIES_DEVICELIB_UTILS_COMPAREUTILS_H_ */
