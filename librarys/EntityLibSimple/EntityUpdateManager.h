/*
 * EntityUpdateManager.h
 *
 *  Created on: Aug 27, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_ENTITYUPDATEMANAGER_H_
#define LIBRARIES_ENTITYLIBSIMPLE_ENTITYUPDATEMANAGER_H_

#include <list>
#include <iterator>
#include <Arduino.h>
#include <Ticker.h>

#include <EntityUpdate.h>
#include <Entity.h>

class EntityUpdateManager {
public:
	//EntityUpdateManager(){}
	virtual ~EntityUpdateManager(){}

	int init(std::list<EntityUpdate*>& entities, int interval);

	void update();

private:
	std::list<EntityUpdate*>& entities;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_ENTITYUPDATEMANAGER_H_ */
