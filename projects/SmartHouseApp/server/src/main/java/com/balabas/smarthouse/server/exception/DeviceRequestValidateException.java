package com.balabas.smarthouse.server.exception;

import org.apache.tomcat.websocket.AuthenticationException;
import org.springframework.http.HttpStatus;

import lombok.Getter;

public class DeviceRequestValidateException extends AuthenticationException {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	@Getter
	private HttpStatus status;
	@Getter
	private String message;
	
	public DeviceRequestValidateException(HttpStatus status, String message) {
		super(message);
		this.status = status;
		this.message = message;
    }
	
	public DeviceRequestValidateException(HttpStatus status) {
        this(status, status.name());
    }
	
}
