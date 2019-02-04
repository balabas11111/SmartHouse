/*
 * Entity.h
 *
 *  Created on: Feb 4, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_ENTITYMANAGER_ENTITY_H_
#define LIBRARIES_DEVICELIB_ENTITYMANAGER_ENTITY_H_

#include "Arduino.h"

#include "interfaces/Identifiable.h"
#include "interfaces/Nameable.h"
#include "EntityManager/EntityBase.h"
#include "EntityManager/EntityServiceInt.h"

#include "ESP_Consts.h"

class Entity: public Nameable, Identifiable {
public:
	virtual ~Entity(){};

	virtual uint8_t getSize()=0;
	virtual uint8_t* getKeys()=0;

	virtual uint8_t getEntityDescrId()=0;
	virtual String getJson()=0;

	virtual boolean inMemory(){return true;}

	void initEntity(EntityBase** entityBases,EntityServiceInt* entityService){
		this->entityBases=entityBases;
		this->entityService=entityService;
	}

private:
	EntityServiceInt* entityService;
	EntityBase** entityBases;
	uint16_t index;
};

#endif /* LIBRARIES_DEVICELIB_ENTITYMANAGER_ENTITY_H_ */
