/*
 * AbstractSensorService.h
 *
 *  Created on: Feb 8, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_SERVICES_ABSTRACTSENSORSERVICE_H_
#define LIBRARIES_DEVICELIB_SERVICES_ABSTRACTSENSORSERVICE_H_

#include <sensors/AbstractSensorList.h>
#include <Arduino.h>
#include <ArduinoJson.h>

#include <ESP_Consts.h>
#include <interfaces/EntityServiceBase.h>
#include <StatusMessage/StatusMessage.h>

//File path constants
const PROGMEM char ABSTRACT_SENSORS_BASE_PATH[]="/settings/sensors/";
const PROGMEM char TXT[]=".txt";
const PROGMEM char SLASH[]="/";

class AbstractSensorService: public EntityServiceBase {
public:
	AbstractSensorService(AbstractSensorList* sensors);
	virtual ~AbstractSensorService();

	//boolean startSensors();

	AbstractSensorList* getSensors();
	uint8_t getSensorsCount();

	uint8_t getEntityId();

	void printAbstractSensorServiceDetails();

	//AbstractSensor* getSensorByName(String sensorName);
	//AbstractSensor* getSensorById(uint8_t sensorId);

	int getEntityIdByName(String name);

	void updateSensors();

	boolean loadSensors();
	boolean saveSensors();

	StatusMessage putAbstractSensorFromJson(String json,boolean setDescr,boolean setVals,boolean save);
	boolean loadAbstractSensorFromFile(AbstractSensor* sensor);
	boolean saveAbstractSensorToFile(AbstractSensor* sensor);

	static String getAbstractSensorFilePath(AbstractSensor* sensor);
	static String getAbstractSensorFileFolder(AbstractSensor* sensor);

	//Json operation functions
	virtual int getAbstractItems(JsonArray& items,uint8_t pageId) override;
	virtual int postAbstractItems(JsonArray& items,uint8_t pageId) override;

protected:
	boolean loadSensorListFromFile(AbstractSensorList*);
	boolean saveSensorListToFile(AbstractSensorList*);

	static String getAbstractSensorFilePreffix(AbstractSensor* sensor);

private:
	AbstractSensorList* sensors;
};

#endif /* LIBRARIES_DEVICELIB_SERVICES_ABSTRACTSENSORSERVICE_H_ */
