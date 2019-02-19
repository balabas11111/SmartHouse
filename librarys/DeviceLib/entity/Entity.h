/*
 * Entity.h
 *
 *  Created on: Feb 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_ENTITY_ENTITY_H_
#define LIBRARIES_DEVICELIB_ENTITY_ENTITY_H_

#include  <Arduino.h>
#include <ArduinoJson.h>

#include  <interfaces/Nameable.h>

class Entity:public Nameable{
public:
	Entity(String name,String& dao, String& model);
	virtual ~Entity(){};

	String getDao();

	String getModel();

	int init(JsonObject& data,JsonObject& model);

protected:
	String dao;
	String model;
};

#endif /* LIBRARIES_DEVICELIB_ENTITY_ENTITY_H_ */
