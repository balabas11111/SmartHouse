package com.balabas.smarthouse.server.controller.service;

import org.json.JSONObject;

import com.balabas.smarthouse.server.model.Device;

public interface DeviceRequestorService {

	String executeGetDataOnDevice(Device device, String deviceEntityGroup);

	String executePostDataOnDevice(Device device, JSONObject json);

}
