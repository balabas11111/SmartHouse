/*
 * Measurer.h
 *
 *  Created on: 14 окт. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_MEASURER_MEASURER_H_
#define LIBRARIES_MEASURER_MEASURER_H_

#include "Arduino.h"
#include "Measureable.h"
#include "FunctionalInterrupt.h"

class Measurer: public Measureable{

public:

	Measurer(String _id,String _name,String _kind,String _val,uint8_t _count,std::function<Measureable*(void)> _measureFunction,std::function<Measureable*(void)> _initFunction,boolean _useCached){
		id=_id;
		name=_name;
		kind=_kind;
		val=_val;
		itemsCount=_count;
		measureFunction=_measureFunction;
		initFunction=_initFunction;

		lastMeasureTime=millis();
		useCached=_useCached;

		initialized=false;
	}

	void init(){
		if(initFunction!=nullptr){
			initFunction();
		}
		initialized=true;
	}

	boolean loop(){
		if(initialized){
			measure();
		}

		return true;
	}

	Measureable* getItems(){
		if(!useCached){
			return getExternal();
		}
		return items;
	}

	Measureable* measure(){
		Measureable arr[itemsCount]=getExternal();
		if(useCached){
			items=arr;
		}

		return arr;
	}

	uint getLastMeasureTime(){
		return millis()-lastMeasureTime;
	}

	String getJson(String timeStamp){
		String result="{"+getMeasuableAsJson((Measureable)this)+","
						+getStringAsJson("itemsCount", String(itemsCount), false)+","
						+getStringAsJson("time",timeStamp)+","
						+getStringAsJson("items",getMeasurableArrayAsJson(items,itemsCount),false)+
						"}";

		return result;
	}
private:

	Measureable* getExternal(){
		lastMeasureTime=millis();
		return measureFunction();
	}

	String getMeasurableArrayAsJson(Measureable* ar,uint count){
		String result="[";

		for (uint i=0;i<count;i++){
			result+="{"+getMeasuableAsJson(ar[i])+"}";
			if(i!=count-1){
				result+=",";
			}
		}

		result+="]";

		return result;
	}

	String getMeasuableAsJson(Measureable m){
		String result=getStringAsJson("name",m.id)+","
				+getStringAsJson("name",m.kind)+","
				+getStringAsJson("name",m.name)+","
				+getStringAsJson("name",m.val);

		return result;
	}

	String getStringAsJson(String nodeName,String nodeVal){
		return getStringAsJson(nodeName, nodeVal, true);
	}

	String getStringAsJson(String nodeName,String nodeVal,boolean useQuotes){
		String quotes=useQuotes?"\"":"";
		return "\""+nodeName+"\":"+quotes+nodeVal+quotes;
	}

	boolean initialized;

	std::function<Measureable*(void)> initFunction;
	std::function<Measureable*(void)> measureFunction;
	uint itemsCount;
	Measureable* items;

	ulong lastMeasureTime;
	boolean useCached;
};

#endif /* LIBRARIES_MEASURER_MEASURER_H_ */
