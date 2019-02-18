/*
 * AbstractEntityList.h
 *
 *  Created on: Feb 14, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_INTERFACES_ABSTRACTITEMDAOLIST_H_
#define LIBRARIES_DEVICELIB_INTERFACES_ABSTRACTITEMDAOLIST_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <utility>
#include <list/ArrayList.h>
#include <entity/Entity.h>
#include <entity/abstractItem/AbstractItemDAO.h>


class AbstractItemDAOList: public ArrayList<AbstractItemDAO>{
public:
	AbstractItemDAOList(AbstractItemDAO* items[]):
		ArrayList(items){
		init();
	}

	virtual ~AbstractItemDAOList(){};

	virtual AbstractItemDAO* getItemById(int id){
		if(this->getSize()==0){
			return NULL;
		}
		for(uint8_t i=0;i<this->getSize();i++){
			if(this->getItem(i)->getId()==id){
				return this->getItem(i);
			}
		}
		return NULL;
	}
	virtual AbstractItemDAO* getItemByName(String name){
		if(this->getSize()==0){
			return NULL;
		}
		for(uint8_t i=0;i<this->getSize();i++){
			if(this->getItem(i)->getName()==name){
				return this->getItem(i);
			}
		}
		return NULL;
	}
protected:
	void init(){
		for(uint8_t i=0;i<this->getSize();i++){
			this->getItem(i)->init(i);
		}
	}
};

#endif /* LIBRARIES_DEVICELIB_INTERFACES_ARRAYLISTSEARCHABLE_H_ */
