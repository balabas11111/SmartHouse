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

//Robot Dyn//vem  // 12E
/*
#define A0_PIN 17
#define D0_PIN 16 //GPIO016 ////beeper
#define D1_PIN 5  //GPIO05  //DallasTemp           PIN_WIRE_SCL
#define D2_PIN 4  //GPIO04  //OLED //SDA //blue    PIN_WIRE_SDA
#define D3_PIN 0  //GPIO00  //OLED //SCL //green
#define D4_PIN 2  //GPIO02  //RedLed               INTERNAL_LED_PIN
#define D5_PIN 14 //GPIO14  //DHT
#define D6_PIN 12 //GPIO12  //DallasTemp red led2
#define D7_PIN 13 //GPIO13  //GreenLed
#define D8_PIN 15 //GPIO15  //button
#define SD3_PIN 10
#define SD2_PIN 9

#define RX_PIN 3
#define TX_PIN 1
*/

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
const PROGMEM char CONTENT_TYPE[]="Content-Type";
const PROGMEM char CONTENT_TYPE_TEXT_HTML[]="text/html";
const PROGMEM char CONTENT_TYPE_JSON[]="application/json";
const PROGMEM char CONTENT_TYPE_FORM_URL_ENCODED[]="application/x-www-form-urlencoded";
const PROGMEM char CONTENT_TYPE_TEXT_PLAIN[]="text/plain";

const PROGMEM char CONTENT_TYPE_APPLICATION_OCTED_STREAM[]="application/octet-stream";
const PROGMEM char CONTENT_TYPE_TEXT_CSS[]="text/css";
const PROGMEM char CONTENT_TYPE_APPLICATION_JAVASCRIPT[]="application/javascript";
const PROGMEM char CONTENT_TYPE_IMAGE_PNG[]="image/png";
const PROGMEM char CONTENT_TYPE_IMAGE_GIF[]="image/gif";
const PROGMEM char CONTENT_TYPE_IMAGE_JPEG[]="image/jpeg";
const PROGMEM char CONTENT_TYPE_IMAGE_XICON[]="image/x-icon";
const PROGMEM char CONTENT_TYPE_TEXT_XML[]="text/xml";
const PROGMEM char CONTENT_TYPE_APPLICATION_XPDF[]="application/x-pdf";
const PROGMEM char CONTENT_TYPE_APPLICATION_XZIP[]="application/x-zip";
const PROGMEM char CONTENT_TYPE_APPLICATION_XGZIP[]="application/x-gzip";

const PROGMEM char CONTENT_TYPE_JSON_UTF8[]="application/json; charset=UTF-8";
//serial println constants

const PROGMEM char MESSAGE_STATUS_EQUAL[]="Status=";
const PROGMEM char MESSAGE_EMPTY_STRING[]="";
const PROGMEM char MESSAGE_SPACE[]=" ";
const PROGMEM char MESSAGE_EQUALS[]="=";
const PROGMEM char MESSAGE_DOT_COMMA[]=";";
const PROGMEM char MESSAGE_DOT_COMMA_SPACE[]="; ";
const PROGMEM char MESSAGE_OPEN_BRACE[]="(";
const PROGMEM char MESSAGE_CLOSE_BRACE[]=")";
const PROGMEM char MESSAGE_DIVIDE[]="/";
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

const PROGMEM char MESSAGE_DEVICE_HELPER_UPDATE_EXECUTION[]="----------Device helper Update Sensor values----";
const PROGMEM char MESSAGE_DEVICE_HELPER_STARTED[]="----------Device helper----------";
const PROGMEM char MESSAGE_DEVICE_HELPER_INITIALIZING[]="----------Device helper init----------";
const PROGMEM char MESSAGE_DEVICE_HELPER_LOOPERS[]="----------Device helper loopers----------";
const PROGMEM char MESSAGE_DEVICE_HELPER_LOOP_SIZE[]="DeviceHelper loop size=";
const PROGMEM char MESSAGE_DEVICE_START_DEVICE_ID[]="Start DeviceId=";
const PROGMEM char MESSAGE_DEVICE_INIT_STARTED_COUNT[]="Init started count=";
const PROGMEM char MESSAGE_DEVICE_INIT_COUNT[]="Initialized count=";


