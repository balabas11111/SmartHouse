/*
 * FileUtils.h
 *
 *  Created on: Feb 8, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_UTILS_FILEUTILS_H_
#define LIBRARIES_DEVICELIB_UTILS_FILEUTILS_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <FS.h>

const PROGMEM char FILE_MODE_READ[]= "r";
const PROGMEM char FILE_MODE_WRITE[]= "w";

class FileUtils {
public:
	FileUtils();
	virtual ~FileUtils();

	static bool init();
	static boolean exists(String fileName);
	static boolean existsAndHasSize(String fileName);
	static boolean existsAndHasSizeChar(const char* fileName);

	static boolean saveJsonToFile(String fileName,JsonObject& obj);
	static boolean loadJsonFromFile(String fileName, JsonObject& obj);
	static boolean saveStringToFile(String fileName,String value);
	static String loadStringFromFile(String fileName);

	static File getFile(String fileName,const char* mode);
	static File getFileChar(const char* fileName,const char* mode);
	//static boolean loadJsonFromFile(String fileName, JsonObject& obj, DynamicJsonBuffer* buf);
protected:
	//static bool checkSpiff();
};

#endif /* LIBRARIES_DEVICELIB_UTILS_FILEUTILS_H_ */

