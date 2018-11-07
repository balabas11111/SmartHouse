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
const char LIGHT_RU[] PROGMEM ="Освещение";

const char MEASURE_CELSIUS_DEGREES_RU[] PROGMEM ="°C";
const char MEASURE_PERSENT_RU[] PROGMEM ="%";
const char MEASURE_PASCAL_RU[] PROGMEM ="па";
const char MEASURE_METER_RU[] PROGMEM ="м";
const char MEASURE_LIGHT_RU[] PROGMEM ="Люкс";

const char TEMPERATURE[] PROGMEM ="Temperature";
const char HUMIDITY[] PROGMEM ="Humidity";
const char PRESSURE[] PROGMEM ="Pressure";
const char ALTITUDE[] PROGMEM ="Altitude";
const char LIGHT[] PROGMEM ="Light";

const char TEMPERATURE_DHT22[] PROGMEM ="Temperature_DHT22";
const char HUMIDITY_DHT22[] PROGMEM ="Humidity_DHT22";

const char MEASURE_CELSIUS_DEGREES[] PROGMEM ="°C";
const char MEASURE_PERSENT[] PROGMEM ="%";
const char MEASURE_PASCAL[] PROGMEM ="Pa";
const char MEASURE_METER[] PROGMEM ="m";
const char MEASURE_LIGHT[] PROGMEM ="Lux";

//const PROGMEM char HTML_BH1750_MEASURER[] = ""
//widget actions
const PROGMEM char ACTION_GET_STATIC_SETTINGS_DATA[]="getStSettData";       //returns static data from espSettings
const PROGMEM char ACTION_GET_WIDGET_HTML_OR_VAL[]="getWidgetHtml";       //returns all components text
const PROGMEM char ACTION_GET_ALL_WIDGET_JSON[]="getAllWidgetsJson";
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
//status messages
const PROGMEM char MESSAGE_STATUS_JSON_WIDGET_NOT_FOUND[]="{\"status\":\"widgetNotFound\"}";
const PROGMEM char MESSAGE_STATUS_JSON_ACTION_NOT_ALLOWED[]="{\"status\":\"ACTION_NOT_ALLOWED_On_ThisWidget\"}";
const PROGMEM char MESSAGE_STATUS_JSON_PARAMETERS_MISSING[]="{\"status\":\"One of req params miss actionName,widgetName\"}";
const PROGMEM char MESSAGE_STATUS_JSON_EMPTY_REQUIRED_PARAM[]="{\"status\":\"One of req params empty actionName,widgetName\"}";

//content types
const PROGMEM char CONTENT_TYPE_TEXT_HTML[]="text/html";
const PROGMEM char CONTENT_TYPE_JSON[]="application/json";

//serial println constants

const PROGMEM char MESSAGE_STATUS_EQUAL[]="Status=";
const PROGMEM char MESSAGE_EMPTY_STRING[]="";
const PROGMEM char MESSAGE_SPACE[]=" ";
const PROGMEM char MESSAGE_EQUALS[]="=";
const PROGMEM char MESSAGE_DOT_COMMA[]=";";
const PROGMEM char MESSAGE_DONE[]="...done";
const PROGMEM char MESSAGE_HORIZONTAL_LINE[]="------------------------------";

//serial println wifi constants
const PROGMEM char MESSAGE_HANDLE_HTTP_WIDGET[]="----------Handle Http widget----------";

//serial println device diagnostic constants
//before 16936 17048 17696
const PROGMEM char MESSAGE_DEVICE_DIAGNOSTIC_BEGIN[]="----------Device diagnostic----------";
const PROGMEM char MESSAGE_DEVICE_FREE_HEAP[]="FreeHeap";
const PROGMEM char MESSAGE_DEVICE_CPU_MHZ[]="CpuFreqMHz";
const PROGMEM char MESSAGE_DEVICE_FLASH_CHIP_SIZE[]="FlashChipSize";
const PROGMEM char MESSAGE_DEVICE_FREE_SCETCH_SPACE[]="FreeSketchSpace";
const PROGMEM char MESSAGE_DEVICE_RESET_REASON[]="ResetReason";

const PROGMEM char MESSAGE_DEVICE_HELPER_UPDATE_EXECUTION[]="----------Device helper UpdateExecution----";
const PROGMEM char MESSAGE_DEVICE_HELPER_STARTED[]="----------Device helper----------";
const PROGMEM char MESSAGE_DEVICE_HELPER_INITIALIZING[]="----------Device helper init----------";
const PROGMEM char MESSAGE_DEVICE_HELPER_LOOPERS[]="----------Device helper loopers----------";
const PROGMEM char MESSAGE_DEVICE_HELPER_LOOP_SIZE[]="DeviceHelper loop size=";
const PROGMEM char MESSAGE_DEVICE_START_DEVICE_ID[]="Start DeviceId=";
const PROGMEM char MESSAGE_DEVICE_INIT_STARTED_COUNT[]="Init started count=";
const PROGMEM char MESSAGE_DEVICE_INIT_COUNT[]="Initialized count=";

//----------EspSettingsBox Messages---------------
const PROGMEM char MESSAGE_ESPSETTINGSBOX_ABSTRACT_ITEM_SAVE_BEGIN[]="Save AbstractItem to file=";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_SAVED[]="...saved";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_DEFAULT_VALUES_SAVED[]="Default settings will be saved";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_ERROR_FILE_NOT_EXISTS[]="File not exists";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_ERROR_FILE_EMPTY[]="File is empty";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_ERROR_PARSE_JSON[]="Error parse JSON file";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_VALUE_PARSED[]="values parsed";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_NO_SETTINGS_FILE_EXISTS[]="No settings file exists";

