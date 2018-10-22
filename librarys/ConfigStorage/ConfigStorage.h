/*
 * ConfigStorage.h
 *
 *  Created on: Oct 22, 2018
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_CONFIGSTORAGE_CONFIGSTORAGE_H_
#define LIBRARIES_CONFIGSTORAGE_CONFIGSTORAGE_H_

#include <Initializable.h>
#include <Arduino.h>
#include <ArduinoJson.h>


const PROGMEM char DEFAULT_FILE_NAME[]          = "ConfigStorage.txt";
const PROGMEM char MESSAGE_PROPERTY_NOT_FOUND[] = "Not found";
const PROGMEM char MESSAGE_PROPERTY_SECURED[]   = "Secured property";
//
const PROGMEM boolean PROPERTY_SECURED=1;
const PROGMEM boolean PROPERTY_NOT_SECURED=0;
//
const PROGMEM boolean PROPERTY_EDITABLE=1;
const PROGMEM boolean PROPERTY_NOT_EDITABLE=0;
//--------------allowed properties----------------------
//device props
const PROGMEM char DEVICE_ID[]            = "DevId";                 const PROGMEM char DEVICE_ID_DEF[]            = "DefDevice";            const PROGMEM uint16_t DEVICE_ID_INDX                   =0; const PROGMEM boolean DEVICE_ID_SEC=0;             const PROGMEM boolean DEVICE_ID_ED=0;
const PROGMEM char DEVICE_KIND[]          = "DevKind";               const PROGMEM char DEVICE_KIND_DEF[]          = "Sensors";              const PROGMEM uint16_t DEVICE_KIND_INDX                 =1; const PROGMEM boolean DEVICE_KIND_SEC=0;           const PROGMEM boolean DEVICE_KIND_ED=0;
const PROGMEM char DEVICE_DESCRIPTION[]   = "DevDescr";              const PROGMEM char DEVICE_DESCRIPTION_DEF[]   = "Dev Descr";            const PROGMEM uint16_t DEVICE_DESCRIPTION_INDX          =2; const PROGMEM boolean DEVICE_DESCRIPTION_SEC=0;    const PROGMEM boolean DEVICE_DESCRIPTION_ED=1;
const PROGMEM char DEVICE_LOCATION[]      = "DevLoc";                const PROGMEM char DEVICE_LOCATION_DEF[]      = "Dev Loc";              const PROGMEM uint16_t DEVICE_LOCATION_INDX             =3; const PROGMEM boolean DEVICE_LOCATION_SEC=0;       const PROGMEM boolean DEVICE_LOCATION_ED=1;
const PROGMEM char DEVICE_VERSION[]       = "DevVer";                const PROGMEM char DEVICE_VERSION_DEF[]       = "1.0.1";                const PROGMEM uint16_t DEVICE_VERSION_INDX              =4; const PROGMEM boolean DEVICE_VERSION_SEC=0;        const PROGMEM boolean DEVICE_VERSION_ED=0;
const PROGMEM char DEVICE_SOUND_ENABLED[] = "DevSoundEn";            const PROGMEM char DEVICE_SOUND_ENABLED_DEF[] = "DevSoundEn";           const PROGMEM uint16_t DEVICE_SOUND_ENABLED_INDX        =5; const PROGMEM boolean DEVICE_SOUND_ENABLED_SEC=0;  const PROGMEM boolean DEVICE_SOUND_ENABLED_ED=1;
//display props
const PROGMEM char DISPLAY_MODEL[]              = "DispModel";       const PROGMEM char DISPLAY_MODEL_DEF[]              = "DispModel";       const PROGMEM uint16_t DISPLAY_MODEL_INDX               =6;  const PROGMEM boolean DISPLAY_MODEL_SEC=0;                const PROGMEM boolean DISPLAY_MODEL_ED=0;
const PROGMEM char DISPLAY_AUTOCHANGE_INTERVAL[]= "DispAutoChgInt";  const PROGMEM char DISPLAY_AUTOCHANGE_INTERVAL_DEF[]= "DispAutoChgInt";  const PROGMEM uint16_t DISPLAY_AUTOCHANGE_INTERVAL_INDX =7;  const PROGMEM boolean DISPLAY_AUTOCHANGE_INTERVAL_SEC=0;  const PROGMEM boolean DISPLAY_AUTOCHANGE_INTERVAL_ED=1;
const PROGMEM char DISPLAY_ALWAYS_ON[]          = "DispAlwOn";       const PROGMEM char DISPLAY_ALWAYS_ON_DEF[]          = "DispAlwOn";       const PROGMEM uint16_t DISPLAY_ALWAYS_ON_INDX           =8;  const PROGMEM boolean DISPLAY_ALWAYS_ON_SEC=0;            const PROGMEM boolean DISPLAY_ALWAYS_ON_ED=1;
const PROGMEM char DISPLAY_AUTO_OFF_TIME[]      = "DispAutoOffTime"; const PROGMEM char DISPLAY_AUTO_OFF_TIME_DEF[]      = "DispAutoOffTime"; const PROGMEM uint16_t DISPLAY_AUTO_OFF_TIME_INDX       =9;  const PROGMEM boolean DISPLAY_AUTO_OFF_TIME_SEC=0;        const PROGMEM boolean DISPLAY_AUTO_OFF_TIME_ED=1;
//security props
//---settings page user
const PROGMEM char SECURITY_ADMIN_USER[]        = "SecAdminUsr";     const PROGMEM char SECURITY_ADMIN_USER_DEF[]        = "admin";          const PROGMEM uint16_t SECURITY_ADMIN_USER_INDX =10;    const PROGMEM boolean SECURITY_ADMIN_USER_SEC=0;  const PROGMEM boolean SECURITY_ADMIN_USER_ED=1;
const PROGMEM char SECURITY_ADMIN_PASS[]        = "SecAdminPass";    const PROGMEM char SECURITY_ADMIN_PASS_DEF[]        = "admin";          const PROGMEM uint16_t SECURITY_ADMIN_PASS_INDX =11;    const PROGMEM boolean SECURITY_ADMIN_PASS_SEC=1;  const PROGMEM boolean SECURITY_ADMIN_PASS_ED=1;

const PROGMEM char SECURITY_ADMIN_PAGE[]        = "SecAdminPages";   const PROGMEM char SECURITY_ADMIN_PAGE_DEF[]        = "settings.htm";   const PROGMEM uint16_t SECURITY_ADMIN_PAGE_INDX =12;    const PROGMEM boolean SECURITY_ADMIN_PAGE_SEC=0;  const PROGMEM boolean SECURITY_ADMIN_PAGE_ED=1;
//---index page user
const PROGMEM char SECURITY_INDEX_USER[]        = "SecIndexUsr";     const PROGMEM char SECURITY_INDEX_USER_DEF[]        = "user";    const PROGMEM uint16_t SECURITY_INDEX_USER_INDX =13;    const PROGMEM boolean SECURITY_INDEX_USER_SEC=0;  const PROGMEM boolean SECURITY_INDEX_USER_ED=1;
const PROGMEM char SECURITY_INDEX_PASS[]        = "SecIndexPass";    const PROGMEM char SECURITY_INDEX_PASS_DEF[]        = "user";    const PROGMEM uint16_t SECURITY_INDEX_PASS_INDX =14;    const PROGMEM boolean SECURITY_INDEX_PASS_SEC=1;  const PROGMEM boolean SECURITY_INDEX_PASS_ED=1;

const PROGMEM char SECURITY_INDEX_PAGE[]        = "SecIndexPages";   const PROGMEM char SECURITY_INDEX_PAGE_DEF[]        = "";        const PROGMEM uint16_t SECURITY_INDEX_PAGE_INDX =15;    const PROGMEM boolean SECURITY_INDEX_PAGE_SEC=0;  const PROGMEM boolean SECURITY_INDEX_PAGE_ED=1;
//---edit files page user
const PROGMEM char SECURITY_EDIT_USER[]         = "SecEditUsr";      const PROGMEM char SECURITY_EDIT_USER_DEF[]         = "SecEditUsr";      const PROGMEM uint16_t SECURITY_EDIT_USER_INDX  =16;    const PROGMEM boolean SECURITY_EDIT_USER_SEC=0;   const PROGMEM boolean SECURITY_EDIT_USER_ED=1;
const PROGMEM char SECURITY_EDIT_PASS[]         = "SecEditPass";     const PROGMEM char SECURITY_EDIT_PASS_DEF[]         = "SecEditPass";     const PROGMEM uint16_t SECURITY_EDIT_PASS_INDX  =17;    const PROGMEM boolean SECURITY_EDIT_PASS_SEC=1;   const PROGMEM boolean SECURITY_EDIT_PASS_ED=1;

const PROGMEM char SECURITY_EDIT_PAGE[]         = "SecEditPages";    const PROGMEM char SECURITY_EDIT_PAGE_DEF[]         = "";                const PROGMEM uint16_t SECURITY_EDIT_PAGE_INDX  =18;    const PROGMEM boolean SECURITY_EDIT_PAGE_SEC=0;   const PROGMEM boolean SECURITY_EDIT_PAGE_ED=1;
//----rest calls security
const PROGMEM char SECURITY_REST_USER[]         = "SecRestUsr";      const PROGMEM char SECURITY_REST_USER_DEF[]         = "SecRestUsr";      const PROGMEM uint16_t SECURITY_REST_USER_INDX  =19;    const PROGMEM boolean SECURITY_REST_USER_SEC=0;   const PROGMEM boolean SECURITY_REST_USER_ED=1;
const PROGMEM char SECURITY_REST_PASS[]         = "SecRestPass";     const PROGMEM char SECURITY_REST_PASS_DEF[]         = "SecRestPass";     const PROGMEM uint16_t SECURITY_REST_PASS_INDX  =20;    const PROGMEM boolean SECURITY_REST_PASS_SEC=1;   const PROGMEM boolean SECURITY_REST_PASS_ED=1;

const PROGMEM char SECURITY_REST_PAGE[]         = "SecRestPages";    const PROGMEM char SECURITY_REST_PAGE_DEF[]         = "";                const PROGMEM uint16_t SECURITY_REST_PAGE_INDX  =21;    const PROGMEM boolean SECURITY_REST_PAGE_SEC=0;   const PROGMEM boolean SECURITY_REST_PAGE_ED=1;

//wifi properties
const PROGMEM char WIFI_MODE[]              = "wifiAP";           const PROGMEM char WIFI_MODE_DEF[]              = "1";/*1STA,2AP*/          const PROGMEM uint16_t WIFI_MODE_INDX              =22; const PROGMEM boolean WIFI_MODE_SEC=0;               const PROGMEM boolean WIFI_MODE_ED=1;
const PROGMEM char WIFI_SSID_ACCESS_POINT[] = "wifiSSID_AP";      const PROGMEM char WIFI_SSID_ACCESS_POINT_DEF[] = "ESP_8266_DefDevice";     const PROGMEM uint16_t WIFI_SSID_ACCESS_POINT_INDX =23; const PROGMEM boolean WIFI_SSID_ACCESS_POINT_SEC=0;  const PROGMEM boolean WIFI_SSID_ACCESS_POINT_ED=1;
const PROGMEM char WIFI_SSID_STA[]          = "wifiSSID_STA";     const PROGMEM char WIFI_SSID_STA_DEF[]          = "balabasKiev5";           const PROGMEM uint16_t WIFI_SSID_STA_INDX          =24; const PROGMEM boolean WIFI_SSID_STA_SEC=0;           const PROGMEM boolean WIFI_SSID_STA_ED=1;
const PROGMEM char WIFI_PASS[]              = "wifiPass";         const PROGMEM char WIFI_PASS_DEF[]              = "wuWylKegayg2wu22";       const PROGMEM uint16_t WIFI_PASS_INDX              =25; const PROGMEM boolean WIFI_PASS_SEC=1;               const PROGMEM boolean WIFI_PASS_ED=1;

