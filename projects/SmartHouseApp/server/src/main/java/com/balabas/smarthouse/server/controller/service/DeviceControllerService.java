package com.balabas.smarthouse.server.controller.service;

import com.balabas.smarthouse.server.model.request.DeviceRequest;
import com.balabas.smarthouse.server.model.request.DeviceRequestResult;

public interface DeviceControllerService {

	DeviceRequestResult<String> processIsServerOnlineRequest(DeviceRequest request);

	DeviceRequestResult<String> processDeviceRegistrationRequest(DeviceRequest request);

	DeviceRequestResult<String> processDataChangedOnDeviceRequest(DeviceRequest request);

}
