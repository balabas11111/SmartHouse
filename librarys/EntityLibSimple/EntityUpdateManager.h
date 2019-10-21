/*
 * EntityUpdateManager.h
 *
 *  Created on: Aug 27, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_ENTITYUPDATEMANAGER_H_
#define LIBRARIES_ENTITYLIBSIMPLE_ENTITYUPDATEMANAGER_H_

#include <Arduino.h>
#include <list>
#include <iterator>
#include <Ticker.h>
#include <EntityUpdate.h>



class EntityUpdateManager {
public:
	EntityUpdateManager(EntityUpdate* entities[], int count);
	virtual ~EntityUpdateManager(){}

	int init(int interval);

	void registerEntity(EntityUpdate* entity);

	void updateEntities(bool force = false);
	void loop(bool force = false);

private:
	std::list<EntityUpdate*> entities;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_ENTITYUPDATEMANAGER_H_ */