const PROGMEM char MESSAGE_DEVICE_STARTED[]="=========================Device Started=========================";
//alarm constants
const PROGMEM char MESSAGE_DEVICE_HELPER_CHECK_FOR_ALARM[]="----------Check for alarm----";
const PROGMEM char MESSAGE_DEVICE_HELPER_ALARM_MODE_IDENTIFIED[]="Alarm identified";
const PROGMEM char MESSAGE_DEVICE_HELPER_ALARM_MODE_NOT_IDENTIFIED[]="No Alarm";
const PROGMEM char MESSAGE_DEVICE_HELPER_ALARM_MODE_STARTED[]="Alarm started";
const PROGMEM char MESSAGE_DEVICE_HELPER_ALARM_MODE_FINISHED[]="Alarm finished";

const PROGMEM char MESSAGE_DEVICE_HELPER_ALARM_MODE_FINISHED_RESULT[]="Режим тревоги прекращен. Все показатели в норме";

//----------EspSettingsBox Messages---------------
const PROGMEM char MESSAGE_ESPSETTINGSBOX_ABSTRACT_ITEM_SAVE_BEGIN[]="Save AbstractItem to file=";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_SAVED[]="...saved";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_DEFAULT_VALUES_SAVED[]="Default settings will be saved";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_ERROR_FILE_NOT_EXISTS[]="File not exists";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_ERROR_FILE_EMPTY[]="File is empty";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_ERROR_PARSE_JSON[]="Error parse JSON file";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_VALUE_PARSED[]="val read=";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_READ_FROM_FILE_COMPLETE[]="read from file complete";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_ITEM_SETTINGS_LOADED[]="val put=";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_NO_SETTINGS_FILE_EXISTS[]="No settings file exists";

const PROGMEM char MESSAGE_ESPSETTINGSBOX_TOTAL_CHILDS_EQ[]="Total childs=";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_FILE_SYSTEM_BEGIN_INIT[]="File system begin init";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_FILE_SYSTEM_STATUS[]="File system status ";

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
const PROGMEM char MESSAGE_ESPSETTINGSBOX_SIZE_EQ[]="size=";

const PROGMEM char MESSAGE_ESPSETTINGSBOX_NAME_EQ[]="name=";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_DESCR_EQ[]="descr=";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_FIELD_ID_EQ[]="fieldId=";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_MIN_VAL_EQ[]="minVal=";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_MAX_VAL_EQ[]="maxVal=";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_QUEUE_EQ[]="queue=";

//ESPSettings box defaults
const PROGMEM char ESPSETTINGSBOX_NAME[]="espSettingsBox";
const PROGMEM char ESPSETTINGSBOX_GET_SIMPLE_JSON_PUBLISH_URL[]="/espSettingsBox/getSimpleJson";
const PROGMEM char ESPSETTINGSBOX_GET_JSON_PUBLISH_URL[]="/espSettingsBox/getJson";
const PROGMEM char ESPSETTINGSBOX_SET_JSON_PUBLISH_URL[]="/espSettingsBox/setValue";

const PROGMEM char ESPSETTINGSBOX_SETTINGS_DEPLOY_PATH[]="/settings";
const PROGMEM char ESPSETTINGSBOX_SETTINGS_PATH[]="/settings/";
const PROGMEM char ESPSETTINGSBOX_SETTINGS_DEFAULT_FILE[]="settings.txt";
const PROGMEM char ESPSETTINGSBOX_SETTINGS_TXT_EXT[]=".txt";

const PROGMEM char ESPSETTINGSBOX_SETTINGS_REMOVED[]="Removed ";

//ThingSpeak parsing
const PROGMEM char ESPSETTINGSBOX_THINGSPEAK_PARSE_CHCREATION[]="-----------------Parse CHannel creation----------";
const PROGMEM char ESPSETTINGSBOX_THINGSPEAK_CHANNEL_JSON[]="Channel_JSON=";
const PROGMEM char ESPSETTINGSBOX_THINGSPEAK_CHANNELID[]="Channel=";
const PROGMEM char ESPSETTINGSBOX_THINGSPEAK_WRITEKEY[]="WriteKey=";
const PROGMEM char ESPSETTINGSBOX_THINGSPEAK_READKEY[]="ReadKey=";

const PROGMEM char ESPSETTINGSBOX_THINGSPEAK_UPDATE_MANAGECHANNEL[]="Update Manage channel";
const PROGMEM char ESPSETTINGSBOX_THINGSPEAK_UPDATE_CHANNEL[]="Update channel";

