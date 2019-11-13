package com.balabas.smarthouse.server.service;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.util.Map;

import org.springframework.http.HttpEntity;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpMethod;
import org.springframework.http.ResponseEntity;
import org.springframework.http.client.SimpleClientHttpRequestFactory;
import org.springframework.stereotype.Service;
import org.springframework.util.MultiValueMap;
import org.springframework.web.client.RestTemplate;

@Service
public class HttpRequestExecutorImpl implements HttpRequestExecutor {

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
		RestTemplate restTemplate = new RestTemplate();
	    
		HttpEntity<String> request = new HttpEntity<>(body, headers);
		
		restTemplate.setRequestFactory(new SimpleClientHttpRequestFactory());
        SimpleClientHttpRequestFactory rf = (SimpleClientHttpRequestFactory) restTemplate
                .getRequestFactory();
        rf.setReadTimeout(5000);
        rf.setConnectTimeout(5000);
		
		return restTemplate.postForEntity(url, request, String.class);
	}

	@Override
	public ResponseEntity<String> executeGetRequest(String url, HttpHeaders headers, Map<String,String> params){
		RestTemplate restTemplate = new RestTemplate();
		
		HttpEntity<String> request = new HttpEntity<>(headers);
		
		restTemplate.setRequestFactory(new SimpleClientHttpRequestFactory());
        SimpleClientHttpRequestFactory rf = (SimpleClientHttpRequestFactory) restTemplate
                .getRequestFactory();
        rf.setReadTimeout(5000);
        rf.setConnectTimeout(5000);
		
		return restTemplate.exchange(url, HttpMethod.GET, request, String.class, params);
		
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
