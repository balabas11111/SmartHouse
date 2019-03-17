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

#define FILE_MODE_READ "r"
#define FILE_MODE_WRITE "w"

class FileUtils {
public:
	FileUtils();
	virtual ~FileUtils();

	static bool init();
	static bool exists(String fileName);
	static bool existsAndHasSize(String fileName);
	static bool existsAndHasSizeChar(const char* fileName);

	static bool saveJsonToFile(String fileName,JsonObject& obj);
	static bool loadJsonFromFile(String fileName, JsonObject& obj);
	static bool saveStringToFile(String fileName,String value);
	static String loadStringFromFile(String fileName);

	static File getFile(String fileName,const char* mode);
	static File getFileChar(const char* fileName,const char* mode);

	static unsigned int getCrc(const char* fileName);
	static unsigned int getCrc(JsonObject& obj);

	static bool compareCrs(const char* fileName,JsonObject& obj);

	static void dir();
	static void dir(const char* path);

	static bool saveRootJson(const char* fileName,const char* rootPath,JsonObject& root);
	static bool deleteFile(const char* fileName);
	static bool deleteAllFiles(const char* path);

	static bool printFile(const char* fileName);

	static int saveFile(const char* fileName,uint8_t *data, size_t len);

	static void dirFiles(JsonObject& json);
};

#endif /* LIBRARIES_DEVICELIB_UTILS_FILEUTILS_H_ */

