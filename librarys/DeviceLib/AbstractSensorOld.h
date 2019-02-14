/*
 * AbstractSensor.h
 *
 *  Created on: 30 окт. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_AbstractSensor_H_
#define LIBRARIES_DEVICELIB_AbstractSensor_H_

#include "Arduino.h"
#include "ESP_Consts.h"

#include "interfaces/Nameable.h"

//Abstract sensor fields
const char SENSOR_FIELD_id[]    PROGMEM = "id";
const char SENSOR_FIELD_name[]  PROGMEM = "name";
const char SENSOR_FIELD_type[]  PROGMEM = "type";
const char SENSOR_FIELD_size[]  PROGMEM = "size";
const char SENSOR_FIELD_descr[] PROGMEM = "descr";
const char SENSOR_FIELD_val[]   PROGMEM = "val";

const char SENSOR_FIELD_fieldId[]    PROGMEM = "fieldId";
const char SENSOR_FIELD_minVal[]     PROGMEM = "minVal";
const char SENSOR_FIELD_maxVal[]     PROGMEM = "minVal";
const char SENSOR_FIELD_setAllowed[] PROGMEM = "setAllowed";

//Abstract sensor Kinds
const char SENSOR_KIND_pinDigital[] PROGMEM = "pinDigital";
const char SENSOR_KIND_sensor[]     PROGMEM = "sensor";

class AbstractSensorOld:public Nameable{

public:
	struct childRecords{
			uint8_t id;
			String name;
			String type;
			String size;
			String descr;
			float val;

			uint8_t fieldId;
			float minVal;
			float maxVal;
			boolean setAllowed;
		};
	typedef struct childRecords SensorValue;

	AbstractSensorOld(uint8_t id,String name,String type,String size,String descr, uint8_t itemCount,float val=NULL){
		this->id=id;
		this->name=name;
		this->type=type;
		this->size=size;
		this->descr=descr;
		this->itemCount=itemCount;
		this->val=val;

		initializeChildren();
	}
	virtual ~AbstractSensorOld(){};

	virtual String getKind()=0;

	SensorValue* getItems(){
		return items;
	}

	SensorValue* getItem(uint8_t index){
		return &items[index];
	}

	virtual void update(){};

	virtual boolean loop(){return false;};

	virtual String getJson(uint8_t id){
		return getSensorValueJson(items[id],id);
	}

	uint8_t getId(){
		return this->id;
	}

	void setId(uint8_t){
		this->id=id;
	}

	virtual String getName() override{
		return this->name;
	}

	boolean hasNoItems(){
		return itemCount==0;
	}
	uint8_t getItemCount(){
		return itemCount;
	}
	int8_t getChildItemIndexByName(String name){
		for(uint8_t i=0;i<itemCount;i++){
			if(items[i].name==name){
				return i;
			}
		}
		return -1;
	}

	void setDescr(String descr){
		this->descr=descr;
	}
	void setDescr(uint8_t child,String descr){
		if(itemCount>child){
			this->items[child].descr=descr;
		}
	}

	void setMinVal(uint8_t child,float minVal){
		if(itemCount>child){
			this->items[child].minVal=minVal;
		}
	}

	void setMaxVal(uint8_t child,float maxVal){
		if(itemCount>child){
			this->items[child].maxVal=maxVal;
		}
	}

	void setFieldId(uint8_t itemIndex,int8_t fieldId){
		if(!isItemIndexValid(itemIndex)){
			return;
		}
		this->items[itemIndex].fieldId=fieldId;
	}

	boolean isItemIndexValid(uint8_t index){
		return itemCount>index;
	}

	String getName(uint8_t index){
		return this->items[index].name;
	}

	uint8_t getFieldId(uint8_t index){
		return 	this->items[index].fieldId;
	}

	String getFieldIdStr(uint8_t index){
		return 	String(this->items[index].fieldId);
	}

	String getDescr(){
		return 	this->descr;
	}

	String getType(){
		return 	this->type;
	}

	String getSize(){
		return 	this->size;
	}

	String getDescr(uint8_t index){
		return 	this->items[index].descr;
	}

	String getValStr(uint8_t index){
		return 	String(this->items[index].val);
	}

	float getVal(){
		return 	this->val;
	}

	uint8_t getValInt(){
		return 	this->items[0].val;
	}

	uint8_t getValInt(uint8_t index){
		return 	this->items[index].val;
	}

	float getValFloat(uint8_t index){
		return this->items[index].val;
	}

	boolean getSetAllowed(){
		return 	this->setAllowed;
	}

	boolean getSetAllowed(uint8_t index){
		return 	this->items[index].setAllowed;
	}

	void setNonActiveSensorValue(String name,String descr,uint8_t fieldId,float minVal,float maxVal){
		Serial.println(FPSTR("Non Active value set not activated"));
	}

	boolean getPeriodicSend(){
		return periodicSend;
	}

	boolean getAutoCreateChannel(){
		return autoCreateChannel;
	}
/*
	String constructGetUrl(String baseUrl,String paramVal){
		if(!this->periodicSend){
				return "";
		}
			String result="";

			for(uint8_t i=0;i<this->itemCount;i++){
				uint8_t fieldId=this->getFieldId(i);
				if(fieldId!=0){
					result+=paramVal+this->getFieldIdStr(i)+"="+this->getValStr(i);
				}
			}

			if(result!=""){
				return baseUrl+result;
			}

			return "";
	}
*/
	void printValues(){
		Serial.print(FPSTR(MESSAGE_ABSTRACT_ITEM_NAME_EQ));
		Serial.print(name);
		Serial.print(FPSTR(MESSAGE_DOT_COMMA_SPACE));
		for(uint8_t i=0;i<itemCount;i++){
			Serial.print(items[i].name);
			Serial.print(FPSTR(MESSAGE_EQUALS));
			Serial.print(items[i].val);
			Serial.print(FPSTR(MESSAGE_DOT_COMMA_SPACE));
		}
		Serial.println();
	}

	boolean getStatus(){
		return status;
	}

	int16_t getIntValByIndex(uint8_t index){
		return (int16_t)items[index].val;
	}

	String getIntStringValByIndex(uint8_t index){
		return String(getIntValByIndex(index));
	}

	String getIntStringValFromFloat(float valFlt){
		uint16_t intVal=(int16_t)valFlt;
		return String(intVal);
	}

	virtual String getPath(){
		return String(NULL);
	}

	virtual boolean setVal(float val){
		if(!getSetAllowed()){
			return false;
		}

		if(this->val!=val){
			dispatch=true;
			this->val=val;
		}

		return true;
	}

	virtual boolean setVal(uint8_t index,float val){
		if(!getSetAllowed(index)){
			return false;
		}

		if(getValFloat(index)!=val){
			dispatch=true;
			this->items[index].val=val;
		}

		return true;
	}

