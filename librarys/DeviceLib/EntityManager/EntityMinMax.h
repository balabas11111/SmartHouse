/*
 * EntityThSp.h
 *
 *  Created on: Feb 4, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_ENTITYMANAGER_ENTITYMINMAX_H_
#define LIBRARIES_DEVICELIB_ENTITYMANAGER_ENTITYMINMAX_H_

#include <EntityManager/EntityFields.h>
#include "Arduino.h"

#include "interfaces/Identifiable.h"
#include "interfaces/Nameable.h"

#include "ESP_Consts.h"

const uint8_t MinMaxSize PROGMEM =3;

const char* const MinMaxKeys[]      PROGMEM=
{		"min","max","autoCreateChannel"
};
const char* const MinMaxDefaults[]      PROGMEM=
{
		"0","0","1"
};

class EntityMinMax:public EntityFields {
public:
	EntityMinMax(){};
	virtual ~EntityMinMax();

	EntityFields* clone() const {return new EntityMinMax(*this);}
	virtual uint8_t getId(){return ENTITY_MIN_MAX_INDEX;}
	virtual const char* const* getKeys(){return MinMaxKeys;}
	virtual const char* const* getDefaults(){return MinMaxDefaults;}
	//virtual uint8_t getSize(){return MinMaxSize;}
};

#endif /* LIBRARIES_DEVICELIB_ENTITYMANAGER_ENTITYTHSP_H_ */
