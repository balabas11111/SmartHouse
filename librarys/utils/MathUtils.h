/*
 * MathUtils.h
 *
 *  Created on: 2 мар. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_UTILS_MATHUTILS_H_
#define LIBRARIES_UTILS_MATHUTILS_H_

#define ARRAY_SIZE(x) sizeof(x)/sizeof(x[0])
#define VAR_NAME(var) #var

class MathUtils {
public:
	virtual ~MathUtils(){};


	static bool compareChar(const char* ch1,const char* ch2){
		return strcmp(ch1,ch2)==0;
	}
};

#endif /* LIBRARIES_UTILS_MATHUTILS_H_ */