//----------------------------------------------------------------------------
const PROGMEM char ESPSETTINGSBOX_START_TAG[]="espSettingsBox_";
const PROGMEM char ESBOX_deviceFirmWareVersion[]="deviceFirmWareVersion";
const PROGMEM char ESBOX_DeviceId[]="DeviceId";
const PROGMEM char ESBOX_DeviceKind[]="DeviceKind";
const PROGMEM char ESBOX_DeviceDescription[]="DeviceDescription";
const PROGMEM char ESBOX_DeviceLocation[]="DeviceLocation";
const PROGMEM char ESBOX_displayAlvaysOn[]="displayAlvaysOn";
const PROGMEM char ESBOX_displayAutochange[]="displayAutochange";
const PROGMEM char ESBOX_refreshInterval[]="refreshInterval";
const PROGMEM char ESBOX_accessUser[]="accessUser";
const PROGMEM char ESBOX_accessPass[]="accessPass";
const PROGMEM char ESBOX_STARS[]="*****";
const PROGMEM char ESBOX_settingsUser[]="settingsUser";
const PROGMEM char ESBOX_settingsPass[]="settingsPass";
const PROGMEM char ESBOX_isAccesPoint[]="isAccesPoint";
const PROGMEM char ESBOX_ssidAP[]="ssidAP";
const PROGMEM char ESBOX_ssid[]="ssid";
const PROGMEM char ESBOX_password[]="password";
const PROGMEM char ESBOX_staticIp[]="staticIp";
const PROGMEM char ESBOX_localIp[]="localIp";
const PROGMEM char ESBOX_apIp[]="apIp";
const PROGMEM char ESBOX_gateIp[]="gateIp";
const PROGMEM char ESBOX_subnetIp[]="subnetIp";
const PROGMEM char ESBOX_dnsIp[]="dnsIp";
const PROGMEM char ESBOX_dnsIp2[]="dnsIp2";
const PROGMEM char ESBOX_serverIp[]="serverIp";
const PROGMEM char ESBOX_beepOnAlert[]="beepOnAlert";
const PROGMEM char ESBOX_alarmSendNotifAlertStart[]="alarmSendNotifAlertStart";
const PROGMEM char ESBOX_alarmSendNotifAlertStop[]="alarmSendNotifAlertStop";
const PROGMEM char ESBOX_alarmPlaySound[]="alarmPlaySound";
const PROGMEM char ESBOX_isThingSpeakEnabled[]="isThingSpeakEnabled";
const PROGMEM char ESBOX_postDataToTSInterval[]="postDataToTSInterval";
const PROGMEM char ESBOX_thSkUsrKey[]="thSkUsrKey";
const PROGMEM char ESBOX_thSkWKey[]="thSkWKey";
const PROGMEM char ESBOX_thSkRKey[]="thSkRKey";
const PROGMEM char ESBOX_thSkChId[]="thSkChId";
const PROGMEM char ESBOX_thSkTKey[]="thSkTKey";
const PROGMEM char ESBOX_isMqttEnabled[]="isMqttEnabled";
const PROGMEM char ESBOX_postDataToMqttInterval[]="postDataToMqttInterval";
const PROGMEM char ESBOX_mqtt_server[]="mqtt_server";
const PROGMEM char ESBOX_mqtt_user[]="mqtt_user";
const PROGMEM char ESBOX_mqtt_pass[]="mqtt_pass";
const PROGMEM char ESBOX_mqtt_port[]="mqtt_port";
const PROGMEM char ESBOX_isHttpPostEnabled[]="isHttpPostEnabled";
const PROGMEM char ESBOX_postDataToHttpInterval[]="postDataToHttpInterval";
const PROGMEM char ESBOX_httpPostIp[]="httpPostIp";
const PROGMEM char ESBOX_ntpEnabled[]="ntpEnabled";
const PROGMEM char ESBOX_NTP_poolServerName[]="NTP_poolServerName";
const PROGMEM char ESBOX_NTP_timeOffset[]="NTP_timeOffset";
const PROGMEM char ESBOX_NTP_updateInterval[]="NTP_updateInterval";
const PROGMEM char ESBOX_NTP_timeTriggerInterval[]="NTP_timeTriggerInterval";
const PROGMEM char ESBOX_telegramApiKey[]="telegramApiKey";
const PROGMEM char ESBOX_telegramReceivers[]="telegramReceivers";
const PROGMEM char ESBOX_telegramSenders[]="telegramSenders";

