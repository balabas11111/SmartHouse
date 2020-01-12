package com.balabas.smarthouse.server.service;

import java.util.Map;

import org.json.JSONObject;

import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;

public interface DeviceRequestorService {

	String executePostDataOnDevice(IDevice device, JSONObject json);
	
	String executePostDataOnDeviceEntity(IDevice device, IEntity entity, Map<String, Object> values);

	String executeGetDataOnDevice(IDevice device, String groupName);

}
