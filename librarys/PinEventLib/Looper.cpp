/*
 * Looper.cpp
 *
 *  Created on: 13 окт. 2018 г.
 *      Author: Vitaliy
 */

#include <Looper.h>

Looper::Looper(Loopable** _items,uint8_t sizeIn){
	items=_items;
	//size=sizeof(_items);
	size=sizeIn;

	displayDetails();
	Serial.println();
}

String Looper::displayDetails(){
	Serial.println("----------------------");

	String result="Looper size="+String(size);

	Serial.println(result);

	Serial.println();
	for(uint8_t i=0;i<size;i++){
		items[i]->displayDetails();
	}

	Serial.println("----------------------");

	return result;
}


boolean Looper::loop(){
	boolean result=false;

	for(uint8_t i=0;i<size;i++){
		result=result || items[i]->loop();
	}
	#ifdef DISPLAY_LOOPS
		Serial.println("Looper loop="+String(result));
	#endif

	return result;
}