const PROGMEM char WIFI_IP_IS_STATIC[]    = "wifiIpIsStatic";     const PROGMEM char WIFI_IP_IS_STATIC_DEF[]    = "0";                   const PROGMEM uint16_t WIFI_IP_IS_STATIC_INDX      =26; const PROGMEM boolean WIFI_IP_IS_STATIC_SEC=0;       const PROGMEM boolean WIFI_IP_IS_STATIC_ED=1;
const PROGMEM char WIFI_IP_ACCESS_POINT[] = "wifiIpAp";           const PROGMEM char WIFI_IP_ACCESS_POINT_DEF[] = "192.168.4.1";         const PROGMEM uint16_t WIFI_IP_ACCESS_POINT_INDX   =27; const PROGMEM boolean WIFI_IP_ACCESS_POINT_SEC=0;    const PROGMEM boolean WIFI_IP_ACCESS_POINT_ED=1;
const PROGMEM char WIFI_IP_STA[]          = "wifiIpSta";          const PROGMEM char WIFI_IP_STA_DEF[]          = "192.168.0.100";       const PROGMEM uint16_t WIFI_IP_STA_INDX            =28; const PROGMEM boolean WIFI_IP_STA_SEC=0;             const PROGMEM boolean WIFI_IP_STA_ED=1;
const PROGMEM char WIFI_IP_GATEWAY[]      = "wifiIpGate";         const PROGMEM char WIFI_IP_GATEWAY_DEF[]      = "192.168.0.1";         const PROGMEM uint16_t WIFI_IP_GATEWAY_INDX        =29; const PROGMEM boolean WIFI_IP_GATEWAY_SEC=0;         const PROGMEM boolean WIFI_IP_GATEWAY_ED=1;
const PROGMEM char WIFI_IP_SUBNET[]       = "wifiIpSubnet";       const PROGMEM char WIFI_IP_SUBNET_DEF[]       = "255.255.255.0";       const PROGMEM uint16_t WIFI_IP_SUBNET_INDX         =30; const PROGMEM boolean WIFI_IP_SUBNET_SEC=0;          const PROGMEM boolean WIFI_IP_SUBNET_ED=1;
const PROGMEM char WIFI_IP_DNS1[]         = "wifiIpDns1";         const PROGMEM char WIFI_IP_DNS1_DEF[]         = "192.168.0.1";         const PROGMEM uint16_t WIFI_IP_DNS1_INDX           =31; const PROGMEM boolean WIFI_IP_DNS1_SEC=0;            const PROGMEM boolean WIFI_IP_DNS1_ED=1;
const PROGMEM char WIFI_IP_DNS2[]         = "wifiIpDns2";         const PROGMEM char WIFI_IP_DNS2_DEF[]         = "192.168.0.1";         const PROGMEM uint16_t WIFI_IP_DNS2_INDX           =32; const PROGMEM boolean WIFI_IP_DNS2_SEC=0;            const PROGMEM boolean WIFI_IP_DNS2_ED=1;
//web server properties
const PROGMEM char WEB_SERVER_PORT[]            = "webServPort";        const PROGMEM char WEB_SERVER_PORT_DEF[]            = "80";         const PROGMEM uint16_t WEB_SERVER_PORT_INDX            =33;  const PROGMEM boolean WEB_SERVER_PORT_SEC=0;            const PROGMEM boolean WEB_SERVER_PORT_ED=0;
const PROGMEM char WEB_SERVER_DEFAULT_PAGE[]    = "webServDefPage";     const PROGMEM char WEB_SERVER_DEFAULT_PAGE_DEF[]    = "/index.htm"; const PROGMEM uint16_t WEB_SERVER_DEFAULT_PAGE_INDX    =34;  const PROGMEM boolean WEB_SERVER_DEFAULT_PAGE_SEC=0;    const PROGMEM boolean WEB_SERVER_DEFAULT_PAGE_ED=0;
const PROGMEM char WEB_SERVER_AUTODEPLOY_ROOT[] = "webServAutoDeplRoot";const PROGMEM char WEB_SERVER_AUTODEPLOY_ROOT_DEF[] = "/web/";      const PROGMEM uint16_t WEB_SERVER_AUTODEPLOY_ROOT_INDX =35;  const PROGMEM boolean WEB_SERVER_AUTODEPLOY_ROOT_SEC=0; const PROGMEM boolean WEB_SERVER_AUTODEPLOY_ROOT_ED=0;
const PROGMEM char WEB_SERVER_AUTODEPLOY_EXT[]  = "webServAutoDeplExt"; const PROGMEM char WEB_SERVER_AUTODEPLOY_EXT_DEF[]  = "html,htm,js,css,jpeg,png,jpg"; const PROGMEM uint16_t WEB_SERVER_AUTODEPLOY_EXT_INDX  =36;  const PROGMEM boolean WEB_SERVER_AUTODEPLOY_EXT_SEC=0;  const PROGMEM boolean WEB_SERVER_AUTODEPLOY_EXT_ED=0;
//alarmSettings
const PROGMEM char ALARM_NOTIFY_ENABLED[]      = "alarmNotifyEnabled"; const PROGMEM char ALARM_NOTIFY_ENABLED_DEF[]      = "1"; const PROGMEM uint16_t ALARM_NOTIFY_ENABLED_INDX       =37; const PROGMEM boolean ALARM_NOTIFY_ENABLED_SEC=0;       const PROGMEM boolean ALARM_NOTIFY_ENABLED_ED=1;
const PROGMEM char ALARM_NOTIFY_ON_START[]     = "alarmNotifyOnStart"; const PROGMEM char ALARM_NOTIFY_ON_START_DEF[]     = "1"; const PROGMEM uint16_t ALARM_NOTIFY_ON_START_INDX      =38; const PROGMEM boolean ALARM_NOTIFY_ON_START_SEC=0;      const PROGMEM boolean ALARM_NOTIFY_ON_START_ED=1;
const PROGMEM char ALARM_NOTIFY_ON_END[]       = "alarmNotifyOnEnd";   const PROGMEM char ALARM_NOTIFY_ON_END_DEF[]       = "1"; const PROGMEM uint16_t ALARM_NOTIFY_ON_END_INDX        =39; const PROGMEM boolean ALARM_NOTIFY_ON_END_SEC=0;        const PROGMEM boolean ALARM_NOTIFY_ON_END_ED=1;
const PROGMEM char ALARM_NOTIFY_MIN_INTERVAL[] = "alarmNotifyMinInt";  const PROGMEM char ALARM_NOTIFY_MIN_INTERVAL_DEF[] = "0"; const PROGMEM uint16_t ALARM_NOTIFY_MIN_INTERVAL_INDX  =40; const PROGMEM boolean ALARM_NOTIFY_MIN_INTERVAL_SEC=0;  const PROGMEM boolean ALARM_NOTIFY_MIN_INTERVAL_ED=1;