//const PROGMEM char ESPSETTINGSBOX_THINGSPEAK_MQ_SERVER[]="user";
//const PROGMEM char ESPSETTINGSBOX_THINGSPEAK_MQ_USERNAME[]="user";

//WiFi deploy folder
const PROGMEM char ESPSETTINGSBOX_DEFAULT_PAGE[]="/web/index.htm";
const PROGMEM char ESPSETTINGSBOX_DEFAULT_WEB_FOLDER[]="/web/";
const PROGMEM char ESPSETTINGSBOX_DEFAULT_WEB_FOLDER_ESP32[]="web/";
const PROGMEM char ESPSETTINGSBOX_DEPLOY_EXT[]="gz,js,css";

//MQTT helper messages
const PROGMEM char MESSAGE_MQTTHELPER_SEND_IS_DISABLED[]="===MQTT Helper is disabled in settings box===";
const PROGMEM char MESSAGE_MQTTHELPER_CREATE_STARTED[]="--------------------Create MQTT Helper----------------------";
const PROGMEM char MESSAGE_MQTTHELPER_BEGIN[]="--------------------Begin MQTT Helper----------------------";
const PROGMEM char MESSAGE_MQTTHELPER_INIT_STARTED[]="--------------------Init MQTT Helper----------------------";
const PROGMEM char MESSAGE_MQTTHELPER_SENDING_MESSAGE[]="----------Sending message---------";
const PROGMEM char MESSAGE_MQTTHELPER_INIT_COMPLETE[]="==Init MQTT complete===";
const PROGMEM char MESSAGE_MQTTHELPER_TOPIC_COUNT[]="topicCount=";
const PROGMEM char MESSAGE_MQTTHELPER_TOPIC_ADDED_TO_SUBSCRIBED[]="added  TOPIC=";
const PROGMEM char MESSAGE_MQTTHELPER_TOPIC_SUBSCRIBED[]="subscribed  TOPIC=";
const PROGMEM char MESSAGE_MQTTHELPER_TOPIC_SUBSCRIBED_RES_EQ[]=" subsc result=";

const PROGMEM char MESSAGE_MQTTHELPER_ERR_NOT_INITIALIZED[]="Mqtt not initialized";
const PROGMEM char MESSAGE_MQTTHELPER_ERR_NOT_CONNECTED[]="Mqtt not connected";
const PROGMEM char MESSAGE_MQTTHELPER_ERR_NOT_ENABLED[]="Mqtt not enabled";

const PROGMEM char MESSAGE_MQTTHELPER_SENT[]="Mqtt sent";
const PROGMEM char MESSAGE_MQTTHELPER_SEND_FAILED[]="Mqtt send FAILED";

const PROGMEM char MESSAGE_MQTTHELPER_NAME[]="MqttHelper";
const PROGMEM char MESSAGE_MQTTHELPER_MQTT_USER_EQ[]="mqtt_user=";
const PROGMEM char MESSAGE_MQTTHELPER_MQTT_PASS_EQ[]="mqtt_pass=";
const PROGMEM char MESSAGE_MQTTHELPER_MQTT_CONNECTED_EQ[]="connected=";

const PROGMEM char MESSAGE_MQTTHELPER_MQTT_START_CONNECT_EQ[]="==============Start connect mqttClient================";
const PROGMEM char MESSAGE_MQTTHELPER_MQTT_TEST_PUBLISH_OK[]="Test publish OK ";
const PROGMEM char MESSAGE_MQTTHELPER_MQTT_TEST_PUBLISH_FAILED[]="Test publish FAILED ";

const PROGMEM char MESSAGE_MQTTHELPER_MQTT_CONNECT_FAILED[]="Connect to MQTT FAILED ";
const PROGMEM char MESSAGE_MQTTHELPER_MQTT_WIFI_CONNECT_FAILED[]="MQTT failed No WiFi connection ";

const PROGMEM char MESSAGE_MQTTHELPER_PROCESS_ABSTRACT_ITEM_MESSAGE_RECEIVED[]="----------Abstract item Message received---------";
const PROGMEM char MESSAGE_MQTTHELPER_TOPIC_EQ[]="topic=";
const PROGMEM char MESSAGE_MQTTHELPER_MESSAGE_EQ[]="; message='";
const PROGMEM char MESSAGE_MQTTHELPER_MESSAGE_END[]="';";
const PROGMEM char MESSAGE_MQTTHELPER_RESULT_EQ[]="result=";
const PROGMEM char MESSAGE_MQTTHELPER_TARGET_QUEUE_EQ[]="target queue=";
const PROGMEM char MESSAGE_MQTTHELPER_VAL_EQ[]=" val=";

