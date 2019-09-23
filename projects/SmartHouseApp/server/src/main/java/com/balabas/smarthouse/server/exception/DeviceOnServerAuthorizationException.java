package com.balabas.smarthouse.server.exception;

import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.ResponseStatus;

@ResponseStatus(value = HttpStatus.FORBIDDEN)
public class DeviceOnServerAuthorizationException extends Exception {

	private static final long serialVersionUID = 1L;
	private static final String MESSAGE_DEVICE = "Device id=";
	private static final String MESSAGE_IP = " ip=";
	
	public static final String MESSAGE_SERVER_KEY = " serverKey=";
	public static final String MESSAGE_DEVICE_KEY = " deviceKey=";
	public static final String MESSAGE_NON_REGISTERED_DEVICE_KEY = " non registered deviceKey=";

	public DeviceOnServerAuthorizationException(String id, String key, String ip) {
        super(MESSAGE_DEVICE + id + MESSAGE_SERVER_KEY + key + MESSAGE_IP + ip);
    }
	
	public DeviceOnServerAuthorizationException(String id, String key, String ip, String kind) {
        super(MESSAGE_DEVICE + id + kind + key + MESSAGE_IP + ip);
    }
	
}
