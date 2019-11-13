package com.balabas.smarthouse.server.service;

import java.util.Map;

import org.springframework.http.HttpHeaders;
import org.springframework.http.ResponseEntity;
import org.springframework.util.MultiValueMap;

public interface HttpRequestExecutor {

	ResponseEntity<String> executeGetRequest(String url, HttpHeaders headers, Map<String, String> params);

	ResponseEntity<String> executePostRequest(String url, HttpHeaders headers, String body);
	
	ResponseEntity<String> executePostRequest(String url, HttpHeaders headers, MultiValueMap<String, Object> map);
}