protected:

	uint8_t id;
	String name;
	String type;
	String size;
	String descr;
	float val;
	boolean setAllowed=false;

	uint8_t itemCount;

	boolean periodicSend=true;
	boolean autoCreateChannel=true;

	SensorValue* items;
	boolean status=false;

	boolean dispatch=false;

	String constructAlarmMessage(String preffix,String currValueStr,float val,String overValueStr,float overVal){
		return " "+preffix+currValueStr+String(val)+overValueStr+String(overVal)+"<br>";
	}

	boolean checkItemForAlarm(uint8_t index){
		if(items[index].val>=items[index].maxVal
				|| items[index].val<=items[index].minVal){
			return true;
		}
		return false;
	}

	void setSetAllowed(uint8_t index,boolean val){
		items[index].setAllowed=val;
	}

	String getItemJson(){
		return "\"id\":\""+String(id)+"\","
				+"\"name\":\""+getName()+"\","
				+"\"kind\":\""+getKind()+"\","
				+"\"type\":\""+type+"\","
				+"\"size\":\""+size+"\","
				+"\"descr\":\""+descr+"\","
				+"\"itemCount\":\""+String(itemCount)+"\""+getExtraJsonItem();
	}

	String getSensorValueJson(SensorValue m,uint8_t ind){
		return "{\"id\":\""+String(m.id)+"\","
				+"\"name\":\""+m.name+"\","
				+"\"type\":\""+m.type+"\","
				+"\"size\":\""+m.size+"\","
				+"\"descr\":\""+m.descr+"\","
				+"\"val\":\""+getSensorValForJson(m.val)+"\","
				+"\"minVal\":\""+String(m.minVal)+"\","
				+"\"maxVal\":\""+String(m.maxVal)+"\","
				+"\"fieldId\":\""+String(m.fieldId)+"\""
				+getExtraJsonChild(ind)+"}";
	}

	String getSensorValueSimpleJson(SensorValue m){
		return "{\"name\":\""+m.name+"\","
				+"\"val\":\""+getSensorValForJson(m.val)+"\","
				+"\"fieldId\":\""+String(m.fieldId)+"\"}";
	}

	virtual String getSensorValForJson(float val){
		return String(val);
	}

	virtual String getExtraJsonItem(){
		return "";
	}

	virtual String getExtraJsonChild(uint8_t ind){
		return "";
	}

	void initializeChildren(){
		if(itemCount>0)
			items=new SensorValue[itemCount];
	}


};

#endif /* LIBRARIES_DEVICELIB_AbstractSensor_H_ */
