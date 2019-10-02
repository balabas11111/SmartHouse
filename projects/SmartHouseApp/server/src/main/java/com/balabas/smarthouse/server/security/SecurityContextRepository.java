package com.balabas.smarthouse.server.security;

public interface SecurityContextRepository {

	DeviceSecurityContext put(String deviceId, String deviceKey, String serverKey);
	
	DeviceSecurityContext get(String deviceId);
}
