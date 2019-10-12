package com.balabas.smarthouse.server.exception;

import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.ResponseStatus;

@ResponseStatus(value = HttpStatus.BAD_REQUEST)
public class BadDataException extends Exception {

	private static final long serialVersionUID = 1L;

	public BadDataException(String message, String data) {
        super(message+" data:"+data);
    }
	
	public BadDataException(String message) {
		super(message);
	}
}
