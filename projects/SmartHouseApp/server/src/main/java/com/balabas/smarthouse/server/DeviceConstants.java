package com.balabas.smarthouse.server;

public class DeviceConstants {

	public static final String FIELD_SPLITTER = ":";
	public static final String TOPIC_SPLITTER  = "/";
	
	public static final String HTTP_PREFFIX  = "http://";
	public static final String DEVICE_URL_ROOT = "/";
	public static final String DEVICE_URL_DATA = "/data";
	//Ddevice info params
	public static final String DEVICE_FIELD_DEVICE = "device";
	public static final String DEVICE_FIELD_DEVICE_INFO = "info";
	
	public static final String DEVICE_FIELD_URL_ROOT = "urlRoot";
	public static final String DEVICE_FIELD_URL_DATA = "urlData";
	public static final String DEVICE_FIELD_MODEL = "model";
	
	public static final String DEVICE_FIELD_GROUP = "group";
	public static final String DEVICE_FIELD_ENTITY_NAME = "name";
	public static final String DEVICE_FIELD_DATA = "data";
	public static final String DEVICE_FIELD_KEY = "key";
	public static final String DEVICE_FIELD_TOKEN = "token";
	
	//Standard group names
	public static final String GROUP_DEFAULT = "default";
	public static final String GROUP_SETTINGS = "settings";
	public static final String GROUP_SENSORS = "sensors";
	public static final String GROUP_SERVICES = "services";
	public static final String GROUP_DEVICE = "device";
	
	//Entity reserved fields keys
	public static final String ENTITY_FIELD_STATUS = "status";
	public static final String ENTITY_FIELD_DESCRIPTION = "d";
	public static final String ENTITY_FIELD_SENSOR_ITEMS = "si";
	public static final String ENTITY_FIELD_FIELDS_ARRAY = "fa";
	public static final String ENTITY_FIELD_KEYS_ARRAY = "ka";
	public static final String ENTITY_FIELD_ITEM_CLASS = "ic";
	public static final String ENTITY_FIELD_MQ = "mq";
	
	public static final String ENTITY_FIELD_ID = "id";
	public static final String ENTITY_FIELD_IP = "ip";
	public static final String ENTITY_FIELD_ROOT = "root";
	public static final String ENTITY_FIELD_DATA = "data";
	public static final String ENTITY_FIELD_SWG = "swg";
	public static final String ENTITY_FIELD_COUNT = "count";
	public static final String ENTITY_FIELD_C = "c";
	public static final String ENTITY_FIELD_MESSAGE = "msg";
	
	public static final String MESSAGE_FIELD_SEVERITY = "msvr";
	public static final String MESSAGE_FIELD_CODE = "mcode";
	public static final String MESSAGE_FIELD_TEXT = "mtext";
	public static final String MESSAGE_CODE_DEVICE_WILL_BE_RESTARTED = "rst";
	
	public static final String MESSAGE_SEVERITY_INFO  = "INFO";
	public static final String MESSAGE_SEVERITY_WARN  = "WARN";
	public static final String MESSAGE_SEVERITY_ERROR = "ERROR";
	
	public static final String HEADER_SREQUEST = "SHr"; 
	
	public static final String ENTITY_FIELD_SWG_EQ_1 = "?swg=1";
	public static final String ENTITY_FIELD_SERVER_REQUEST_EQ_1 = ENTITY_FIELD_SWG_EQ_1 
			+ "&" + HEADER_SREQUEST +"=1";
	//Entity standard key names
	public static final String ENTITY_FIELD_ON = "on";
	
	//Group device reserved field names
	public static final String ENTITY_DEVICE_SERVER_ID = "serverId";
	public static final String ENTITY_DEVICE_DEVICE_ID = "deviceId";
	public static final String ENTITY_DEVICE_DEVICE_FIRMWARE = "deviceFirmware";
	public static final String ENTITY_DEVICE_DEVICE_DESCRIPTION = "deviceDescr";
	public static final String ENTITY_DEVICE_DEVICE_KEY = "deviceKey";
	public static final String ENTITY_DEVICE_DEVICE_IP = "deviceIp";
	public static final String ENTITY_DEVICE_DEVICE_REGISTRATION_RESULT = "regRes";
	
	//URL properties
	public static final String DEVICE_ID  = "deviceId";
	public static final String DEVICE_ID_PARAM  = "{deviceId}";
	public static final String DEVICES_GET_URL  = "/devices";
	public static final String DEVICES_GET_DEVICE_URL  = "/devices{deviceId}";
	public static final String DEVICES_GET_DATA_DEVICE_URL  = "/devices/getData_{deviceId}";
	
	
	
	private DeviceConstants(){}
}
