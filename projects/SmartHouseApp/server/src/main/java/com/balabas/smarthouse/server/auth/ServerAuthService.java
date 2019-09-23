package com.balabas.smarthouse.server.auth;

import com.balabas.smarthouse.server.exception.DeviceOnServerAuthorizationException;
import com.balabas.smarthouse.server.exception.ResourceNotFoundException;
import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.request.DeviceRequest;

public interface ServerAuthService {

	boolean checkServerKeyHash(String keySha1);
	
	boolean checkDeviceRegistrationRequest(DeviceRequest request) throws DeviceOnServerAuthorizationException;
	
	boolean checkDeviceOnDataUpdatedRequest(DeviceRequest request, Device device) throws DeviceOnServerAuthorizationException, ResourceNotFoundException;
	
	String getServerKeyHashed();
}