const PROGMEM char MESSAGE_ESPSETTINGSBOX_PRINT_SETTINGS_FILE[]="-----------Print settings file----------------";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_CURRENT_MEMORY_STATE[]="Current memory state";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_DIRECT_FILE_READ[]="----------------direct file read------------";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_SETTINGS_FROM_MEMORY[]="-----------------Settings from memory-------";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_SAVE_DEVICE_SETTINGS_TO_FILE[]="Save device settings to file";

const PROGMEM char MESSAGE_ESPSETTINGSBOX_SETTINGS_TO_MEMORY[]="-----------------Settings loaded to memory-------";

const PROGMEM char MESSAGE_ESPSETTINGSBOX_BEGIN_SAVE[]=" Begin save";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_FILE_SAVED[]="...saved";

const PROGMEM char MESSAGE_ESPSETTINGSBOX_FILE_EXISTS[]=" file exists";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_FILE_MISSED[]=" file MISSED";

const PROGMEM char MESSAGE_ESPSETTINGSBOX_BEGIN_INITIALIZE[]="Begin initialize of EspSettingsBox";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_ALREADY_INITIALIZE[]="EspSettingsBox already initialized";

const PROGMEM char MESSAGE_ESPSETTINGSBOX_LOADED[]="EspSettingsBox settings loaded=";

const PROGMEM char MESSAGE_ESPSETTINGSBOX_SPIFFS_INFO[]="--------------SPIFFS INFO--------------";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_INITIALIZED_EQ[]="Initialized=";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_SPIFF_INIT_EQ[]="spiffInitialized=";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_BLOCK_SIZE[]="blockSize=";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_MAX_OPEN_FILES[]="maxOpenFiles=";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_MAX_PATH_LENGTH[]="maxPathLength=";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_PAGE_SIZE[]="pageSize=";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_TOTAL_BYTES[]="totalBytes=";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_USED_BYTES[]="usedBytes=";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_NAME_EQ[]="name=";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_SIZE_EQ[]="size=";

//ESPSettings box defaults
const PROGMEM char ESPSETTINGSBOX_SETTINGS_DEPLOY_PATH[]="/settings";
const PROGMEM char ESPSETTINGSBOX_SETTINGS_PATH[]="/settings/";
const PROGMEM char ESPSETTINGSBOX_SETTINGS_DEFAULT_FILE[]="settings.txt";
const PROGMEM char ESPSETTINGSBOX_SETTINGS_TXT_EXT[]=".txt";

//WiFi deploy folder
const PROGMEM char ESPSETTINGSBOX_DEFAULT_PAGE[]="/web/index.htm";
const PROGMEM char ESPSETTINGSBOX_DEFAULT_WEB_FOLDER[]="/web/";
const PROGMEM char ESPSETTINGSBOX_DEPLOY_EXT[]="html,htm,js,css,jpeg,png,jpg,gif,ttf,woff,woff2,tpl,txt";

//Wifi helper messages-----------------------------
const PROGMEM char MESSAGE_WIFIHELPER_INIT_CONNECTION[]="------------------Init WiFi connection------------------";
const PROGMEM char MESSAGE_WIFIHELPER_WIFI_DISCONNECTING[]="WiFi disconnecting";
const PROGMEM char MESSAGE_WIFIHELPER_SETUP_SERVER[]="SetupServer";
const PROGMEM char MESSAGE_WIFIHELPER_WIFI_MODE_EQ[]="WiFi.getMode=";
const PROGMEM char MESSAGE_WIFIHELPER_GET_AUTO_CONNECT[]=" WiFi.getAutoConnect=";
const PROGMEM char MESSAGE_WIFIHELPER_WIFI_STATUS_EQ[]=" WiFi.status=";
const PROGMEM char MESSAGE_WIFIHELPER_POST_INIT_WEB_SERV_HANDLERS[]="--------------------PostInit Web server handlers---------";
const PROGMEM char MESSAGE_WIFIHELPER_SERVER_SETUP_COMPLETED[]="Server setup completed";

const PROGMEM char MESSAGE_WIFIHELPER_DEPLOYING_PATH[]="Deploying Path=";
const PROGMEM char MESSAGE_WIFIHELPER_AS_WEB_FILES[]=" as web files";
const PROGMEM char MESSAGE_WIFIHELPER_EXTENSIONS[]="Extensions ";
const PROGMEM char MESSAGE_WIFIHELPER_TO_BE_DEPLOYED[]=" to be deployed";
const PROGMEM char MESSAGE_WIFIHELPER_BASE_URL_EQ[]="Base URL ";
const PROGMEM char MESSAGE_WIFIHELPER_ADDED_FILE[]="Added file ";
const PROGMEM char MESSAGE_WIFIHELPER_SIZE_EQ[]=" size=";
const PROGMEM char MESSAGE_WIFIHELPER_EXT_EQ[]=" ext=";
const PROGMEM char MESSAGE_WIFIHELPER_URL_EQ[]="   URL=";

//Ntp clock client
const PROGMEM char MESSAGE_TIME_CLIENT_JSON_BEFORE_TIME[]="{\"wsId\":\"clock\",\"wsClass\":\"wsItem\",\"wsValue\":\"";
const PROGMEM char MESSAGE_TIME_CLIENT_JSON_AFTER_TIME[]="\"}";

//device widgets snapshot TODO: add this
const PROGMEM char DEVICE_WIDGETS_SNAPSHOT[]="----------Device helper----------";

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