const PROGMEM char ALARM_SOUND_ENABLED[]       = "alarmSoundEnabled";  const PROGMEM char ALARM_SOUND_ENABLED_DEF[]       = "1";  const PROGMEM uint16_t ALARM_SOUND_ENABLED_INDX   =41; const PROGMEM boolean ALARM_SOUND_ENABLED_SEC=0;       const PROGMEM boolean ALARM_SOUND_ENABLED_ED=1;
const PROGMEM char ALARM_SOUND_INTERVAL[]      = "alarmSoundInterval"; const PROGMEM char ALARM_SOUND_INTERVAL_DEF[]      = "30"; const PROGMEM uint16_t ALARM_SOUND_INTERVAL_INDX  =42; const PROGMEM boolean ALARM_SOUND_INTERVAL_SEC=0;      const PROGMEM boolean ALARM_SOUND_INTERVAL_ED=1;
//Thingspeak settings
const PROGMEM char THINGSPEAK_ENABLED[]        = "thSpEnabled";   const PROGMEM char THINGSPEAK_ENABLED_DEF[]        = "1";                 const PROGMEM uint16_t THINGSPEAK_ENABLED_INDX         =43; const PROGMEM boolean THINGSPEAK_ENABLED_SEC=0;        const PROGMEM boolean THINGSPEAK_ENABLED_ED=1;
const PROGMEM char THINGSPEAK_POST_INTERVAL[]  = "thSpPostInt";   const PROGMEM char THINGSPEAK_POST_INTERVAL_DEF[]  = "241";               const PROGMEM uint16_t THINGSPEAK_POST_INTERVAL_INDX   =44; const PROGMEM boolean THINGSPEAK_POST_INTERVAL_SEC=0;  const PROGMEM boolean THINGSPEAK_POST_INTERVAL_ED=1;
const PROGMEM char THINGSPEAK_USER_KEY[]       = "thSpUserKey";   const PROGMEM char THINGSPEAK_USER_KEY_DEF[]       = "U6KUFVOVLGBA0FD2";  const PROGMEM uint16_t THINGSPEAK_USER_KEY_INDX        =45; const PROGMEM boolean THINGSPEAK_USER_KEY_SEC=0;       const PROGMEM boolean THINGSPEAK_USER_KEY_ED=1;
const PROGMEM char THINGSPEAK_WRITE_KEY[]      = "thSpWriteKey";  const PROGMEM char THINGSPEAK_WRITE_KEY_DEF[]      = "BN4T5Z1M0ZVALWYR";  const PROGMEM uint16_t THINGSPEAK_WRITE_KEY_INDX       =46; const PROGMEM boolean THINGSPEAK_WRITE_KEY_SEC=0;      const PROGMEM boolean THINGSPEAK_WRITE_KEY_ED=1;
const PROGMEM char THINGSPEAK_READ_KEY[]       = "thSpReadKey";   const PROGMEM char THINGSPEAK_READ_KEY_DEF[]       = "SB58HIU5UR4F8WOW";  const PROGMEM uint16_t THINGSPEAK_READ_KEY_INDX        =47; const PROGMEM boolean THINGSPEAK_READ_KEY_SEC=0;       const PROGMEM boolean THINGSPEAK_READ_KEY_ED=1;
const PROGMEM char THINGSPEAK_TWEETER_KEY[]    = "thSpTKey";      const PROGMEM char THINGSPEAK_TWEETER_KEY_DEF[]    = "3L0KJ5J1YO247PFV";  const PROGMEM uint16_t THINGSPEAK_TWEETER_KEY_INDX     =48; const PROGMEM boolean THINGSPEAK_TWEETER_KEY_SEC=0;    const PROGMEM boolean THINGSPEAK_TWEETER_KEY_ED=1;
const PROGMEM char THINGSPEAK_CHANNEL_ID[]     = "thSpChannel";   const PROGMEM char THINGSPEAK_CHANNEL_ID_DEF[]     = "418229";            const PROGMEM uint16_t THINGSPEAK_CHANNEL_ID_INDX      =49; const PROGMEM boolean THINGSPEAK_CHANNEL_ID_SEC=0;     const PROGMEM boolean THINGSPEAK_CHANNEL_ID_ED=1;
//MqttSettingsString
const PROGMEM char MQTT_ENABLED[]          = "mqttEnabled";       const PROGMEM char MQTT_ENABLED_DEF[]          = "0";                 const PROGMEM uint16_t MQTT_ENABLED_INDX               =50; const PROGMEM boolean MQTT_ENABLED_SEC=0;      const PROGMEM boolean MQTT_ENABLED_ED=1;
const PROGMEM char MQTT_SERVER_URL[]       = "mqttServer";        const PROGMEM char MQTT_SERVER_URL_DEF[]       = "m23.cloudmqtt.com"; const PROGMEM uint16_t MQTT_SERVER_URL_INDX            =51; const PROGMEM boolean MQTT_SERVER_URL_SEC=0;   const PROGMEM boolean MQTT_SERVER_URL_ED=1;
const PROGMEM char MQTT_SERVER_PORT[]      = "mqttPort";          const PROGMEM char MQTT_SERVER_PORT_DEF[]      = "10186";             const PROGMEM uint16_t MQTT_SERVER_PORT_INDX           =52; const PROGMEM boolean MQTT_SERVER_PORT_SEC=0;  const PROGMEM boolean MQTT_SERVER_PORT_ED=1;
const PROGMEM char MQTT_USER[]             = "mqttUser";          const PROGMEM char MQTT_USER_DEF[]             = "tpheglmk";          const PROGMEM uint16_t MQTT_USER_INDX                  =53; const PROGMEM boolean MQTT_USER_SEC=0;         const PROGMEM boolean MQTT_USER_ED=1;
const PROGMEM char MQTT_PASS[]             = "mqttPass";          const PROGMEM char MQTT_PASS_DEF[]             = "QgYRfVzDhQ31";      const PROGMEM uint16_t MQTT_PASS_INDX                  =54; const PROGMEM boolean MQTT_PASS_SEC=1;         const PROGMEM boolean MQTT_PASS_ED=1;
const PROGMEM char MQTT_TOPIC_SEND[]       = "mqttTopic";         const PROGMEM char MQTT_TOPIC_SEND_DEF[]       = "topic/basePublish"; const PROGMEM uint16_t MQTT_TOPIC_SEND_INDX            =55; const PROGMEM boolean MQTT_TOPIC_SEND_SEC=0;   const PROGMEM boolean MQTT_TOPIC_SEND_ED=1;
//Viber settings
const PROGMEM char VIBER_ENABLED[]          = "viberEnabled";     const PROGMEM char VIBER_ENABLED_DEF[]          = "0";       const PROGMEM uint16_t VIBER_ENABLED_INDX          =56; const PROGMEM boolean VIBER_ENABLED_SEC=0;  const PROGMEM boolean VIBER_ENABLED_ED=1;
const PROGMEM char VIBER_USER[]             = "viberUser";        const PROGMEM char VIBER_USER_DEF[]             = "";        const PROGMEM uint16_t VIBER_USER_INDX             =57; const PROGMEM boolean VIBER_USER_SEC=0;     const PROGMEM boolean VIBER_USER_ED=1;
const PROGMEM char VIBER_PASS[]             = "viberPass";        const PROGMEM char VIBER_PASS_DEF[]             = "";        const PROGMEM uint16_t VIBER_PASS_INDX             =58; const PROGMEM boolean VIBER_PASS_SEC=1;     const PROGMEM boolean VIBER_PASS_ED=1;
//Sensors settings
const PROGMEM char SENSOR_MIN_INTERVAL[]    = "snrMinInt";        const PROGMEM char SENSOR_MIN_INTERVAL_DEF[]    = "60";      const PROGMEM uint16_t SENSOR_MIN_INTERVAL_INDX    =59; const PROGMEM boolean SENSOR_MIN_INTERVAL_SEC=0;  const PROGMEM boolean SENSOR_MIN_INTERVAL_ED=1;

