/*
 * Measurer.h
 *
 *  Created on: 14 Ã®ÃªÃ². 2018 Ã£.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_MEASURER_MEASURERCONSTS_H_
#define LIBRARIES_MEASURER_MEASURERCONSTS_H_

#define VAR_NAME(var) #var

#include "Arduino.h"


const char TEMPERATURE_RU[] PROGMEM ="Температура";
const char HUMIDITY_RU[] PROGMEM ="Влажность";
const char PRESSURE_RU[] PROGMEM ="Давление";
const char ALTITUDE_RU[] PROGMEM ="Высота";

const char MEASURE_CELSIUS_DEGREES_RU[] PROGMEM ="°C";
const char MEASURE_PERSENT_RU[] PROGMEM ="%";
const char MEASURE_PASCAL_RU[] PROGMEM ="па";
const char MEASURE_METER_RU[] PROGMEM ="м";

const char TEMPERATURE[] PROGMEM ="Temperature";
const char HUMIDITY[] PROGMEM ="Humidity";
const char PRESSURE[] PROGMEM ="Pressure";
const char ALTITUDE[] PROGMEM ="Altitude";

const char MEASURE_CELSIUS_DEGREES[] PROGMEM ="°C";
const char MEASURE_PERSENT[] PROGMEM ="%";
const char MEASURE_PASCAL[] PROGMEM ="Pa";
const char MEASURE_METER[] PROGMEM ="m";

const PROGMEM char OPEN_HEADER[] = "<div class=\"w3-card-4\" style=\"width:25%;  float:left; min-width: 250px; min-height:270px; margin-left: 10px; margin-top: 10px; margin-right: 10px; margin-bottom: 10px;\">	<div class=\"w3-container w3-teal\"><h2>";
const PROGMEM char ClOSE_HEAD_OPEN_CONT[] = "</h2></div><form class=\"w3-container\">";
const PROGMEM char HTML_P_FORM_DIV_CLOSE[] = "<p></p></form></div>";

const PROGMEM char HTML_LABEL_OPEN_WITH_B[] =
		"<label style=\"max-width: 240px;\"><b>";
const PROGMEM char HTML_LABEL_CLOSE_WITH_B_INPUT[] =
		"</b></label><input class=\"w3-input w3-border\" style=\"width:95%\" name=\"";
const PROGMEM char HTML_INPUT_OPEN_AFTER_NAME_TILL_VALUE[] =
		"\" type=\"text\" value=\"";
const PROGMEM char HTML_INPUT_CLOSE_WITH_B[] =
		"\" disabled>";

#endif /* LIBRARIES_MEASURER_MEASURERCONSTS_H_ */
