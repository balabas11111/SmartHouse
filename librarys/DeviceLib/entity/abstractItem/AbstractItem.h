/*
 * AbstractItem.h
 *
 *  Created on: 16 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_ENTITY_ABSTRACTITEM_ABSTRACTITEM_H_
#define LIBRARIES_DEVICELIB_ENTITY_ABSTRACTITEM_ABSTRACTITEM_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <utility>
#include <interfaces/ChangeDispatchable.h>
#include <entity/abstractItem/AbstractItem.h>
#include <list/AbstractItemList.h>
#include <entity/descr/AbstractItemDescriptor.h>

class AbstractItem: public ChangeDispatchable{
public:
	AbstractItem(AbstractItem* items[]){};
	virtual ~AbstractItem(){};

	int begin();

	String getFieldValue(String key);
	int setFieldValue(String key,String value);

	boolean validateKey(String key);

	int loadDefault();

	int putJson(JsonObject& obj);
	String getJson(String root);

	boolean isCached();
	int saveToCache();



	int initializeItem(JsonObject* parent,JsonObject* object);

protected:
	JsonObject* parent;
	JsonObject* object;

	AbstractItemDescriptor descriptor;

	AbstractItem* item;

	AbstractItemList<AbstractItem>* list;

	String path;
};

#endif /* LIBRARIES_DEVICELIB_ENTITY_ABSTRACTITEM_ABSTRACTITEM_H_ */
