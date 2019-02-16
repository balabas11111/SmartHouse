/*
 * AbstractSensorValue.h
 *
 *  Created on: Feb 14, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_SENSORS_ABSTRACTSENSORVALUE_H_
#define LIBRARIES_DEVICELIB_SENSORS_ABSTRACTSENSORVALUE_H_

#include <Arduino.h>
#include <interfaces/ChangeDispatchable.h>
#include <entity/Entity.h>
#include <interfaces/JsonSetGetAble.h>

//Abstract SensorValue fields
const char SENSOR_FIELD_id[]    PROGMEM = "id";
const char SENSOR_FIELD_name[]  PROGMEM = "name";
const char SENSOR_FIELD_type[]  PROGMEM = "type";
const char SENSOR_FIELD_size[]  PROGMEM = "size";
const char SENSOR_FIELD_descr[] PROGMEM = "descr";
const char SENSOR_FIELD_val[]   PROGMEM = "val";

const char SENSOR_FIELD_hash[]   PROGMEM = "hash";

const char* const SENSOR_VALUE_ALL_FIELDS_GET[] PROGMEM = {
		SENSOR_FIELD_id,SENSOR_FIELD_name,SENSOR_FIELD_type,SENSOR_FIELD_size,SENSOR_FIELD_descr,SENSOR_FIELD_val,SENSOR_FIELD_hash
};

const char* const SENSOR_VALUE_STATIC_FIELDS_GET[] PROGMEM = {
		SENSOR_FIELD_id,SENSOR_FIELD_name,SENSOR_FIELD_type,SENSOR_FIELD_size,SENSOR_FIELD_descr
};

const char* const SENSOR_VALUE_VAL_FIELDS_GET[] PROGMEM = {
		SENSOR_FIELD_id,SENSOR_FIELD_name,SENSOR_FIELD_val,SENSOR_FIELD_hash
};
const char* const SENSOR_VALUE_FIELDS_SET[] PROGMEM ={
		SENSOR_FIELD_descr,SENSOR_FIELD_val
};


class AbstractSensorValue: public Entity,public JsonSetGetAble{
public:
	AbstractSensorValue(uint8_t id,String name,String type,String size,String descr,float val);
	virtual ~AbstractSensorValue(){};

	//getters
	uint8_t getId() override;
	String getName() override;

	const String& getType() const;
	const String& getSize() const;

	const String& getDescr() const;
	virtual float getVal() const;

	boolean getSetValAllowed() const;
	boolean getSetStaticAllowed() const;

	//setters
	boolean setDescr(String& descr);
	boolean setVal(float val);

	//by fieldname operations
	const char* const getFieldsAll();
	const char* const getFieldsStatic();
	const char* const getFieldsVal();
	const char* const getFieldsSetAvailable();

	const String& getByFieldName(String fieldName);
	boolean setByFieldName(String fieldName,String fieldVal);

	//HashAble impl
	virtual String getHashableString();

	//JsonSetGetAble implementations
	virtual int set(JsonObject& item) override;
	virtual int get(JsonObject& item) override;

	virtual int setStatic(JsonObject& item) override;
	virtual int getStatic(JsonObject& item) override;

	virtual int setVal(JsonObject& item) override;
	virtual int getVal(JsonObject& item) override;

protected:
	boolean setId(uint8_t id);
	boolean setName(String& name);

	boolean setType(String& type);
	boolean setSize(String& size);

	virtual int setValByFieldName(JsonObject& item,const char* const fields);
	virtual int getValByFieldName(JsonObject& item,const char* const fields);
private:
	//static fields
	String type;
	String size;

	//dynamical fields
	String descr;
	float val;

	boolean setValAllowed=false;
	boolean setStaticAllowed=false;
};

#endif /* LIBRARIES_DEVICELIB_SENSORS_ABSTRACTSENSORVALUE_H_ */
