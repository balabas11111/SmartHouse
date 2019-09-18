package com.balabas.smarthouse.server.exception;

import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.ResponseStatus;

@ResponseStatus(value = HttpStatus.NOT_FOUND)
public class ResourceNotFoundException extends Exception {

	private static final long serialVersionUID = 1L;
	private static final String MESSAGE_PREFFIX = "Resource Not found Id = ";

	public ResourceNotFoundException(String id) {
		super(MESSAGE_PREFFIX + id);
	}

	public ResourceNotFoundException(Long id) {
		super(MESSAGE_PREFFIX + Long.toString(id));
	}
}
