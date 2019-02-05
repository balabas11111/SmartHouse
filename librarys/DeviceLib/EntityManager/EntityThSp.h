/*
 * EntityThSp.h
 *
 *  Created on: Feb 4, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_ENTITYMANAGER_ENTITYTHSP_H_
#define LIBRARIES_DEVICELIB_ENTITYMANAGER_ENTITYTHSP_H_

#include <EntityManager/EntityFields.h>
#include "Arduino.h"

#include "interfaces/Identifiable.h"
#include "interfaces/Nameable.h"

#include "ESP_Consts.h"

const uint8_t ThingSpeakSize PROGMEM =3;

const char* const ThingSpeakKeys[]      PROGMEM=
{		"fieldId","periodicSend","autoCreateChannel"
};
const char* const ThingSpeakDefaults[]      PROGMEM=
{
		"0","0","1"
};

class EntityThSp:public EntityFields {
public:
	EntityThSp(){};
	virtual ~EntityThSp();

	EntityFields* clone() const {return new EntityThSp(*this);}
	virtual uint8_t getId(){return ENTITY_THING_SPEAK_INDEX;}
	virtual const char* const* getKeys(){return ThingSpeakKeys;}
	virtual const char* const* getDefaults(){return ThingSpeakDefaults;}
	//virtual uint8_t getSize(){return ThingSpeakSize;}
};

#endif /* LIBRARIES_DEVICELIB_ENTITYMANAGER_ENTITYTHSP_H_ */
