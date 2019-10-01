package com.balabas.smarthouse.server;

public class DeviceConstants {

	public static final String HTTP_PREFFIX  = "http://";
	public static final String DEVICE_URL_ROOT = "/";
	public static final String DEVICE_URL_DATA = "/data/";
	//Ddevice info params
	public static final String DEVICE_FIELD_DEVICE = "device";
	public static final String DEVICE_FIELD_DEVICE_INFO = "info";
	
	public static final String DEVICE_FIELD_GROUP = "group";
	public static final String DEVICE_FIELD_ENTITY_NAME = "name";
	
	//Standard group names
	public static final String GROUP_DEFAULT = "default";
	public static final String GROUP_SETTINGS = "settings";
	public static final String GROUP_SENSORS = "sensors";
	public static final String GROUP_SERVICES = "services";
	public static final String GROUP_DEVICE = "device";
	
	//Entity reserved fields keys
	public static final String ENTITY_FIELD_DESCRIPTION = "d";
	public static final String ENTITY_FIELD_SENSOR_ITEMS = "si";
	public static final String ENTITY_FIELD_ITEM_CLASS = "ir";
	
	//Entity standard key names
	public static final String ENTITY_FIELD_ON = "on";
	
	//Group device reserved field names
	public static final String ENTITY_DEVICE_DEVICE_ID = "deviceId";
	public static final String ENTITY_DEVICE_DEVICE_FIRMWARE = "deviceFirmware";
	public static final String ENTITY_DEVICE_DEVICE_DESCRIPTION = "deviceDescr";
	
	//URL properties
	public static final String DEVICE_ID  = "deviceId";
	public static final String DEVICE_ID_PARAM  = "{deviceId}";
	public static final String DEVICES_GET_URL  = "/devices";
	public static final String DEVICES_GET_DEVICE_URL  = "/devices{deviceId}";
	public static final String DEVICES_GET_DATA_DEVICE_URL  = "/devices/getData_{deviceId}";
	
	
	
	private DeviceConstants(){}
}
