/*
 * AbstractSensor.h
 *
 *  Created on: Feb 14, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_SENSORS_ABSTRACTSENSOR_H_
#define LIBRARIES_DEVICELIB_SENSORS_ABSTRACTSENSOR_H_

#include <Arduino.h>
#include <ArduinoJson.h>

#include <interfaces/Entity.h>
#include <interfaces/Loopable.h>

#include <sensors/AbstractSensorValue.h>
#include <sensors/AbstractSensorValueList.h>
/*
const char SENSOR_FIELD_fieldId[]    PROGMEM = "fieldId";
const char SENSOR_FIELD_minVal[]     PROGMEM = "minVal";
const char SENSOR_FIELD_maxVal[]     PROGMEM = "minVal";
const char SENSOR_FIELD_setAllowed[] PROGMEM = "setAllowed";
*/
//Abstract sensor Statuses;
enum SensorStatus: uint8_t {
	SensorStatus_notInit, SensorStatus_init, SensorStatus_initComplete, SensorStatus_ok, SensorStatus_failure
};

//Abstract sensor Kinds
const char SENSOR_KIND_pinDigital[] PROGMEM = "pinDigital";
const char SENSOR_KIND_sensor[]     PROGMEM = "sensor";

class AbstractSensor: public Entity{
public:
	AbstractSensor(uint8_t id,String name,String type,String size,String descr, uint8_t childCount,float val=NULL);
	virtual ~AbstractSensor(){};

	virtual uint8_t getId() override;
	virtual String getName() override;

	virtual boolean getChanged() override;
	virtual String getHashableString();

	AbstractSensorValue* getHeader();
	AbstractSensorValue* getChild(int index);

	AbstractSensorValueList* getChilds();

	uint8_t getStatus();

	virtual int set(JsonObject& item) override;
	virtual int get(JsonObject& item) override;

protected:
	void initChildren(int childCount);

private:
	uint8_t curStatus;
	AbstractSensorValue* header;

	AbstractSensorValueList* childs;
};

#endif /* LIBRARIES_DEVICELIB_SENSORS_ABSTRACTSENSOR_H_ */