//Wifi helper messages-----------------------------
const PROGMEM char MESSAGE_WIFIHELPER_NAME[]="WiFiHelper";
const PROGMEM char MESSAGE_WIFIHELPER_INIT_CONNECTION[]="------------------Init WiFi connection------------------";
const PROGMEM char MESSAGE_WIFIHELPER_WIFI_DISCONNECTING[]="WiFi disconnecting";
const PROGMEM char MESSAGE_WIFIHELPER_SETUP_SERVER[]="SetupServer";
const PROGMEM char MESSAGE_WIFIHELPER_WIFI_MODE_EQ[]="WiFi.getMode=";
const PROGMEM char MESSAGE_WIFIHELPER_GET_AUTO_CONNECT[]=" WiFi.getAutoConnect=";
const PROGMEM char MESSAGE_WIFIHELPER_WIFI_STATUS_EQ[]=" WiFi.status=";
const PROGMEM char MESSAGE_WIFIHELPER_POST_INIT_WEB_SERV_HANDLERS[]="--------------------PostInit Web server handlers---------";
const PROGMEM char MESSAGE_WIFIHELPER_SERVER_SETUP_COMPLETED[]="Server setup completed";
const PROGMEM char MESSAGE_WIFIHELPER_HTTP_SERVER_STARTED[]="HTTP server started";

const PROGMEM char MESSAGE_WIFIHELPER_DEPLOYING_PATH[]="Deploying Path=";
const PROGMEM char MESSAGE_WIFIHELPER_AS_WEB_FILES[]=" as web files";
const PROGMEM char MESSAGE_WIFIHELPER_EXTENSIONS[]="Extensions ";
const PROGMEM char MESSAGE_WIFIHELPER_TO_BE_DEPLOYED[]=" to be deployed ";
const PROGMEM char MESSAGE_WIFIHELPER_BASE_URL_EQ[]="Base URL ";
const PROGMEM char MESSAGE_WIFIHELPER_ADDED_FILE[]="Added file ";
const PROGMEM char MESSAGE_WIFIHELPER_SIZE_EQ[]=" size=";
const PROGMEM char MESSAGE_WIFIHELPER_EXT_EQ[]=" ext=";
const PROGMEM char MESSAGE_WIFIHELPER_URL_EQ[]=" URL=";
const PROGMEM char MESSAGE_WIFIHELPER_RESPONSE_EQ[]=" response=";
const PROGMEM char MESSAGE_WIFIHELPER_HTTP_STATUS_EQ[]=" http status=";
const PROGMEM char MESSAGE_WIFIHELPER_HTTP_BODY_EQ[]=" body=";
const PROGMEM char MESSAGE_WIFIHELPER_HTTP_BEGIN_EQ[]=" begin=";
const PROGMEM char MESSAGE_WIFIHELPER_HTTP_HEADER_EQ[]=" header=";

const PROGMEM char MESSAGE_WIFIHELPER_DEPLOY_FILEMANAGER_EDIT[]="Deploying Filemanager /edit";

const PROGMEM char MESSAGE_WIFIHELPER_GET[]=" GET ";
const PROGMEM char MESSAGE_WIFIHELPER_POST[]=" POST ";

const PROGMEM char MESSAGE_WIFIHELPER_WIFI_DIAGNOSTIC[]="-----------wiFi diagnostic-------------------";

const PROGMEM char MESSAGE_WIFIHELPER_WIFI_STATUS_UNKNOWN[]="UNKNOWN";
const PROGMEM char MESSAGE_WIFIHELPER_WIFI_STATUS_WL_CONNECTED[]="WL_CONNECTED";
const PROGMEM char MESSAGE_WIFIHELPER_WIFI_STATUS_WL_NO_SSID_AVAIL[]="WL_NO_SSID_AVAIL";
const PROGMEM char MESSAGE_WIFIHELPER_WIFI_STATUS_WL_CONNECT_FAILED[]="WL_CONNECT_FAILED";
const PROGMEM char MESSAGE_WIFIHELPER_WIFI_STATUS_WL_IDLE_STATUS[]="WL_IDLE_STATUS";
const PROGMEM char MESSAGE_WIFIHELPER_WIFI_STATUS_WL_SCAN_COMPLETED[]="WL_SCAN_COMPLETED";
const PROGMEM char MESSAGE_WIFIHELPER_WIFI_STATUS_WL_DISCONNECTED[]="WL_DISCONNECTED";

