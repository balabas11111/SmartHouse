/*
 * IdentifiAbleNameAble.h
 *
 *  Created on: 15 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_INTERFACES_ENTITY_H_
#define LIBRARIES_DEVICELIB_INTERFACES_ENTITY_H_

#include  <Arduino.h>
#include <ArduinoJson.h>
#include <entity/descr/AbstractItemDescriptor.h>

#include  <interfaces/ChangeDispatchable.h>
#include  <interfaces/DescribeAble.h>
#include  <interfaces/HashAble.h>
#include  <interfaces/Identifiable.h>
#include  <interfaces/Nameable.h>
#include  <interfaces/SizeAble.h>
#include  <interfaces/TypeAble.h>


class Entity: public DescribeAble, public Identifiable, public Nameable, public HashAble, public SizeAble, public TypeAble, public ChangeDispatchable {
public:
	Entity(){};

};


#endif /* LIBRARIES_DEVICELIB_INTERFACES_IDENTIFIABLENAMEABLE_H_ */
