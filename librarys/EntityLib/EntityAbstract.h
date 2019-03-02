/*
 * EntityAbstract.h
 *
 *  Created on: 2 мар. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIB_ENTITYABSTRACT_H_
#define LIBRARIES_ENTITYLIB_ENTITYABSTRACT_H_

#include <EntityJson.h>

class EntityAbstract:public EntityJson {
public:
	EntityAbstract(const char* group,const char* entName,const char* descr,const char* descriptor)
		:EntityJson(group, entName, descr, descriptor){};
	virtual ~EntityAbstract(){};
};

#endif /* LIBRARIES_ENTITYLIB_ENTITYABSTRACT_H_ */
