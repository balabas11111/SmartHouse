/*
 * ConstsLocRu.h
 *
 *  Created on: 19 ���. 2019 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_LOCALES_CONSTSLOC_RU_H_
#define LIBRARIES_DEVICELIB_LOCALES_CONSTSLOC_RU_H_

#include "Arduino.h"
/*----------------------------CommonValues--------------------*/

const char* const DAYS_OF_WEEK_SHORT[] PROGMEM={
		"Пн", "Вт", "Ср", "Чт", "Пт", "Сб", "Вс"
};

const char* const DAYS_OF_WEEK[] PROGMEM={
		"Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота", "Воскресенье"
};
/*----------------------------CommonValues--------------------*/

/*----------------------------StatusMessage--------------------*/
const PROGMEM char MESSAGE_PROV_NOT_FOUND[]  = "Компонент провайдер не найден:";
const PROGMEM char MESSAGE_PROC_NOT_FOUND[]  = "Компонент процессор не найден:";
const PROGMEM char STATUS_UNKNOWN_DESCR[]  = "Статус не определен";

const char* const STATUSES_DESCR[] PROGMEM=
{
		"Ok",
		"Ошибка",
		"Не найдено",
		"Неверная длинна",
		"Ошибка парсинга",
		"Ошибка обновления",
		"Параметр не найден",
		"Ошибка проверки",
		"Сущность не найдена",
		"Не удалось",
		"Ошибка отправки",
		"Не определено",
		"Принято",
		"Сохранено",
		"Удалено",
		"Функция отключена",
		"Данные отправлены",
		"Нечего отправлять",
		"Ошибка конфигурации"
};
/*----------------------------StatusMessage--------------------*/

/*----------------------------ThingSpeak Settings--------------------*/

const PROGMEM char MESSAGE_THINGSPEAK_NO_SENSORS[]="Отсутствуют датчики";
const PROGMEM char MESSAGE_THINGSPEAK_PUBLISH_NOT_ALLOWED[]="Публикация данных ThingSpeak не разрешена";
const PROGMEM char MESSAGE_THINGSPEAK_NO_USER_SPECIFIED[]="Пользователь, пароль, ключ ThingSpeak не указаны";

/*----------------------------ThingSpeak Settings--------------------*/


#endif /* LIBRARIES_DEVICELIB_LOCALES_CONSTSLOC_RU_H_ */
