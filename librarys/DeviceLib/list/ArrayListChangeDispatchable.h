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
#include <interfaces/ChangeDispatchable.h>


template <typename T, typename std::enable_if<std::is_base_of<ChangeDispatchable, T>::value>::type* = nullptr>
class ArrayListChangeDispatchable: public ArrayList<T>, public ChangeDispatchable {
public:
	virtual ~ArrayListChangeDispatchable(){};

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
