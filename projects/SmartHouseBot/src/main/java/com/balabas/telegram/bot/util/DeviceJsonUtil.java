package com.balabas.telegram.bot.util;

import org.json.JSONException;
import org.json.JSONObject;

import static com.balabas.smarthouse.server.DeviceConstants.GROUP_DEVICE;
import static com.balabas.smarthouse.server.DeviceConstants.TAG_DEVICE_FIRMWARE;;

public class DeviceJsonUtil {

	private DeviceJsonUtil(){}
	

	public static String getDeviceFirmware(JSONObject json) throws JSONException{

		if(json.has(GROUP_DEVICE)){
			JSONObject deviceJson = json.getJSONObject(GROUP_DEVICE);
			if(deviceJson.has(TAG_DEVICE_FIRMWARE)){
				return deviceJson.getString(TAG_DEVICE_FIRMWARE);
			}
		}
		
		return null;
	}
}
