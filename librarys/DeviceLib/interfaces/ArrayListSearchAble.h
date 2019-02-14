/*
 * AbstractEntityList.h
 *
 *  Created on: Feb 14, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_INTERFACES_ARRAYLISTSEARCHABLE_H_
#define LIBRARIES_DEVICELIB_INTERFACES_ARRAYLISTSEARCHABLE_H_

#include <Arduino.h>
#include <interfaces/ArrayList.h>

template <typename T>
class ArrayListSearchAble: public ArrayList<T> {
public:
	virtual T* getItemById(int id)=0;
	virtual T* getItemByName(String name)=0;
	virtual T* getItemByHash(String hash)=0;
};

#endif /* LIBRARIES_DEVICELIB_INTERFACES_ARRAYLISTSEARCHABLE_H_ */
