package com.balabas.smarthouse.server;

public class DeviceConstants {

	public static final String TAG_DEVICE = "device";
	public static final String TAG_DEVICE_ID = "deviceId";
	public static final String TAG_DEVICE_FIRMWARE = "deviceFirmware";
	
	public static final String GROUP = "group";
	public static final String GROUP_SETTINGS = "settings";
	public static final String GROUP_SENSORS = "sensors";
	public static final String GROUP_SERVICES = "services";
	public static final String GROUP_DEVICE = TAG_DEVICE;
	
	
	public static final String DEVICE_ID  = "deviceId";
	
	public static final String DEVICE_ID_PARAM  = "{deviceId}";
	
	public static final String DEVICES_GET_URL  = "/devices";
	
	public static final String DEVICES_GET_DEVICE_URL  = "/devices{deviceId}";
	
	public static final String DEVICES_GET_DATA_DEVICE_URL  = "/devices/getData_{deviceId}";
	
	private DeviceConstants(){}
}