const PROGMEM char SENSOR_DHT_MAX_TEMP[]    = "dhtMaxTemp";       const PROGMEM char SENSOR_DHT_MAX_TEMP_DEF[]    = "255";     const PROGMEM uint16_t SENSOR_DHT_MAX_TEMP_INDX    =60; const PROGMEM boolean SENSOR_DHT_MAX_TEMP_SEC=0;  const PROGMEM boolean SENSOR_DHT_MAX_TEMP_ED=1;
const PROGMEM char SENSOR_DHT_MIN_TEMP[]    = "dhtMinTemp";       const PROGMEM char SENSOR_DHT_MIN_TEMP_DEF[]    = "-120";    const PROGMEM uint16_t SENSOR_DHT_MIN_TEMP_INDX    =61; const PROGMEM boolean SENSOR_DHT_MIN_TEMP_SEC=0;  const PROGMEM boolean SENSOR_DHT_MIN_TEMP_ED=1;
const PROGMEM char SENSOR_DHT_MAX_HUM[]     = "dhtMaxHum";        const PROGMEM char SENSOR_DHT_MAX_HUM_DEF[]     = "101";     const PROGMEM uint16_t SENSOR_DHT_MAX_HUM_INDX     =62; const PROGMEM boolean SENSOR_DHT_MAX_HUM_SEC=0;   const PROGMEM boolean SENSOR_DHT_MAX_HUM_ED=1;
const PROGMEM char SENSOR_DHT_MIN_HUM[]     = "dhtMinHum";        const PROGMEM char SENSOR_DHT_MIN_HUM_DEF[]     = "0";       const PROGMEM uint16_t SENSOR_DHT_MIN_HUM_INDX     =63; const PROGMEM boolean SENSOR_DHT_MIN_HUM_SEC=0;   const PROGMEM boolean SENSOR_DHT_MIN_HUM_ED=1;

