/*
 * IdentifiAbleNameAble.h
 *
 *  Created on: 15 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_INTERFACES_ENTITY_H_
#define LIBRARIES_DEVICELIB_INTERFACES_ENTITY_H_

#include  <interfaces/ChangeDispatchable.h>
#include  <interfaces/HashAble.h>
#include  <interfaces/Identifiable.h>
#include  <interfaces/JsonSetGetAble.h>
#include  <interfaces/Nameable.h>


class Entity: public Identifiable, public Nameable, public HashAble, public ChangeDispatchable, public JsonSetGetAble {

};

#endif /* LIBRARIES_DEVICELIB_INTERFACES_IDENTIFIABLENAMEABLE_H_ */
