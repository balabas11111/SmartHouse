/*
 * HashUtils.h
 *
 *  Created on: 17 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_UTILS_HASHUTILS_H_
#define LIBRARIES_DEVICELIB_UTILS_HASHUTILS_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Hash.h>

class HashUtils {
public:
	HashUtils(){};

	static String getHash(String obj){
		return sha1(obj);
	}
};

#endif /* LIBRARIES_DEVICELIB_UTILS_HASHUTILS_H_ */
