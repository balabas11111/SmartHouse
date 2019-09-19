package com.balabas.telegram.bot.service.server;

import java.io.UnsupportedEncodingException;
import java.util.List;
import java.util.Map;

import org.springframework.core.ParameterizedTypeReference;
import org.springframework.http.HttpMethod;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Service;
import org.springframework.web.client.RestTemplate;

import com.balabas.smarthouse.server.HttpRequestUtil;
import com.balabas.smarthouse.server.model.Device;

import lombok.extern.log4j.Log4j2;

@Log4j2
@Service
public class HttpRequestExecutorImpl implements HttpRequestExecutor {

	@Override
	public ResponseEntity<String> executeGetRequest(String url, Map<String, String> params) throws UnsupportedEncodingException {
		RestTemplate restTemplate = new RestTemplate();
		String execUrl = url + HttpRequestUtil.getParamsString(params);
		
		log.info("GET "+execUrl);
		
		return restTemplate.getForEntity(execUrl, String.class);
	}
	
	@Override
	public ResponseEntity<Device> executeGetRequestDevice(String url, Map<String, String> params) throws UnsupportedEncodingException {
		RestTemplate restTemplate = new RestTemplate();
		String execUrl = url + HttpRequestUtil.getParamsString(params);
		
		log.info("GET "+execUrl);
		
		return restTemplate.getForEntity(execUrl, Device.class);
	}

	@Override
	public ResponseEntity<List<Device>> executeGetRequestDeviceList(String url, Map<String, String> params)
			throws UnsupportedEncodingException {
		RestTemplate tmpl = new RestTemplate();
		String execUrl = url + HttpRequestUtil.getParamsString(params);
        
        return (tmpl.exchange(
        		execUrl,
                HttpMethod.GET,
                null,
                new ParameterizedTypeReference<List<Device>>(){}));
	}
	
}
