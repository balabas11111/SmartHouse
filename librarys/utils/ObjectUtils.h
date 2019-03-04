/*
 * ObjectUtils.h
 *
 *  Created on: 3 мар. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_UTILS_OBJECTUTILS_H_
#define LIBRARIES_UTILS_OBJECTUTILS_H_

#define ARRAY_SIZE(x) sizeof(x)/sizeof(x[0])
#define VAR_NAME(var) #var

class ObjectUtils {
public:
	virtual ~ObjectUtils(){};
};

#endif /* LIBRARIES_UTILS_OBJECTUTILS_H_ */
