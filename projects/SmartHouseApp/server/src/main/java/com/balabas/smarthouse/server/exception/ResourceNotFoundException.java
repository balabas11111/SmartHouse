package com.balabas.smarthouse.server.exception;

import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.ResponseStatus;

@ResponseStatus(value = HttpStatus.NOT_FOUND)
public class ResourceNotFoundException extends Exception {

	private static final long serialVersionUID = 1L;
	private static final String MESSAGE_PREFFIX_RESOURCE = "Resource ";
	private static final String MESSAGE_PREFFIX = " Not found Id = ";
	
	private static final String DEFAULT_MESSAGE_PREFFIX = MESSAGE_PREFFIX_RESOURCE + MESSAGE_PREFFIX;

	public ResourceNotFoundException(Class<?> clazz, String id) {
        super(clazz.getSimpleName() + MESSAGE_PREFFIX + id);
    }
	
	public ResourceNotFoundException(String id) {
		super(DEFAULT_MESSAGE_PREFFIX + id);
	}

	public ResourceNotFoundException(Long id) {
		super(DEFAULT_MESSAGE_PREFFIX + Long.toString(id));
	}
}
