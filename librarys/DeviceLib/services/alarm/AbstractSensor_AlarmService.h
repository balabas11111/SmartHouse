/*
 * AbstractSensor_AlarmService.h
 *
 *  Created on: Feb 12, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_SERVICES_ALARM_ABSTRACTSENSOR_ALARMSERVICE_H_
#define LIBRARIES_DEVICELIB_SERVICES_ALARM_ABSTRACTSENSOR_ALARMSERVICE_H_

#include <AbstractSensorOld.h>
#include <Arduino.h>


class AbstractSensor_AlarmService {
public:
	AbstractSensor_AlarmService(long minAlarmInterval);
	virtual ~AbstractSensor_AlarmService();

	virtual String processAlarms(AbstractSensor** sensors,uint8_t sensorsSize);

private:
	boolean alarmMode;
	unsigned long minAlarmInterval;
	unsigned long lastAlarmTime;
};

#endif /* LIBRARIES_DEVICELIB_SERVICES_ALARM_ABSTRACTSENSOR_ALARMSERVICE_H_ */
