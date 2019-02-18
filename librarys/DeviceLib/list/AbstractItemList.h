/*
 * AbstractEntityList.h
 *
 *  Created on: Feb 14, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_INTERFACES_ABSTRACTITEMLIST_H_
#define LIBRARIES_DEVICELIB_INTERFACES_ABSTRACTITEMLIST_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <utility>
#include <list/ArrayList.h>
#include <entity/Entity.h>
#include <entity/abstractItem/AbstractItem.h>


template <typename T, typename std::enable_if<std::is_base_of<AbstractItem, T>::value>::type* = nullptr>
class AbstractItemList: public ArrayList<T>, public ChangeDispatchable {
public:
	virtual ~AbstractItemList(){};

	template <typename T,typename KV, typename std::enable_if<std::is_base_of<AbstractItem, T>::value>::type* = nullptr>
	virtual T* getItemByFieldValue(String key,KV value){
		if(this->getSize()==0){
			return NULL;
		}
		for(uint8_t i=0;i<this->getSize();i++){
			if(this->getItem(i)->getFieldValue(key)==value){
				return this->getItem(i);
			}
		}
		return NULL;
	}
	virtual T* getItemById(int id){
		return this->getItemByFieldValue<AbstractItem,int>("id", id);
	}
	virtual T* getItemByName(String name){
		return this->getItemByFieldValue<AbstractItem,String>("name", name);
	}
	virtual boolean getChanged() override{
		if(this->getSize()==0){
			return false;
		}
		if(getData()->getChanged()){
			return true;
		}
		for(uint8_t i=0;i<this->getSize();i++){
			if(this->getItem(i)->getChanged()){
				return true;
			}
		}
		return false;
	}
	virtual void setChanged(boolean changed) override{
		if(this->getSize()==0){
			return;
		}
		if(this->getData()->getChanged()){
			this->getData()->setChanged(changed);
		}

		for(uint8_t i=0;i<this->getSize();i++){
			if(this->getItem(i)->getChanged()){
				this->getItem(i)->setChanged(changed);
			}
		}
	}
};

#endif /* LIBRARIES_DEVICELIB_INTERFACES_ARRAYLISTSEARCHABLE_H_ */
