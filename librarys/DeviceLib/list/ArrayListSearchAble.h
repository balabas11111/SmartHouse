/*
 * AbstractEntityList.h
 *
 *  Created on: Feb 14, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_INTERFACES_ARRAYLISTSEARCHABLE_H_
#define LIBRARIES_DEVICELIB_INTERFACES_ARRAYLISTSEARCHABLE_H_

#include <Arduino.h>
#include <utility>
#include <list/ArrayList.h>
#include <interfaces/Entity.h>

template <typename T, typename std::enable_if<std::is_base_of<Entity, T>::value>::type* = nullptr>
class ArrayListSearchAble: public ArrayList<T>, public ChangeDispatchable {
public:
	virtual ~ArrayListSearchAble(){};

	virtual T* getItemById(int id){
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
	virtual T* getItemByName(String name){
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
	virtual T* getItemByHash(String hash){
		if(this->getSize()==0){
			return NULL;
		}
		for(uint8_t i=0;i<this->getSize();i++){
			if(this->getItem(i)->getHash()==hash){
				return this->getItem(i);
			}
		}
		return NULL;
	}
	virtual boolean getChanged() override{
		if(this->getSize()==0){
			return false;
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
		for(uint8_t i=0;i<this->getSize();i++){
			if(this->getItem(i)->getChanged()){
				this->getItem(i)->setChanged(changed);
			}
		}
	}
};

#endif /* LIBRARIES_DEVICELIB_INTERFACES_ARRAYLISTSEARCHABLE_H_ */
