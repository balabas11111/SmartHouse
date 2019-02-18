/*
 * AbstractItemDAO.h
 *
 *  Created on: 17 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_ENTITY_ABSTRACTITEM_ABSTRACTITEMDAO_H_
#define LIBRARIES_DEVICELIB_ENTITY_ABSTRACTITEM_ABSTRACTITEMDAO_H_

#include <interfaces/Nameable.h>
#include <interfaces/Identifiable.h>
#include <list/AbstractItemList.h>

class AbstractItemDAO:public Nameable, public Identifiable {
public:
	AbstractItemDAO(String name,AbstractItem* daoItems[]);
	virtual ~AbstractItemDAO(){};

	virtual int init(int id);

	AbstractItemList<AbstractItem>* getItems();

	int startModel(JsonObject* model,JsonObject* descriptor);

	boolean canList();
	boolean canCash();
	boolean canGet();
	boolean canSave();

protected:
	AbstractItemList<AbstractItem>* items;

	JsonObject* model;
	JsonObject* descriptor;
};

#endif /* LIBRARIES_DEVICELIB_ENTITY_ABSTRACTITEM_ABSTRACTITEMDAO_H_ */
