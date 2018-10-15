/*
 * Measurer.h
 *
 *  Created on: 14 окт. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_MEASURER_MEASURER_H_
#define LIBRARIES_MEASURER_MEASURER_H_

#include "Arduino.h"
#include <Measureable.h>
#include "FunctionalInterrupt.h"
#include <PinEventProcessor.h>

class Measurer: public Measureable,public PinEventProcessor{

public:

	virtual boolean init()=0;
	virtual void getExternal()=0;

	Measurer(String _id,String _name,String _kind,String _val,uint8_t _count,boolean _quoteValue)
	:Measureable(_id, _name, _kind, _val){
		itemsCount=_count;
		lastMeasureTime=millis();
		quoteValue=_quoteValue;

		items=new Measureable[itemsCount];

		initialized=false;
	}

	String getName() override{
		return name;
	}

	boolean loop(){
		if(initialized){
			measure();
		}

		return true;
	}

	Measureable* getItems(){
		return items;
	}

	Measureable* measure(){
		Serial.println("-measure "+name);
		getExternal();
		return getItems();
	}

	uint getLastMeasureTime(){
		return millis()-lastMeasureTime;
	}

	String getJson(){
		return getJson(String(millis()));
	}

	String getJson(String timeStamp){
		String result="{"+getMeasuableAsJson(*(this),true)+","
						+getStringAsJson("itemsCount", String(itemsCount), false)+","
						+getStringAsJson("time",timeStamp)+","
						+getStringAsJson("items",getMeasurableArrayAsJson(measure(),itemsCount,quoteValue),false)+
						"}";

		return result;
	}

	boolean processEvent(PinEvent event) override{
		Serial.println(event.getText());
		return false;
	}
	PinEvent processEventNow(PinEvent event) override{
		if(event.isValid())
			if(isTargetOfEvent(event)){
				//Serial.println(name+" processNow "+event.getText());
				Serial.println("Measurer ("+name+") - PinEvent("+event.getText()+")");
				if(event.getKind().equals(PIN_EVENT_STATE_GET)){
					return constructEvent(PIN_EVENT_STATE_CURRENT,true);
				}
			}
		//Serial.println("Not my event");
		return PinEvent();
	}

	PinEvent constructEvent(String evName,boolean bubble) override{
		String str=getJson();
		//PinEvent res=PinEvent(evName,true,0,0,0,str,name,PIN_EVENT_TARGET_ANY);
		//Serial.println("Event constructed "+res.getText());
		return PinEvent(evName,bubble,0,0,0,str,name,PIN_EVENT_TARGET_ANY);
	}

	boolean isDispatcherOfEvent(PinEvent event) override{
		if(name.equals(event.getDispatcherName())){
			return true;
		}

		return false;
	}

	boolean isTargetOfEvent(PinEvent event) override{
		if(name.equals(event.getTargetName())){
			return true;
		}

		return false;
	}
private:

	String getMeasurableArrayAsJson(Measureable* ar,uint count,boolean _quoteValue){
		String result="[";

		for (uint i=0;i<count;i++){
			result+="{"+getMeasuableAsJson(ar[i],_quoteValue)+"}";
			if(i!=count-1){
				result+=",";
			}
		}

		result+="]";

		return result;
	}

	String getMeasuableAsJson(boolean _quoteValue){
		return getMeasuableAsJson(*(this),_quoteValue);
	}

	String getMeasuableAsJson(Measureable m,boolean _quoteValue){
		String result=getStringAsJson("id",m.id)+","
				+getStringAsJson("name",m.name)+","
				+getStringAsJson("val",m.val,_quoteValue)+","
				+getStringAsJson("kind",m.kind);

		return result;
	}

	String getStringAsJson(String nodeName,String nodeVal){
		return getStringAsJson(nodeName, nodeVal, true);
	}

	String getStringAsJson(String nodeName,String nodeVal,boolean useQuotes){
		String quotes=useQuotes?"\"":"";
		return "\""+nodeName+"\":"+quotes+nodeVal+quotes;
	}

protected:
	boolean initialized;
	uint itemsCount;
	Measureable* items;

	ulong lastMeasureTime;
	boolean quoteValue;
};

#endif /* LIBRARIES_MEASURER_MEASURER_H_ */
