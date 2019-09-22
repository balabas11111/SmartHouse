package com.balabas.smarthouse.server.util;

import static com.balabas.smarthouse.server.DeviceConstants.GROUP_DEVICE;
import static com.balabas.smarthouse.server.DeviceConstants.GROUP_SENSORS;
import static com.balabas.smarthouse.server.DeviceConstants.GROUP_SETTINGS;
import static com.balabas.smarthouse.server.DeviceConstants.TAG_DEVICE_ID;
import static com.balabas.smarthouse.server.DeviceConstants.TAG_DEVICE_FIRMWARE;

import org.json.JSONException;
import org.json.JSONObject;

public class ServerValuesMockUtil {

	public static final String FIRMWARE1 = "FirmWare1";
	
	private ServerValuesMockUtil(){}
	
	public static JSONObject getSettingsSensors(String deviceId) throws JSONException{
		return new JSONObject()
		.put(GROUP_SETTINGS, getSettingsJson())
		.put(GROUP_SENSORS, getSensorsJson())
		.put(GROUP_DEVICE, getDeviceJson(deviceId));
	}
	
	public static JSONObject getSensors(String deviceId) throws JSONException{
		return new JSONObject()
				.put(GROUP_SENSORS, getSensorsJson())
				.put(GROUP_DEVICE, getDeviceJson(deviceId));
	}
	
	private static JSONObject getDeviceJson(String deviceId) throws JSONException{
		//String id = deviceId.substring(DEVICE_ID.length());
		
		return new JSONObject().put(TAG_DEVICE_ID, deviceId).put(TAG_DEVICE_FIRMWARE, FIRMWARE1);
	}
	
	private static JSONObject getSensorsJson() throws JSONException{
		return new JSONObject()
				.put("bh1750", new JSONObject().put("d", "bh1750-descr").put("l", "light_value"))
				.put("bme280", new JSONObject().put("d", "bme280-descr").put("p", "pressure_value").put("h", "humidity_value").put("t", "temperatue_value"));
	}
	
	private static JSONObject getSettingsJson() throws JSONException{
		return new JSONObject()
				.put("WiFiSetting", new JSONObject().put("set1", "set1").put("set2", "123").put("set3", "set33"));
	}
}
