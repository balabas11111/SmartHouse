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

//sensors constants
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

//const PROGMEM char HTML_BH1750_MEASURER[] = ""
//widget actions
const PROGMEM char ACTION_GET_STATIC_SETTINGS_DATA[]="getStSettData";       //returns static data from espSettings
const PROGMEM char ACTION_GET_WIDGET_HTML_OR_VAL[]="getWidgetHtml";       //returns all components text
const PROGMEM char ACTION_GET_WIDGET_JSON[]="getWidgetJson";       //returns all components text
const PROGMEM char ACTION_GET_WIDGETS_CHILDREN_AS_JSON[]="getWidgetChildrenAsJson";       //returns all components children as json
const PROGMEM char ACTION_SUBMIT_FORM_GET_WIDGETS_CHILDREN_AS_JSON[]="submitFormGetChildrenAsJson";       //used to process data from client
const PROGMEM char ACTION_SUBMIT_WIDGET_GET_VALUE[]="submitAndGetWidget"; //used to process data from client
//url constants
const PROGMEM char URL_REMOTE_GET_WIDGETS[]="handleHttpWidget";       //url to reload httpWidgets on client
const PROGMEM char URL_PROCESS_EVENTS[]="processEvent";       //url to process incoming pin events

const PROGMEM char  URL_REMOTE_GET_WIDGETS_METHOD[]="POST";       //method of remote call
//base constants
const PROGMEM char PARAM_ACTION_ID[]="actionName";       //request action, which to be executed on ESP
const PROGMEM char PARAM_REMOTE_ID[]="widgetName";       //request parameter identifies id of entity on ESP board
const PROGMEM char PARAM_REMOTE_VAL[]="remoteVal";       //request parameter which remote value is required
const PROGMEM char PARAM_CLASS_NAME[]="className";       //request parameter identifies class of customers control
const PROGMEM char PARAM_CHILD_CLASS[]="childClassName";       //request parameter identifies child class of customer control to update
const PROGMEM char PARAM_CLIENT_DATA[]="clientData";
//classess
const PROGMEM char CLASS_REFRESHABLE_SettingsWidgetESP[]="SettingsWidgetESP";       //used to auto update espsettings
const PROGMEM char CLASS_REFRESHABLE_MeasurerWidgetESP[]="MeasurerWidgetESP";       //used to update whole html
const PROGMEM char CLASS_REFRESHABLE_CHILDREN_MeasurerWidgetESPJson[]="MeasurerWidgetESPJson"; //used to update form values by json
const PROGMEM char CLASS_REFRESHABLE_MeasurerWidgetESPJsonStatus[]="MeasurerWidgetESPJsonStatus"; //class for display status of ajax
const PROGMEM char CLASS_REFRESHABLE_IMAGE[]="refreshableImage";

const PROGMEM char CLASS_REFRESHABLE_CHILD[]="refreshableChild"; //view of returnable json
//attributes
const PROGMEM char ATTRIBUTE_REMOTE_ID[]="remoteId";       //remoteId tag. SPecifies widgetName on ESP
const PROGMEM char ATTRIBUTE_REMOTE_VAL[]="remoteVal";     //whichValueSelect from received JSON
const PROGMEM char ATTRIBUTE_RELOAD_INTERVAL[]="reloadInterval";       //used to auto reload some component
const PROGMEM char ATTRIBUTE_TARGET_TAG[]="targetTag";                 //tag which should be updated
const PROGMEM char ATTRIBUTE_TARGET_TAG_REPLACE_PART[]="targetTagReplacePart";  //if partOf tag should be updated
//Some stamdard values
const PROGMEM char REMOTE_VAL_VALUE[]="value";
const PROGMEM char REMOTE_VAL_IMAGE[]="Image";

const PROGMEM char WS_TAG_ID[]="wsId";
const PROGMEM char WS_TAG_VALUE[]="wsValue";
const PROGMEM char WS_TAG_PARENT[]="wsParent";
const PROGMEM char WS_TAG_CHILD[]="wsChild";
const PROGMEM char WS_TAG_ITEM[]="wsItem";
//const PROGMEM char VIEW_NAME_jsonFormView[]="jsonFormView"; //view of returnable json
//base controls
const char HTML_B_OPEN[] PROGMEM ="<b>";
const char HTML_B_CLOSE[] PROGMEM ="</b>";

const char HTML_H2_OPEN[] PROGMEM ="<h2>";
const char HTML_H2_CLOSE[] PROGMEM ="</h2>";

const char HTML_P_OPEN[] PROGMEM ="<p>";
const char HTML_p_CLOSE[] PROGMEM ="</p>";
//base attributes
const PROGMEM char HTML_ATTR_ID_OPEN[]                = " id=\"";
const PROGMEM char HTML_ATTR_NAME_OPEN[]              = " name=\"";
const PROGMEM char HTML_ATTR_TYPE_OPEN[]              = " type=\"";
const PROGMEM char HTML_ATTR_VALUE_OPEN[]             = " value=\"";
const PROGMEM char HTML_ATTR_CLASS_OPEN[]             = " class=\"";
const PROGMEM char HTML_ATTR_STYLE_OPEN[]             = " style=\"";
const PROGMEM char HTML_ATTR_CLOSE[]                  = "\"";
const PROGMEM char HTML_ATTR_SPACE[]                  = " ";

//class names W3
const PROGMEM char HTML_CLASS_W3card4[]               = "w3-card-4";
const PROGMEM char HTML_CLASS_W3container[]           = "w3-container";
const PROGMEM char HTML_CLASS_W3teal[]                = "w3-teal";
const PROGMEM char HTML_CLASS_W3border[]              = "w3-border";
const PROGMEM char HTML_CLASS_W3input[]               = "w3-input";

