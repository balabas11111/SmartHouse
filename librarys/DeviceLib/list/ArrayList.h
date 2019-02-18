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

const char NOT_IMPLEMENTD[]    PROGMEM = "Not implemented";

template <typename T>
class ArrayList {
public:

	ArrayList(){
		this->items=NULL;
		this->size=0;
	};

	ArrayList(T* items[]){
		//constructList(items,ARRAY_SIZE(items));
		this->items=items;
		this->size=ARRAY_SIZE(items);
	}

	ArrayList(T* items[], uint8_t size){
		//constructList(items,size);
		this->items=items;
		this->size=size;
	}

	virtual ~ArrayList(){};

	void constructList(T** items, uint8_t size){
		this->items=items;
		this->size=size;
	}

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

	int getItemIndex(T* item){
		for(uint8_t i=0;i<getSize();i++){
			if(this->getItem(i)==item){
				return i;
			}
		}
		return -1;
	}

	boolean setItem(T* item,int index){
		if(!checkIndex(index) || items==NULL || items==nullptr){
			return false;
		}
		items[index] = item;
		return true;
	}

	boolean checkIndex(int index){
		return (index>0)&&(index<getSize());
	}

	boolean hasElements(){
		return getSize()>0;
	}

	virtual T* getItemByFieldValue(String fieldName,String fieldValue){
		Serial.println(FPSTR(NOT_IMPLEMENTD));
		return NULL;
	}
protected:
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
