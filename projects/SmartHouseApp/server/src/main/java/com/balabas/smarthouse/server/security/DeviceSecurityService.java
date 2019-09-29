package com.balabas.smarthouse.server.security;

import org.springframework.beans.factory.InitializingBean;

import com.balabas.smarthouse.server.exception.DeviceRequestValidateException;
import com.balabas.smarthouse.server.model.request.DeviceRequest;

public interface DeviceSecurityService extends InitializingBean {

	String getDeviceKey(String deviceId);
	
	String getServerKey(String deviceId);
	
	String processDeviceRegistrationRequest(DeviceRequest request) throws DeviceRequestValidateException;
	
	boolean validateDeviceRequestBase(DeviceRequest request) throws DeviceRequestValidateException;
	
	boolean validateDeviceRequestBaseSecurity(DeviceRequest request) throws DeviceRequestValidateException;
	
	boolean validateDeviceRequestRegistration(DeviceRequest request) throws DeviceRequestValidateException;
	
	boolean validateDeviceRequestDataUpdate(DeviceRequest request) throws DeviceRequestValidateException;
	
	DeviceSecurityContext getDeviceSecurityContext(String deviceId);
	
}
