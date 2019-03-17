/*
 * Measurer.h
 *
 *  Created on: 14 ГѓпїЅГўв‚¬Е“ГѓвЂ�Гўв‚¬Е“ГѓпїЅГўв‚¬в„ўГѓвЂљГ‚В®ГѓпїЅГўв‚¬Е“ГѓвЂ�Гўв‚¬Е“ГѓпїЅГўв‚¬в„ўГѓпїЅГўв‚¬ЕѕГѓпїЅГўв‚¬Е“ГѓвЂ�Гўв‚¬Е“ГѓпїЅГўв‚¬в„ўГѓпїЅГўв‚¬В . 2018 ГѓпїЅГўв‚¬Е“ГѓвЂ�Гўв‚¬Е“ГѓпїЅГўв‚¬в„ўГѓпїЅГ‹вЂ .
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_MEASURER_MEASURERCONSTS_H_
#define LIBRARIES_MEASURER_MEASURERCONSTS_H_

#define ARRAY_SIZE(x) sizeof(x)/sizeof(x[0])
#define VAR_NAME(var) #var
#define implements( C, I ) (    __extension__ ({                   \
        static bool impl=(dynamic_cast<I*>(&C))? true : false;     \
        impl;                                                      \
      }))

//if ( implements( instance, SomeInterface ) ) ...


#include "Arduino.h"

const char TEMPERATURE[] PROGMEM ="Temperature";
const char HUMIDITY[] PROGMEM ="Humidity";
const char PRESSURE[] PROGMEM ="Pressure";
const char ALTITUDE[] PROGMEM ="Altitude";
const char LIGHT[] PROGMEM ="Light";

const char TEMPERATURE_DHT22[] PROGMEM ="Temperature_DHT22";
const char HUMIDITY_DHT22[] PROGMEM ="Humidity_DHT22";

const char MEASURE_CELSIUS_DEGREES[] PROGMEM ="В°C";
const char MEASURE_PERSENT[] PROGMEM ="%";
const char MEASURE_PASCAL[] PROGMEM ="Pa";
const char MEASURE_METER[] PROGMEM ="m";
const char MEASURE_LIGHT[] PROGMEM ="Lux";
//--------------------------------------------------------
//Application paths

//ModelData storage
#define PATH_MODEL_DATA_JSON_FILE_PATH "/data/entity/"
#define PATH_MODEL_DATA_JSON_FILE      "/data/entity/root.json"
#define PATH_MODEL_DATA_JSON_FILE_NAME "root.json"
#define PATH_MODEL_DATA_JSON_BY_GROUP  "/deploy/"
#define PATH_WEB_FILES_PATH            "/web/"
//http Root URLs
#define URL_ROOT  "/"
#define URL_INFO  "/info"
#define URL_DIR   "/dir"
#define URL_CAT   "/cat"
#define URL_FILES "/files"
//--------------------------------------------------------
//content types
#define CONTENT_TYPE                  "Content-Type"
#define CONTENT_TYPE_TEXT_HTML        "text/html"
#define CONTENT_TYPE_JSON             "application/json"
#define CONTENT_TYPE_FORM_URL_ENCODED "application/x-www-form-urlencoded"
#define CONTENT_TYPE_TEXT_PLAIN       "text/plain"

const PROGMEM char CONTENT_TYPE_APPLICATION_OCTED_STREAM[]="application/octet-stream";
const PROGMEM char CONTENT_TYPE_TEXT_CSS[]="text/css";
const PROGMEM char CONTENT_TYPE_TEXT_JSON[]="text/json";
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
const PROGMEM char CONTENT_TYPE_TEXT_JSON_UTF8[] ="text/json; charset=UTF-8";
const PROGMEM char CONTENT_TYPE_TEXT_HTML_UTF8[] ="text/html; charset=UTF-8";
const PROGMEM char CONTENT_TYPE_TEXT_XML_UTF8[]  ="text/xml; charset=UTF-8";
const PROGMEM char CONTENT_TYPE_TEXT_PLAIN_UTF8[]="text/plain; charset=UTF-8";
//Default Server argument
const PROGMEM char SERVER_ARG_name[]="name";
const PROGMEM char SERVER_ARG_dir[]="dir";
const PROGMEM char SERVER_ARG_file[]="file";
const PROGMEM char SERVER_ARG_kind[]="kind";
const PROGMEM char SERVER_ARG_list[]="list";

//headers
const PROGMEM char HEADER_Location[]="Location";
//
const PROGMEM char NEXT_LINE_RN[]="\r\n";
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
const PROGMEM char MESSAGE_DEVICE_FREE_HEAP[]="FreeHeap=";
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

const PROGMEM char MESSAGE_DEVICE_HELPER_ALARM_MODE_FINISHED_RESULT[]="Р РµР¶РёРј С‚СЂРµРІРѕРіРё РїСЂРµРєСЂР°С‰РµРЅ. Р’СЃРµ РїРѕРєР°Р·Р°С‚РµР»Рё РІ РЅРѕСЂРјРµ";

//----------EspSettingsBox Messages---------------
const PROGMEM char MESSAGE_ESPSETTINGSBOX_ABSTRACT_ITEM_SAVE_BEGIN[]="Save AbstractSensor to file=";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_SETTINGS_SAVE_BEGIN[]="Save Settings to file=";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_SAVED[]="...saved ";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_DEFAULT_VALUES_SAVED[]="DEFAULT ";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_ERROR_FILE_NOT_EXISTS[]="File not exists";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_ERROR_BOX_FILE_NOT_EXISTS[]="Box File not exists";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_BOX_FILE_EXISTS[]="Box File EXISTS";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_ERROR_FILE_EMPTY[]="File is empty";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_ERROR_PARSE_JSON[]="Error parse JSON file";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_VALUE_PARSED[]="val read=";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_FILE_LINES[]="File lines=";
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
const PROGMEM char MESSAGE_ESPSETTINGSBOX_SETTINGS_FROM_BOX_MEMORY[]="-----------------Settings from BOX memory-------";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_SAVE_DEVICE_SETTINGS_TO_FILE[]="Save device settings to file";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_SAVE_EXTRABOX_SETTINGS_TO_FILE[]="Save ExtraBox settings to file";

const PROGMEM char MESSAGE_ESPSETTINGSBOX_SETTINGS_TO_MEMORY[]="-----------------Settings loaded to memory-------";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_SETTINGS_TO_BOX_MEMORY[]="-----------------Settings loaded to BOX memory-------";

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
const PROGMEM char MESSAGE_ESPSETTINGSBOX_spiffsUsed_EQ[]=" spiffsUsed=";
const PROGMEM char MESSAGE_ESPSETTINGSBOX_spiffsTotal_EQ[]=" spiffsTotal=";

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

const PROGMEM char ESPSETTINGSBOX_FOLDER_DIVIDER[]="/";
const PROGMEM char ESPSETTINGSBOX_SETTINGS_DEPLOY_PATH[]="/settings";
const PROGMEM char ESPSETTINGSBOX_SETTINGS_PATH[]="/settings/";
const PROGMEM char ESPSETTINGSBOX_EXTRA_SETTINGS_BOX_PATH[]="/settings/extra/";
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
const PROGMEM char ESBOX_accessPassConfirm[]="accessPassConfirm";
const PROGMEM char ESBOX_STARS[]="*****";
const PROGMEM char ESBOX_settingsUser[]="settingsUser";
const PROGMEM char ESBOX_settingsPass[]="settingsPass";
const PROGMEM char ESBOX_settingsPassConfirm[]="settingsPassConfirm";
const PROGMEM char ESBOX_isAccesPoint[]="isAccesPoint";
const PROGMEM char ESBOX_ssidAP[]="ssidAP";
const PROGMEM char ESBOX_ssid[]="ssid";
const PROGMEM char ESBOX_password[]="password";
const PROGMEM char ESBOX_passwordConfirm[]="passwordConfirm";
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
const PROGMEM char ESBOX_isHttpPostEnabled[]="isHttpPostEnabled";
const PROGMEM char ESBOX_postDataToHttpInterval[]="postDataToHttpInterval";
const PROGMEM char ESBOX_httpPostIp[]="httpPostIp";
const PROGMEM char ESBOX_telegramApiKey[]="telegramApiKey";
const PROGMEM char ESBOX_telegramReceivers[]="telegramReceivers";
const PROGMEM char ESBOX_telegramSenders[]="telegramSenders";

//WiFi deploy folder
const PROGMEM char WEB_DEFAULT_PAGE[]="/web/index.htm";
const PROGMEM char WEB_DEFAULT_WEB_FOLDER[]="/web/";
const PROGMEM char WEB_DEFAULT_WEB_FOLDER_NO_END[]="/web";
const PROGMEM char WEB_DEFAULT_WEB_FOLDER_ESP32[]="web/";
const PROGMEM char WEB_DEPLOY_EXT[]="gz,js,css";

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
const PROGMEM char MESSAGE_WIFIHELPER_DEPLOY_DEFAULT_PAGES[]="Deploying default pages /edit /index /settings /list?dir= /list?file=";

const PROGMEM char MESSAGE_WIFIHELPER_GET[]=" GET ";
const PROGMEM char MESSAGE_WIFIHELPER_POST[]=" POST ";

const PROGMEM char MESSAGE_WIFIHELPER_WIFI_DIAGNOSTIC[]="-----------wiFi diagnostic-------------------";
const PROGMEM char WL_UNKNOWN_STATUS[]="WL_UNKNOWN_STATUS";

const char* const WIFIHELPER_WIFI_STATUSES[]      PROGMEM=
{
		"WL_IDLE_STATUS",
		"WL_NO_SSID_AVAIL",
		"WL_SCAN_COMPLETED",
		"WL_CONNECTED",
		"WL_CONNECT_FAILED",
		"WL_CONNECTION_LOST",
		"WL_DISCONNECTED",
		"WL_CONNECTING"
};

const char* const WIFIHELPER_WIFI_MODES[]      PROGMEM=
{
		"WIFI_OFF",
		"WIFI_STA",
		"WIFI_AP",
		"WIFI_AP_STA"
};

const char* const WIFIHELPER_PHY_MODES[]      PROGMEM={ "", "B", "G", "N" };

const char* const WIFIHELPER_SlEEP_MODES[]      PROGMEM={ "WIFI_NONE_SLEEP", "WIFI_LIGHT_SLEEP", "WIFI_MODEM_SLEEP"};


const PROGMEM char MESSAGE_WIFIHELPER_WIFI_MODE[]=" MODE: ";
const PROGMEM char MESSAGE_WIFIHELPER_WIFI_PHYMODE[]=" PHY MODE: ";
const PROGMEM char MESSAGE_WIFIHELPER_WIFI_SlEEPMODE[]=" SlEEP MODE: ";
const PROGMEM char MESSAGE_WIFIHELPER_WIFI_AUTOCONNECT[]=" autoConnect: ";
const PROGMEM char MESSAGE_WIFIHELPER_WIFI_CHANNEL[]=" channel: ";
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
#define MESSAGE_WIFIHELPER_HTTP_STATUS_FILE_NOT_FOUND "File Not Found"
const PROGMEM char MESSAGE_WIFIHELPER_HTTP_STATUS_TEXT_I_M_HERE[]="I'm here";

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

const PROGMEM char MESSAGE_THINGSPEAK_GET_CHANNELS[]=" - РєР°РЅР°Р»РѕРІ С‡С‚РµРЅРёСЏ Р±С‹Р»Рѕ СЃРѕР·РґР°РЅРѕ";
const PROGMEM char MESSAGE_THINGSPEAK_SET_CHANNELS[]=" - РєР°РЅР°Р»РѕРІ Р·Р°РїРёСЃРё Р±С‹Р»Рѕ СЃРѕР·РґР°РЅРѕ";

const PROGMEM char MESSAGE_THINGSPEAK_CHANNELS_PREF[]="channels/";
const PROGMEM char MESSAGE_THINGSPEAK_SUBSCRIBE_FIELDS_FIELD[]="/subscribe/fields/field";

const PROGMEM char MESSAGE_THINGSPEAK_FAILED_STATUS[]="Failed";
const PROGMEM char MESSAGE_THINGSPEAK_OK_STATUS[]="Ok";
const PROGMEM char MESSAGE_THINGSPEAK_EMPTY_KEY[]="EmptyKey";

//device widgets snapshot TODO: add this
const PROGMEM char DEVICE_WIDGETS_SNAPSHOT[]="----------Device helper----------";

const PROGMEM char MESSAGE_SERVER_ARG_ENTITY[]="entity";
const PROGMEM char MESSAGE_SERVER_ARG_PAGE[]="page";
const PROGMEM char MESSAGE_SERVER_ARG_NAME[]="name";
const PROGMEM char MESSAGE_SERVER_ARG_VAL[]="val";
const PROGMEM char MESSAGE_SERVER_ARG_REMOTE_TARGET[]="remote_target";
const PROGMEM char MESSAGE_SERVER_ARG_REMOTE_PAGE[]="remote_page";
const PROGMEM char MESSAGE_SERVER_ARG_VAL_JSON[]="val_json";

const PROGMEM char MESSAGE_SERVER_ARG_VAL_ALL[]="all";
const PROGMEM char MESSAGE_SERVER_ARG_VAL_restart[]="restart";
const PROGMEM char MESSAGE_SERVER_ARG_VAL_recreateThingSpeak[]="recreateThingSpeak";
const PROGMEM char MESSAGE_SERVER_ARG_VAL_deleteSettings[]="deleteSettings";
const PROGMEM char MESSAGE_SERVER_ARG_VAL_RESTART[]="restart";

//abstract item message
const PROGMEM char MESSAGE_ABSTRACT_ITEM_NAME_EQ[]="NAME=";
const PROGMEM char MESSAGE_ABSTRACT_ITEM_INDEX_EQ[]="index=";
const PROGMEM char MESSAGE_ABSTRACT_ITEM_VAL_EQ[]=" val=";
const PROGMEM char MESSAGE_ABSTRACT_ITEM_METHOD_NOT_IMPLEMENTED[]=" Method not implemented";

const PROGMEM char MESSAGE_ABSTRACT_ITEM_CURRENT_VAL_EQ[]=" ГѓпїЅГ‚В ГѓвЂ�Гўв‚¬вЂќГѓпїЅГ‚В ГѓвЂ�Гўв‚¬ВўГѓпїЅГ‚ВЎГѓВўГўвЂљВ¬Г…ВЎГѓпїЅГ‚В ГѓвЂ�Гўв‚¬ВўГѓпїЅГ‚ВЎГѓВўГўвЂљВ¬Г‚ВЎГѓпїЅГ‚В ГѓпїЅГўв‚¬В¦ГѓпїЅГ‚В ГѓвЂ�Гўв‚¬ВўГѓпїЅГ‚В ГѓвЂљГ‚Вµ ГѓпїЅГ‚В ГѓвЂљГ‚В·ГѓпїЅГ‚В ГѓпїЅГўв‚¬В¦ГѓпїЅГ‚В ГѓвЂљГ‚В°ГѓпїЅГ‚ВЎГѓВўГўвЂљВ¬Г‚ВЎГѓпїЅГ‚В ГѓвЂљГ‚ВµГѓпїЅГ‚В ГѓпїЅГўв‚¬В¦ГѓпїЅГ‚В ГѓвЂ�Гўв‚¬ЛњГѓпїЅГ‚В ГѓвЂљГ‚Вµ =";
const PROGMEM char MESSAGE_ABSTRACT_ITEM_MAX_SPECIFIED_VAL_EQ[]=" ГѓпїЅГ‚В ГѓвЂ�Г‹Е“ГѓпїЅГ‚В ГѓвЂљГ‚В°ГѓпїЅГ‚В ГѓвЂ�Гўв‚¬пїЅГѓпїЅГ‚ВЎГѓпїЅГ†вЂ™ГѓпїЅГ‚В ГѓвЂ�Гўв‚¬ЛњГѓпїЅГ‚В ГѓвЂ�Г‹Е“ГѓпїЅГ‚В ГѓвЂљГ‚В°ГѓпїЅГ‚В ГѓвЂљГ‚В»ГѓпїЅГ‚ВЎГѓпїЅГ…В ГѓпїЅГ‚В ГѓпїЅГўв‚¬В¦ГѓпїЅГ‚В ГѓвЂ�Гўв‚¬Вў ГѓпїЅГ‚В ГѓвЂљГ‚В·ГѓпїЅГ‚В ГѓвЂљГ‚В°ГѓпїЅГ‚В ГѓвЂ™Гўв‚¬ЛњГѓпїЅГ‚В ГѓвЂљГ‚В°ГѓпїЅГ‚В ГѓпїЅГўв‚¬В¦ГѓпїЅГ‚В ГѓвЂ�Гўв‚¬ВўГѓпїЅГ‚В ГѓвЂљГ‚Вµ =";
const PROGMEM char MESSAGE_ABSTRACT_ITEM_MIN_SPECIFIED_VAL_EQ[]=" ГѓпїЅГ‚В ГѓвЂ�Г‹Е“ГѓпїЅГ‚В ГѓвЂ�Гўв‚¬ЛњГѓпїЅГ‚В ГѓпїЅГўв‚¬В¦ГѓпїЅГ‚В ГѓвЂ�Гўв‚¬ЛњГѓпїЅГ‚В ГѓвЂ�Г‹Е“ГѓпїЅГ‚В ГѓвЂљГ‚В°ГѓпїЅГ‚В ГѓвЂљГ‚В»ГѓпїЅГ‚ВЎГѓпїЅГ…В ГѓпїЅГ‚В ГѓпїЅГўв‚¬В¦ГѓпїЅГ‚В ГѓвЂ�Гўв‚¬Вў ГѓпїЅГ‚В ГѓвЂљГ‚В·ГѓпїЅГ‚В ГѓвЂљГ‚В°ГѓпїЅГ‚В ГѓвЂ™Гўв‚¬ЛњГѓпїЅГ‚В ГѓвЂљГ‚В°ГѓпїЅГ‚В ГѓпїЅГўв‚¬В¦ГѓпїЅГ‚В ГѓвЂ�Гўв‚¬ВўГѓпїЅГ‚В ГѓвЂљГ‚Вµ =";

const PROGMEM char MESSAGE_ABSTRACT_ITEM_SET_SENSOR_VAL_SETTING_BEGIN[]="------------Saving sensors settings------------";
const PROGMEM char MESSAGE_ABSTRACT_ITEM_SET_SENSOR_VAL_NAME_EQ[]=" Set sensor value NAME=";
const PROGMEM char MESSAGE_ABSTRACT_ITEM_SET_SENSOR_VAL_STATUS_EQ[]=" status=";

//Virtual pin
const PROGMEM char MESSAGE_VIRTUAL_PIN_START_INIT[]="init VirtualPin =";
const PROGMEM char MESSAGE_VIRTUAL_PIN_Mode[]=" In(0)Out(1)=";
const PROGMEM char MESSAGE_VIRTUAL_PIN_ID[]=" vPinId=";

//--------------------------------------------------------

//server urls
const PROGMEM char URL_LIST[]="/list";
const PROGMEM char URL_VIEW[]="/view";
const PROGMEM char URL_TEST[]="/test";

const PROGMEM char URL_INDEX[]="/index";
const PROGMEM char URL_SETTINGS[]="/settings";
const PROGMEM char URL_EDIT[]="/edit";

const PROGMEM char URL_GET_JSON_SENSORS[]="/getJson_sensors";
const PROGMEM char URL_SUBMIT_FORM_SENSORS[]="/submitForm_sensors";

//use /getJson?name=name
const PROGMEM char URL_ENTITY[]="/entity";
const PROGMEM char URL_GET_JSON[]="/getJson";
const PROGMEM char URL_PROCESS_JSON[]="/processJson";

const PROGMEM char URL_SET_DIGITAL_PIN_CURRENT_VALUE[]="/sdpcv";

#define MESSAGE_WIFIHELPER_INDEX_HTML_PAGE "/web/index.htm"
const PROGMEM char MESSAGE_WIFIHELPER_SETTINGS_HTML_PAGE[]="/web/settings.htm";
const PROGMEM char MESSAGE_WIFIHELPER_EDIT_HTML_PAGE[]="/web/edit.htm";

//sensorsNames
const PROGMEM char SENSOR_buttonMenu[]="buttonMenu";
const PROGMEM char SENSOR_buttonLeft[]="buttonLeft";
const PROGMEM char SENSOR_buttonRight[]="buttonRight";
const PROGMEM char SENSOR_lampLeft[]="lampLeft";
const PROGMEM char SENSOR_lampRight[]="lampRight";
const PROGMEM char SENSOR_signalLed[]="signalLed";
const PROGMEM char SENSOR_lamp2[]="lamp2";
const PROGMEM char SENSOR_lamp3[]="lamp3";
const PROGMEM char SENSOR_acMeter[]="acMeter";
const PROGMEM char SENSOR_acMeter2[]="acMeter2";
const PROGMEM char SENSOR_bmeMeasurer[]="bmeMeasurer";
const PROGMEM char SENSOR_luxMeasurer[]="luxMeasurer";
const PROGMEM char SENSOR_ds18d20Measurer[]="ds18d20Measurer";
const PROGMEM char SENSOR_ds18d20Boiler[]="ds18d20Boiler";
const PROGMEM char SENSOR_AC_Rele1[]="AC_Rele1";

#endif /* LIBRARIES_MEASURER_MEASURERCONSTS_H_ */
