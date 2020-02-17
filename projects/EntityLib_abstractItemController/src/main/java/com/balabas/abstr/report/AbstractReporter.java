package com.balabas.abstr.report;

import java.io.ByteArrayInputStream;
import java.util.List;

import org.springframework.http.HttpHeaders;
import org.springframework.http.MediaType;

public interface AbstractReporter<T> {

	ByteArrayInputStream getReport(List<T> entities);
	
	HttpHeaders getHeaders();

	MediaType getContentType();
	
}
