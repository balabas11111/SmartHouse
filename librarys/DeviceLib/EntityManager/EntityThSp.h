/*
 * EntityThSp.h
 *
 *  Created on: Feb 4, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_ENTITYMANAGER_ENTITYTHSP_H_
#define LIBRARIES_DEVICELIB_ENTITYMANAGER_ENTITYTHSP_H_

#include "Arduino.h"

#include "interfaces/Identifiable.h"
#include "interfaces/Nameable.h"

#include "ESP_Consts.h"
#include "EntityManager/EntityBase.h"

const uint8_t ThingSpeakSize PROGMEM =3;

const char* const ThingSpeakKeys[]      PROGMEM=
{		"fieldId","periodicSend","autoCreateChannel"
};
const char* const ThingSpeakDefaults[]      PROGMEM=
{
		"0","0","1"
};

class EntityThSp:public EntityBase {
public:
	virtual ~EntityThSp();

	EntityBase* clone() const {return new EntityThSp(*this);}
	virtual uint16_t getId(){return ENTITY_THING_SPEAK_INDEX;}
	virtual const char* const* getKeys(){return ThingSpeakKeys;}
	virtual const char* const* getDefaults(){return ThingSpeakDefaults;}
	//virtual uint8_t getSize(){return ThingSpeakSize;}
};

#endif /* LIBRARIES_DEVICELIB_ENTITYMANAGER_ENTITYTHSP_H_ */