const PROGMEM uint16_t ITEMS_COUNT=64;

const char * const configNames[] PROGMEM =
{
		DEVICE_ID, DEVICE_KIND,	DEVICE_DESCRIPTION,	DEVICE_LOCATION, DEVICE_VERSION, DEVICE_SOUND_ENABLED,
		DISPLAY_MODEL, DISPLAY_AUTOCHANGE_INTERVAL,	DISPLAY_ALWAYS_ON, DISPLAY_AUTO_OFF_TIME,
		SECURITY_ADMIN_USER, SECURITY_ADMIN_PASS, SECURITY_ADMIN_PAGE,
		SECURITY_INDEX_USER, SECURITY_INDEX_PASS, SECURITY_INDEX_PAGE,
		SECURITY_EDIT_USER,	SECURITY_EDIT_PASS,	SECURITY_EDIT_PAGE,
		SECURITY_REST_USER,	SECURITY_REST_PASS,	SECURITY_REST_PAGE,
		WIFI_MODE, WIFI_SSID_ACCESS_POINT, WIFI_SSID_STA, WIFI_PASS, WIFI_IP_IS_STATIC,
		WIFI_IP_ACCESS_POINT, WIFI_IP_STA, WIFI_IP_GATEWAY,	WIFI_IP_SUBNET,	WIFI_IP_DNS1, WIFI_IP_DNS2,
		WEB_SERVER_PORT, WEB_SERVER_DEFAULT_PAGE, WEB_SERVER_AUTODEPLOY_ROOT, WEB_SERVER_AUTODEPLOY_EXT,
		ALARM_NOTIFY_ENABLED, ALARM_NOTIFY_ON_START, ALARM_NOTIFY_ON_END, ALARM_NOTIFY_MIN_INTERVAL,
		ALARM_SOUND_ENABLED, ALARM_SOUND_INTERVAL,
		THINGSPEAK_ENABLED, THINGSPEAK_POST_INTERVAL, THINGSPEAK_USER_KEY, THINGSPEAK_WRITE_KEY, THINGSPEAK_READ_KEY, THINGSPEAK_TWEETER_KEY, THINGSPEAK_CHANNEL_ID,
		MQTT_ENABLED, MQTT_SERVER_URL, MQTT_SERVER_PORT, MQTT_USER, MQTT_PASS, MQTT_TOPIC_SEND,
		VIBER_ENABLED, VIBER_USER, VIBER_PASS,
		SENSOR_MIN_INTERVAL, SENSOR_DHT_MAX_TEMP, SENSOR_DHT_MIN_TEMP, SENSOR_DHT_MAX_HUM, SENSOR_DHT_MIN_HUM
};

