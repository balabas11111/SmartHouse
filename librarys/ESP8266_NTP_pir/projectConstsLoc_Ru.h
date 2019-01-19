/*
 * ProjectConstsRu.h
 *
 *  Created on: 19 ���. 2019 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ESP8266_NTP_PIR_PROJECTCONSTSLOC_RU_H_
#define LIBRARIES_ESP8266_NTP_PIR_PROJECTCONSTSLOC_RU_H_

#include "Arduino.h"

#define DEVICE_LOCALE 1

//---------------------Device parameters-------------------------
#define DEVICE_DESCR "NTP часы, будильник, Температура/Влажность, Датчик движения"
#define DEVICE_LOCATION "Настройте размещение устройства"
//---------------------Device parameters-------------------------

//---------------------TimeIntervalService parameters-------------------------
const char TimeIntervalService_ServLocalVals[] PROGMEM=
",\"intervalKind\":[\"Будильник\", \"Розетка 1 - включение\"]\
,\"intervalType\":[\"Единожды\", \"Периодически\", \"Ежедневно\", \"По дням\", \"Еженедельно\", \"Ежемесячно\", \"Ежеквартально\"]\
,\"intervalState\":[\"Новый\", \"Ожидает\", \"Выполняется\", \"Окончен\", \"К удалению\", \"Неактивный\"]\
,\"dayOfWeekShort\":[\"Пн\", \"Вт\", \"Ср\", \"Чт\", \"Пт\", \"Сб\", \"Вс\"]\
,\"dayOfWeek\":[\"Понедельник\",\"Вторник\",\"Среда\",\"Четверг\",\"Пятница\",\"Суббота\",\"Воскресенье\"]";

const char* const IntervalKind_Names[] PROGMEM={
		"Будильник", "Розетка 1 - включение"
};

const char* const IntervalType_Names[] PROGMEM={
		"Единожды", "Периодически", "Ежедневно", "По дням", "Еженедельно", "Ежемесячно", "Ежеквартально"
};
const char* const IntervalState_Names[] PROGMEM={
		"Новый", "Ожидает", "Выполняется", "Окончен", "К удалению", "Неактивный"
};
//-------------------TimeIntervalService parameters--------------------------


#endif /* LIBRARIES_ESP8266_NTP_PIR_PROJECTCONSTS_EN_H_ */
