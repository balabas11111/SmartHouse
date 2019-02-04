/*
 * Entity.h
 *
 *  Created on: Feb 4, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_ENTITYMANAGER_ENTITYDESCR_H_
#define LIBRARIES_DEVICELIB_ENTITYMANAGER_ENTITYDESCR_H_
/*
 * Entity class contains list of entityBase identitfiers
 */
#include "Arduino.h"
#include "ESP_Consts.h"

#include "interfaces/Identifiable.h"
#include "interfaces/Nameable.h"

#include "EntityManager/EntityBase.h"

class EntityDescr:public Nameable, Identifiable {
public:
	EntityDescr(){};
	virtual ~EntityDescr(){};

	virtual uint8_t* getKeys()=0;

	virtual uint8_t getSize()=0;

	virtual boolean isInMemory(){return false;}

	virtual boolean isJsonCached(){return true;}

	virtual String getJson(){return "";}

protected:
	EntityBase *entityBases;
};

#endif /* LIBRARIES_DEVICELIB_ENTITYMANAGER_ENTITYDESCR_H_ */