const PROGMEM char MESSAGE_WIFIHELPER_WIFI_SSSID_EQ[]="SSID: ";
const PROGMEM char MESSAGE_WIFIHELPER_WIFI_IP_TYPE_EQ[]="IP Type: ";
const PROGMEM char MESSAGE_WIFIHELPER_WIFI_STATIC_IP_TYPE[]=" STATIC IP ";
const PROGMEM char MESSAGE_WIFIHELPER_WIFI_DNS_IP_TYPE[]=" DNS IP ";
const PROGMEM char MESSAGE_WIFIHELPER_WIFI_IP[]=" IP ";
const PROGMEM char MESSAGE_WIFIHELPER_WIFI_MAC[]=" MAC ";

const PROGMEM char MESSAGE_WIFIHELPER_START_AP[]="Start AP";
const PROGMEM char MESSAGE_WIFIHELPER_ACCESS_POINT[]="       AccessPoint";
const PROGMEM char MESSAGE_WIFIHELPER_STARTING_ACCESS_POINT[]="STarting as access point";
const PROGMEM char MESSAGE_WIFIHELPER_SOFT_AP[]="SoftAP ";
const PROGMEM char MESSAGE_WIFIHELPER_CONNECT_TO[]="connect to";

const PROGMEM char MESSAGE_WIFIHELPER_ESP_SETTINGS_BOX_SSID[]="espSettingsBox.ssid=";
const PROGMEM char MESSAGE_WIFIHELPER_ESP_SETTINGS_BOX_PASSWORD[]="espSettingsBox.password=";
const PROGMEM char MESSAGE_WIFIHELPER_HTTP_STATUS_TEXT_NOT_FOUND[]="Not Found";
const PROGMEM char MESSAGE_WIFIHELPER_HTTP_STATUS_FILE_NOT_FOUND[]="File Not Found";
const PROGMEM char MESSAGE_WIFIHELPER_HTTP_STATUS_TEXT_I_M_HERE[]="I'm here";
const PROGMEM char MESSAGE_WIFIHELPER_EDIT_HTML_PAGE[]="/web/edit.htm";

//
const PROGMEM char MESSAGE_I2CHELPER_BEGIN_INIT[]="-----Begin initialize of I2CHelper-----";
const PROGMEM char MESSAGE_I2CHELPER_SETUP_I2C_BUS[]="Setup i2c bus";
const PROGMEM char MESSAGE_I2CHELPER_SCANNING[]="Scanning I2C...";
const PROGMEM char MESSAGE_I2CHELPER_DEVICE_FOUND[]="I2C device found at address 0x";
const PROGMEM char MESSAGE_I2CHELPER_UNKNOWN_ERROR[]="Unknow error at address 0x";
const PROGMEM char MESSAGE_I2CHELPER_NO_DEVICES_FOUND[]="No I2C devices found\n";

//
const PROGMEM char MESSAGE_PCF8574HELPER_BEGIN_INIT[]="-----Begin initialize of PCF8574_CHelper-----";

//Thingspeak constants
const PROGMEM char MESSAGE_THINGSPEAK_SEND_STARTED[]="----------------Send data to thing speak---------------";
const PROGMEM char MESSAGE_THINGSPEAK_CHANNEL_CREATE_STARTED[]="----------------Create channel on thing speak---------------";
const PROGMEM char MESSAGE_THINGSPEAK_BASE_URL[]="http://api.thingspeak.com/update?api_key=";
const PROGMEM char MESSAGE_THINGSPEAK_CREATE_CHANNEL_URL[]="http://api.thingspeak.com/channels.json";

const PROGMEM char MESSAGE_THINGSPEAK_CURRENT_CHANNEL_URL[]="https://thingspeak.com/channels/";
const PROGMEM char MESSAGE_THINGSPEAK_PRIVATE_SHOW[]="/private_show";

