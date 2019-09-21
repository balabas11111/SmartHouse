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
	EntityUpdateManager(EntityUpdate* entities[], int count);
	virtual ~EntityUpdateManager(){}

	int init(int interval);

	void updateEntities(bool withCheck = true);
	void loop(bool withCheck = true);

private:
	std::list<EntityUpdate*> entities;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_ENTITYUPDATEMANAGER_H_ */
