package com.balabas.smarthouse.server.exception;

import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.ResponseStatus;

@ResponseStatus(value = HttpStatus.BAD_REQUEST)
public class BadValueException extends Exception {

	private static final long serialVersionUID = 1L;

	public BadValueException(String message, String data) {
        super(message+" value:"+data);
    }
	
	public BadValueException(String message) {
		super(message);
	}
}