const char * const defaultValues[] PROGMEM =
{
		DEVICE_ID_DEF, DEVICE_KIND_DEF,	DEVICE_DESCRIPTION_DEF,	DEVICE_LOCATION_DEF, DEVICE_VERSION_DEF, DEVICE_SOUND_ENABLED_DEF,
		DISPLAY_MODEL_DEF, DISPLAY_AUTOCHANGE_INTERVAL_DEF,	DISPLAY_ALWAYS_ON_DEF, DISPLAY_AUTO_OFF_TIME_DEF,
		SECURITY_ADMIN_USER_DEF, SECURITY_ADMIN_PASS_DEF, SECURITY_ADMIN_PAGE_DEF,
		SECURITY_INDEX_USER_DEF, SECURITY_INDEX_PASS_DEF, SECURITY_INDEX_PAGE_DEF,
		SECURITY_EDIT_USER_DEF,	SECURITY_EDIT_PASS_DEF,	SECURITY_EDIT_PAGE_DEF,
		SECURITY_REST_USER_DEF,	SECURITY_REST_PASS_DEF,	SECURITY_REST_PAGE_DEF,
		WIFI_MODE_DEF, WIFI_SSID_ACCESS_POINT_DEF, WIFI_SSID_STA_DEF, WIFI_PASS_DEF, WIFI_IP_IS_STATIC_DEF,
		WIFI_IP_ACCESS_POINT_DEF, WIFI_IP_STA_DEF, WIFI_IP_GATEWAY_DEF,	WIFI_IP_SUBNET_DEF,	WIFI_IP_DNS1_DEF, WIFI_IP_DNS2_DEF,
		WEB_SERVER_PORT_DEF, WEB_SERVER_DEFAULT_PAGE_DEF, WEB_SERVER_AUTODEPLOY_ROOT_DEF, WEB_SERVER_AUTODEPLOY_EXT_DEF,
		ALARM_NOTIFY_ENABLED_DEF, ALARM_NOTIFY_ON_START_DEF, ALARM_NOTIFY_ON_END_DEF, ALARM_NOTIFY_MIN_INTERVAL_DEF,
		ALARM_SOUND_ENABLED_DEF, ALARM_SOUND_INTERVAL_DEF,
		THINGSPEAK_ENABLED_DEF, THINGSPEAK_POST_INTERVAL_DEF, THINGSPEAK_USER_KEY_DEF, THINGSPEAK_WRITE_KEY_DEF, THINGSPEAK_READ_KEY_DEF, THINGSPEAK_TWEETER_KEY_DEF, THINGSPEAK_CHANNEL_ID_DEF,
		MQTT_ENABLED_DEF, MQTT_SERVER_URL_DEF, MQTT_SERVER_PORT_DEF, MQTT_USER_DEF, MQTT_PASS_DEF, MQTT_TOPIC_SEND_DEF,
		VIBER_ENABLED_DEF, VIBER_USER_DEF, VIBER_PASS_DEF,
		SENSOR_MIN_INTERVAL_DEF, SENSOR_DHT_MAX_TEMP_DEF, SENSOR_DHT_MIN_TEMP_DEF, SENSOR_DHT_MAX_HUM_DEF, SENSOR_DHT_MIN_HUM_DEF
};

