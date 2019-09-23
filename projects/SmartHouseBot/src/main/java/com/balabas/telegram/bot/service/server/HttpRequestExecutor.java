package com.balabas.telegram.bot.service.server;

import java.io.UnsupportedEncodingException;
import java.util.List;
import java.util.Map;

import org.json.JSONObject;
import org.springframework.http.ResponseEntity;

import com.balabas.smarthouse.server.model.Device;

public interface HttpRequestExecutor {

	ResponseEntity<String> executeGetRequest(String url, Map<String, String> params) throws UnsupportedEncodingException;
	
	ResponseEntity<Device> executeGetRequestDevice(String url, Map<String, String> params) throws UnsupportedEncodingException;
	
	ResponseEntity<List<Device>> executeGetRequestDeviceList(String url, Map<String, String> params) throws UnsupportedEncodingException;
}
