/*
 * ArrayList.h
 *
 *  Created on: Feb 13, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_INTERFACES_ARRAYLIST_H_
#define LIBRARIES_DEVICELIB_INTERFACES_ARRAYLIST_H_

#include <Arduino.h>
#include <ESP_Consts.h>

template <typename T>
class ArrayList {
public:

	ArrayList(){
		this->size=0;
	};

	ArrayList(T* items[]){
		constructList(items,ARRAY_SIZE(items));
	}

	ArrayList(T* items[], uint8_t size){
		constructList(items,size);
	}

	virtual ~ArrayList(){};

	uint8_t getSize(){
		return size;
	}

	T** getItems(){
		return items;
	}

	T* getItem(uint8_t index){
		if(index>=this->size){
			return NULL;
		}

		return this->items[index];
	}
protected:
	void constructList(T** items, uint8_t size){
		this->items=items;
		this->size=size;
	}
	boolean checkNonNull(T* box) {
		if(box==NULL || box==nullptr){
			Serial.print(FPSTR("Object is NULL"));
			return false;
		}else{
			return true;
		}
	}

private:
	T** items;
	uint8_t size=0;
};

#endif /* LIBRARIES_DEVICELIB_INTERFACES_ARRAYLIST_H_ */