const PROGMEM char MESSAGE_THINGSPEAK_API_KEY_EQ[]="api_key=";
const PROGMEM char MESSAGE_THINGSPEAK_FIELD_FOR_REQUEST[]="&field";
const PROGMEM char MESSAGE_THINGSPEAK_FIELD_FOR_REQUEST_EQ[]="&field=";
const PROGMEM char MESSAGE_THINGSPEAK_NAME_FOR_REQUEST[]="&name";
const PROGMEM char MESSAGE_THINGSPEAK_NAME_FOR_REQUEST_EQ[]="&name=";
const PROGMEM char MESSAGE_THINGSPEAK_DESCRIPTION_FOR_REQUEST[]="&description";
const PROGMEM char MESSAGE_THINGSPEAK_DESCRIPTION_FOR_REQUEST_EQ[]="&description=";

const PROGMEM char MESSAGE_THINGSPEAK_GET_CHANNELS[]=" каналов записи данных;";
const PROGMEM char MESSAGE_THINGSPEAK_SET_CHANNELS[]=" каналов управления;";

const PROGMEM char MESSAGE_THINGSPEAK_CHANNELS_PREF[]="channels/";
const PROGMEM char MESSAGE_THINGSPEAK_SUBSCRIBE_FIELDS_FIELD[]="/subscribe/fields/field";

const PROGMEM char MESSAGE_THINGSPEAK_FAILED_STATUS[]="Failed";
const PROGMEM char MESSAGE_THINGSPEAK_OK_STATUS[]="Ok";
const PROGMEM char MESSAGE_THINGSPEAK_PUBLISH_NOT_ALLOWED[]="Публикация ThingSpeak не разрешена";
const PROGMEM char MESSAGE_THINGSPEAK_NO_USER_SPECIFIED[]="Не задан пользователь ThingSpeak";
const PROGMEM char MESSAGE_THINGSPEAK_EMPTY_KEY[]="EmptyKey";

//Ntp clock client
const PROGMEM char MESSAGE_TIME_CLIENT_JSON_BEFORE_TIME[]="{\"wsId\":\"clock\",\"wsClass\":\"wsItem\",\"wsValue\":\"";
const PROGMEM char MESSAGE_TIME_CLIENT_JSON_AFTER_TIME[]="\"}";

//device widgets snapshot TODO: add this
const PROGMEM char DEVICE_WIDGETS_SNAPSHOT[]="----------Device helper----------";

//commands constants
const PROGMEM char MESSAGE_COMMANDS_EXECUTE_COMMAND[]="----------Execute command----------";
const PROGMEM char MESSAGE_COMMANDS_ERROR[]="Error";
const PROGMEM char MESSAGE_COMMANDS_OK[]="Ok";
const PROGMEM char MESSAGE_COMMANDS_COMMAND_NOT_RECOGNIZED[]="Команда не распознана";
const PROGMEM char MESSAGE_COMMANDS_DEVICE_WILL_BE_RESTARTED[]="Device will be restarted";
const PROGMEM char MESSAGE_COMMANDS_DEVICE_WILL_BE_RESTARTED_MSG[]="Устройство будет перезапущено. Дождитесь перезагрузки страницы";

const PROGMEM char MESSAGE_SERVER_ARG_CONFIRM_COMMAND[]="confirm_command";
const PROGMEM char MESSAGE_SERVER_ARG_PAGE[]="page";
const PROGMEM char MESSAGE_SERVER_ARG_SENSOR[]="sensor";
const PROGMEM char MESSAGE_SERVER_ARG_VAL[]="val";
const PROGMEM char MESSAGE_SERVER_ARG_CURRENT_SENSOR_NAME[]="currentSensor_name";

const PROGMEM char MESSAGE_SERVER_ARG_VAL_ALL[]="all";
const PROGMEM char MESSAGE_SERVER_ARG_VAL_restart[]="restart";
const PROGMEM char MESSAGE_SERVER_ARG_VAL_recreateThingSpeak[]="recreateThingSpeak";
const PROGMEM char MESSAGE_SERVER_ARG_VAL_deleteSettings[]="deleteSettings";
const PROGMEM char MESSAGE_SERVER_ARG_VAL_RESTART[]="restart";

const PROGMEM char MESSAGE_COMMANDS_FILES_DELETED[]="Удалено файлов :";

//abstract item message
const PROGMEM char MESSAGE_ABSTRACT_ITEM_NAME_EQ[]="NAME=";
const PROGMEM char MESSAGE_ABSTRACT_ITEM_INDEX_EQ[]="index=";
const PROGMEM char MESSAGE_ABSTRACT_ITEM_VAL_EQ[]=" val=";
const PROGMEM char MESSAGE_ABSTRACT_ITEM_METHOD_NOT_IMPLEMENTED[]=" Method not implemented";

