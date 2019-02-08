/*
 * FileUtils.h
 *
 *  Created on: Feb 8, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_UTILS_FILEUTILS_H_
#define LIBRARIES_DEVICELIB_UTILS_FILEUTILS_H_

#include <Arduino.h>
#include <FS.h>

class FileUtils {
public:
	FileUtils();
	virtual ~FileUtils();

	static boolean saveStringToFile(String fileName,String value);
	static String loadStringFromFile(String fileName);
};

#endif /* LIBRARIES_DEVICELIB_UTILS_FILEUTILS_H_ */
