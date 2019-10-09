package com.balabas.smarthouse.server.controller.service;

import java.util.Map;

import org.json.JSONObject;

import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.Entity;

public interface DeviceRequestorService {

	String executeGetDataOnDevice(Device device, String deviceEntityGroup);

	String executePostDataOnDevice(Device device, JSONObject json);
	
	String executePostDataOnDeviceEntity(Device device, Entity entity, Map<String, Object> values);

}
