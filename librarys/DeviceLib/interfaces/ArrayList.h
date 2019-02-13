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

	ArrayList();
	ArrayList(T* items[]);
	ArrayList(T* items[], uint8_t size);

	virtual ~ArrayList();

	uint8_t getSize();
	T** getItems();

	T* getItem(uint8_t index);
protected:
	void constructList(T* items[], uint8_t size);
private:
	T* items[];
	uint8_t size=0;
};

#endif /* LIBRARIES_DEVICELIB_INTERFACES_ARRAYLIST_H_ */