const PROGMEM char MESSAGE_ABSTRACT_ITEM_CURRENT_VAL_EQ[]=" поточное значение =";
const PROGMEM char MESSAGE_ABSTRACT_ITEM_MAX_SPECIFIED_VAL_EQ[]=" максимально заданое =";
const PROGMEM char MESSAGE_ABSTRACT_ITEM_MIN_SPECIFIED_VAL_EQ[]=" минимально заданое =";

const PROGMEM char MESSAGE_ABSTRACT_ITEM_SET_SENSOR_VAL_SETTING_BEGIN[]="------------Saving sensors settings------------";
const PROGMEM char MESSAGE_ABSTRACT_ITEM_SET_SENSOR_VAL_NAME_EQ[]=" Set sensor value NAME=";
const PROGMEM char MESSAGE_ABSTRACT_ITEM_SET_SENSOR_VAL_STATUS_EQ[]=" status=";

const PROGMEM char MESSAGE_ABSTRACT_ITEM_NOT_FOUND[]="NotFound";
const PROGMEM char MESSAGE_ABSTRACT_ITEM_OK[]="Ok";

//Virtual pin
const PROGMEM char MESSAGE_VIRTUAL_PIN_START_INIT[]="init VirtualPin =";
const PROGMEM char MESSAGE_VIRTUAL_PIN_Mode[]=" In(0)Out(1)=";
const PROGMEM char MESSAGE_VIRTUAL_PIN_ID[]=" vPinId=";

//--------------------------------------------------------

//server urls
const PROGMEM char URL_EDIT[]="/edit";

const PROGMEM char URL_SUBMIT_FORM_COMMANDS[]="/submitForm_commands";
const PROGMEM char URL_SUBMIT_FORM_SETTINGS[]="/submitForm_settings";
const PROGMEM char URL_GET_JSON_SETTINGS[]="/getJson_settings";
const PROGMEM char URL_GET_JSON_SENSORS[]="/getJson_sensors";
const PROGMEM char URL_SUBMIT_FORM_SENSORS[]="/submitForm_sensors";

const PROGMEM char URL_GET_SENSORS_CURRNT_VALUES[]="/gscv";
const PROGMEM char URL_SET_SENSOR_CURRENT_VALUE[]="/sscv";

const PROGMEM char URL_SET_DIGITAL_PIN_CURRENT_VALUE[]="/sdpcv";
/*
const PROGMEM char URL_GET_BUTTON_LEFT[]="/buttonLeft/getJson";
const PROGMEM char URL_GET_BUTTON_RIGHT[]="/buttonRight/getJson";

const PROGMEM char URL_GET_LAMP_LEFT[]="/lampLeft/getJson";
const PROGMEM char URL_GET_LAMP_RIGHT[]="/lampRight/getJson";
const PROGMEM char URL_GET_ACMETER[]="/acMeter/getJson";
const PROGMEM char URL_GET_BME_MEASURER[]="/bmeMeasurer/getJson";
const PROGMEM char URL_GET_LUX_MEASURER[]="/luxMeasurer/getJson";
const PROGMEM char URL_GET_DS18D20_MEASURER[]="/ds18d20Measurer/getJson";

const PROGMEM char URL_SET_LAMP_LEFT[]="/lampLeft/setValue";
const PROGMEM char URL_SET_LAMP_RIGHT[]="/lampRight/setValue";
*/
//sensorsNames
const PROGMEM char SENSOR_buttonMenu[]="buttonMenu";
const PROGMEM char SENSOR_buttonLeft[]="buttonLeft";
const PROGMEM char SENSOR_buttonRight[]="buttonRight";
const PROGMEM char SENSOR_lampLeft[]="lampLeft";
const PROGMEM char SENSOR_lampRight[]="lampRight";
const PROGMEM char SENSOR_lamp2[]="lamp2";
const PROGMEM char SENSOR_lamp3[]="lamp3";
const PROGMEM char SENSOR_acMeter[]="acMeter";
const PROGMEM char SENSOR_bmeMeasurer[]="bmeMeasurer";
const PROGMEM char SENSOR_luxMeasurer[]="luxMeasurer";
const PROGMEM char SENSOR_ds18d20Measurer[]="ds18d20Measurer";



#endif /* LIBRARIES_MEASURER_MEASURERCONSTS_H_ */