const PROGMEM char HTML_CLASS_W3containerTeal[]       = "w3-container w3-teal";
//styles
const PROGMEM char HTML_STYLE_width25_perc[]          = "width:25%;";
const PROGMEM char HTML_STYLE_float_left[]            = "float:left;";
//DIV section
const PROGMEM char HTML_DIV_OPEN[]                    = "<div>";
const PROGMEM char HTML_DIV_OPEN_NO_END_QUOTE[]       = "<div";
const PROGMEM char HTML_DIV_CLOSE[] PROGMEM           = "</div>";
//form
const PROGMEM char HTML_FORM_OPEN[]                   = "<form>";
const PROGMEM char HTML_FORM_OPEN_NO_END_QUOTE[]      = "<form";
const PROGMEM char HTML_FORM_CLOSE[]                  = "</form>";
//label
const PROGMEM char HTML_LABEL_OPEN[]                  = "<label>";
const PROGMEM char HTML_LABEL_OPEN_NO_END_QUOTE[]     = "<label";
const PROGMEM char HTML_LABEL_CLOSE[]                 = "</label>";
//input
const PROGMEM char HTML_INPUT_OPEN[]                  = "<input>";
const PROGMEM char HTML_INPUT_OPEN_NO_END_QUOTE[]     = "<input";
const PROGMEM char HTML_INPUT_CLOSE[]                 = "</input>";
//id helper constants
const PROGMEM char ID_STATUS_SUFFIX[]="_status";
const PROGMEM char STATUS_OK_CLASS[]="w3-panel w3-green w3-card-4";
const PROGMEM char STATUS_ERROR_CLASS[]="w3-panel w3-red w3-card-4";
//Message constants
const PROGMEM char MESSAGE_REFRESH[]="Обновляю";
const PROGMEM char MESSAGE_NOT_FOUND[]="Не найдено";
const PROGMEM char MESSAGE_ERROR[]="ОШИБКА";
const PROGMEM char MESSAGE_REQUEST_ERROR[]="ОШИБКА запроса";
const PROGMEM char MESSAGE_REMOTE_ERROR[]="Удаленная ошибка";
const PROGMEM char MESSAGE_SERVER_UNAVAILABLE[]="Устройство недоступно";
const PROGMEM char MESSAGE_SENSOR_NOT_FOUND[]="Датчик не найден";
const PROGMEM char MESSAGE_SENSOR_BROKEN[]="Датчик неисправен";
const PROGMEM char MESSAGE_DEVICE_BROKEN[]="Устройство неисправно";

const PROGMEM char MIN_TIMEOUT_VALUE_SENSOR[]="2000";

//--------------------------------------------------------

const PROGMEM char HTML_BME280[]=
		"<div class=\"w3-card-4\" style=\"width:25%;  float:left; min-width: 250px; min-height:270px; margin-left: 10px; margin-top: 10px; margin-right: 10px; margin-bottom: 10px;\">\
			<div class=\"w3-container w3-teal\">\
				  <h1><div>Климат</div></h1>\
			</div>\
			<form id=\"bmeMeasurer\" remoteId=\"bmeMeasurer\" reloadInterval=\"120000\" class=\"MeasurerWidgetESPJson ReloadableWidget w3-container\">\
				<label style=\"max-width: 240px;\"><b>Температура °C</b></label>\
				<input id=\"Temperature\" remoteId=\"Temperature\" targetTag=\"value\" class=\"refreshableChild w3-input w3-border\" style=\"width:95%\" type=\"text\" value=\"\" disabled>\
				<label style=\"max-width: 240px;\"><b>Давление (Паскаль)</b></label>\
				<input id=\"Pressure\" remoteId=\"Pressure\"  targetTag=\"value\" class=\"refreshableChild w3-input w3-border\" style=\"width:95%\" type=\"text\" value=\"\" disabled>\
				<label style=\"max-width: 240px;\"><b>Влажность (%)</b></label>\
				<input id=\"Humidity\" remoteId=\"Humidity\" targetTag=\"value\" class=\"refreshableChild w3-input w3-border\" style=\"width:95%\" type=\"text\" value=\"\" disabled>\
				<label style=\"max-width: 240px;\"><b>Высота над морем (метров)</b></label>\
				<input id=\"Altitude\" remoteId=\"Altitude\" targetTag=\"value\" class=\"refreshableChild w3-input w3-border\" style=\"width:95%\" type=\"text\" value=\"\" disabled>\
				<p></p>\
				<div id=\"bmeMeasurer_status\" class=\"w3-panel w3-green w3-card-4\"><p>Загружаю</p></div>\
			</form>\
		</div>";

const PROGMEM char HTML_BH1750[]=
		"<div class=\"w3-card-4\" style=\"width:25%;  float:left; min-width: 250px; min-height:270px; margin-left: 10px; margin-top: 10px; margin-right: 10px; margin-bottom: 10px;\">\
			<div class=\"w3-container w3-teal\">\
				  <h2>Освещение</h2>\
			</div>\
			<form id=\"luxMeasurer\" remoteId=\"luxMeasurer\" reloadInterval=\"120000\" class=\"MeasurerWidgetESPJson ReloadableWidget w3-container\">\
				<label style=\"max-width: 240px;\"><b>Освещение (Люкс)</b></label>\
				<input id=\"light\" remoteId=\"light\" targetTag=\"value\" class=\"refreshableChild w3-input w3-border\" style=\"width:95%\" type=\"text\" value=\"\" disabled>\
				<p></p>\
				<div id=\"luxMeasurer_status\" class=\"MeasurerWidgetESPJsonStatus w3-panel w3-green w3-card-4\"><p>Загружаю</p></div>\
			</form>\
		</div>";


#endif /* LIBRARIES_MEASURER_MEASURERCONSTS_H_ */