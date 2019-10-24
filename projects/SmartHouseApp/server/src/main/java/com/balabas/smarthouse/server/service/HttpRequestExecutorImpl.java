package com.balabas.smarthouse.server.service;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.util.List;
import java.util.Map;

import org.springframework.core.ParameterizedTypeReference;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpMethod;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.http.client.SimpleClientHttpRequestFactory;
import org.springframework.stereotype.Service;
import org.springframework.util.MultiValueMap;
import org.springframework.web.client.RestTemplate;

import com.balabas.smarthouse.server.entity.model.Device;

import lombok.extern.log4j.Log4j2;

@Log4j2
@Service
public class HttpRequestExecutorImpl implements HttpRequestExecutor {

	@Override
	public ResponseEntity<String> executeGetRequest(String url, Map<String, String> params) throws UnsupportedEncodingException {
		RestTemplate restTemplate = new RestTemplate();
		String execUrl = url + getParamsString(params);
		
		log.info("GET "+execUrl);
		
		return restTemplate.getForEntity(execUrl, String.class);
	}
	
	@Override
	public ResponseEntity<Device> executeGetRequestDevice(String url, Map<String, String> params) throws UnsupportedEncodingException {
		RestTemplate restTemplate = new RestTemplate();
		String execUrl = url + getParamsString(params);
		
		log.info("GET "+execUrl);
		
		return restTemplate.getForEntity(execUrl, Device.class);
	}

	@Override
	public ResponseEntity<List<Device>> executeGetRequestDeviceList(String url, Map<String, String> params)
			throws UnsupportedEncodingException {
		RestTemplate tmpl = new RestTemplate();
		String execUrl = url + getParamsString(params);
        
        return (tmpl.exchange(
        		execUrl,
                HttpMethod.GET,
                null,
                new ParameterizedTypeReference<List<Device>>(){}));
	}

	@Override
	public ResponseEntity<String> executePostRequest(String url, String body) throws UnsupportedEncodingException {
		RestTemplate rest = new RestTemplate();
		HttpHeaders headers = new HttpHeaders();
	    headers.setContentType(MediaType.APPLICATION_JSON);
	    
		HttpEntity<String> request = new HttpEntity<>(body, headers);
		return rest.postForEntity(url, request, String.class);
	}
	
	@Override
	public ResponseEntity<String> executeGetRequest(String url, HttpHeaders headers, Map<String,String> params){
		RestTemplate restTemplate = new RestTemplate();
		
		HttpEntity<String> request = new HttpEntity<>(headers);
		
		return restTemplate.exchange(url, HttpMethod.GET, request, String.class, params);
		
	}
	
	public static String getParamsString(Map<String, String> params) throws UnsupportedEncodingException {
		StringBuilder result = new StringBuilder();

		if (params != null && !params.isEmpty()) {
			result.append("?");

			for (Map.Entry<String, String> entry : params.entrySet()) {
				result.append(URLEncoder.encode(entry.getKey(), "UTF-8"));
				result.append("=");
				result.append(URLEncoder.encode(entry.getValue(), "UTF-8"));
				result.append("&");
			}
		}

		String resultString = result.toString();
		return resultString.length() > 0 ? resultString.substring(0, resultString.length() - 1) : resultString;
	}
	
	@Override
	public ResponseEntity<String> executePostRequest(String url, HttpHeaders headers, String body) {
		RestTemplate rest = new RestTemplate();
	    
		HttpEntity<String> request = new HttpEntity<>(body, headers);
		
		return rest.postForEntity(url, request, String.class);
	}

	@Override
	public ResponseEntity<String> executePostRequest(String url, HttpHeaders headers,
			MultiValueMap<String, Object> map) {
		RestTemplate restTemplate = new RestTemplate();
		
		HttpEntity<MultiValueMap<String, Object>> request= 
                new HttpEntity<MultiValueMap<String, Object>>(map, headers);
		
		restTemplate.setRequestFactory(new SimpleClientHttpRequestFactory());
        SimpleClientHttpRequestFactory rf = (SimpleClientHttpRequestFactory) restTemplate
                .getRequestFactory();
        rf.setReadTimeout(5000);
        rf.setConnectTimeout(5000);
		
		ResponseEntity<String> result =  restTemplate.postForEntity(url, request, String.class);
		return result;
	}
}