class ConfigStorage: public Initializable {

public:
	ConfigStorage();
	ConfigStorage(const char* _filename);
	ConfigStorage(const char* _filename,boolean forceload);
	ConfigStorage(boolean forceload);

	virtual ~ConfigStorage();

	virtual boolean initialize(boolean _init) override;

	boolean load();
	boolean save();

	void print();

	char* getAsJson();
	char* getAsJson(char* key);

	int16_t getKeyIndex(char* key);

	char* get(char* key);
	int   getInt(char* key);
	float   getFloat(char* keyId);

	char* get(uint16_t keyId);
	int   getInt(uint16_t keyId);
	float   getFloat(uint16_t keyId);

	void set(char* key,char* val);
	void setInt(char* key,int val);
	void setFloat(char* key,float val);

	void set(uint16_t keyId,char* val);
	void setInt(uint16_t keyId,int val);
	void setFloat(uint16_t keyId,float val);

	void printMemory();
	void printSettingsFile();

protected:
	void fillDefaultValues();

	const char* fileName;
	char* values[ITEMS_COUNT];

private:
	boolean spiffInitialized;
	void construct(const char* _filename,boolean init,boolean initSpiff);
	//boolean putSettingsToMemory(JsonObject& root);
};

#endif /* LIBRARIES_CONFIGSTORAGE_CONFIGSTORAGE_H_ */
