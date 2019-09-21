/*
 * DataSelectorEntityManager.h
 *
 *  Created on: 21 сент. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_DATASELECTORENTITYMANAGER_H_
#define LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_DATASELECTORENTITYMANAGER_H_

#include <Arduino.h>
#include <ArduinoJson.h>

#include <JsonObjectUtil.h>

#include <Notifiers/DataSelector.h>
#include <EntityManager.h>

class DataSelectorEntityManager:public DataSelector{
public:
	DataSelectorEntityManager(EntityManager* manager){
		this->manager = manager;
	}
	virtual ~DataSelectorEntityManager(){}

	virtual void selectData(EntityJsonRequestResponse* req){
		this->manager->get(req);
	}
protected:
/*
	virtual void doSelectDataByParams(char* group = nullptr, char* name =
			nullptr, char* param = nullptr) override{

		UNUSED(param);

		if(group!=nullptr){
			this->data->addRequestParam((char*)GROUP, group);
		}
		if(name!=nullptr){
			this->data->addRequestParam((char*)NAME, name);
		}

		this->manager->get(this->data);
	}
*/
private:
	EntityManager* manager;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_NOTIFIERS_DATASELECTORENTITYMANAGER_H_ */
