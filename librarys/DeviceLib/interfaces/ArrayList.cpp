/*
 * ArrayList.cpp
 *
 *  Created on: Feb 13, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <interfaces/ArrayList.h>

template<typename T>
ArrayList<T>::ArrayList() {
	this->size=0;
}

template<typename T>
ArrayList<T>::ArrayList(T* items[]) {
	constructList(items,ARRAY_SIZE(items));
}

template<typename T>
ArrayList<T>::ArrayList(T* items[], uint8_t sizeIn)
{
	constructList(items,sizeIn);
}

template<typename T>
ArrayList<T>::~ArrayList() {}

template<typename T>
uint8_t ArrayList<T>::getSize() {
	return size;
}

template<typename T>
T** ArrayList<T>::getItems() {
	return items;
}

template<typename T>
inline T* ArrayList<T>::getItem(uint8_t index) {
	if(index>=this->size){
		return NULL;
	}

	return this->items[index];
}

template<typename T>
void ArrayList<T>::constructList(T* items[], uint8_t size) {
	this->items=items;
	this->size=size;
}
