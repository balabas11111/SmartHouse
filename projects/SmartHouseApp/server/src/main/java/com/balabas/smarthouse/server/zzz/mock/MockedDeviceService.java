package com.balabas.smarthouse.server.zzz.mock;

import java.nio.charset.StandardCharsets;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.annotation.PostConstruct;
import javax.naming.AuthenticationException;

import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Profile;
import org.springframework.context.event.ContextRefreshedEvent;
import org.springframework.context.event.ContextStartedEvent;
import org.springframework.context.event.EventListener;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpMethod;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Component;
import org.springframework.web.client.RestTemplate;
import org.springframework.web.servlet.function.EntityResponse;

import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.request.DeviceRequest;
import com.balabas.smarthouse.server.security.DeviceSecurityService;
import com.balabas.smarthouse.server.service.DeviceService;
import com.google.common.hash.Hashing;

import lombok.extern.log4j.Log4j2;

@Component
//@Profile(value = "mock")
@Log4j2
public class MockedDeviceService implements InitializingBean {

	private static final String ROOT = "http://localhost/api/v1";
	
	private static final String DEVICES_IS_SERVER_ONLINE_ROOT = "/security/online";
	private static final String DEVICES_REGISTER_ROOT = "/security/register";
	private static final String DEVICES_DATA_UPDATE_ROOT = "/devices/data?deviceId=";
	
	@Value("${smarthouse.server.mock}")
    private boolean mockActive;
	
	@Value("${smarthouse.server.key}")
	private String serverKey;
	
	private String serverKeyHash;
	
	private boolean contStarted = false;
	
	private boolean initDone = false;
	
	@Autowired
	private DeviceService deviceService;
	
	@Autowired
	private DeviceSecurityService secService;
	
	private List<DeviceRequest> reqs;
	
	private Map<String, String> serverKeys = new HashMap<>();
	private Map<String, String> deviceKeys = new HashMap<>();
	
	@Override
	public void afterPropertiesSet() throws Exception {
		this.serverKeyHash = hash(serverKey);
		log.info("Mock Key hashed =" + this.serverKeyHash);
	}
	
	public void initMocks() {
		log.info("-----Server context was started MockedDevice-----");
		reqs = ServerValuesMockUtil.getDevicesMock(3);
		 
		reqs.stream().forEach(request->{
			 sendIsOnlineRequest(request);
			 sendRegistrationRequest(request);
		 });
		 
		 initDone = true;
		
		this.contStarted = true;
	}
	
	@Scheduled(fixedRate = 10000)
    public void mockDeviceDataUpdateRequest() {
		if(!mockActive || !initDone || !contStarted) {
			return;
		}
        log.info("Mock data update dispatched");
        
        reqs.stream().forEach(request->{
        	sendDataUpdatedRequest(request);
		 });
    }
	
	private boolean sendIsOnlineRequest(DeviceRequest request) {
		RestTemplate restTemplate = new RestTemplate();
		String url = ROOT + DEVICES_IS_SERVER_ONLINE_ROOT+"?deviceId="+request.getDeviceId();
		
		HttpHeaders headers = new HttpHeaders();
		
		HttpEntity ent = new HttpEntity(headers);
		Map<String,String> params = new HashMap<>();
		
		
		ResponseEntity<String> result = restTemplate.exchange(url, HttpMethod.GET, ent, String.class, params);
		
		return result.getStatusCode().equals(HttpStatus.OK);
	}
	
	private boolean sendRegistrationRequest(DeviceRequest request) {
		RestTemplate restTemplate = new RestTemplate();
		String url = ROOT + DEVICES_REGISTER_ROOT;
		
		HttpHeaders headers = new HttpHeaders();
		
		HttpEntity ent = new HttpEntity(request, headers);
		Map<String,String> params = new HashMap<>();
		
		
		ResponseEntity<String> result = restTemplate.exchange(url, HttpMethod.POST, ent, String.class, params);
		
		OnDevice_processRegistrationRequest(result, request);
		
		return result.getStatusCode().equals(HttpStatus.OK);
	}
	
	private boolean OnDevice_processRegistrationRequest(ResponseEntity<String> result, DeviceRequest request) {
		
		String serverToken = result.getHeaders().get(HttpHeaders.AUTHORIZATION).get(0);
		
		String uDeviceToken = serverKeyHash+":"+serverToken+":"+request.getDeviceKey();
		String uServerToken = serverToken+serverKey+request.getDeviceId();
		
		String deviceHash = hash(uDeviceToken);
		String serverHash = hash(uServerToken);
		
		log.info("---------------Mocked service Generated--------------");
		log.info("serverToken = "+ serverToken);
		log.info("deviceHash = "+ deviceHash);
		log.info("serverHash = "+ serverHash);
		log.info("---------------Security context--------------");
		log.info(secService.getDeviceSecurityContext(request.getDeviceId()));

		serverKeys.put(request.getDeviceId(), serverHash);
		deviceKeys.put(request.getDeviceId(), deviceHash);
		
		return true;
	}
	
	public boolean OnDevice_validateServerKey(HttpHeaders headers, String deviceId) {
		boolean hasAuthHeader = headers!=null 
				&& headers.containsKey(HttpHeaders.AUTHORIZATION)
				&& !headers.get(HttpHeaders.AUTHORIZATION).isEmpty();
		
		if(hasAuthHeader) {
			String receivedKey = headers.get(HttpHeaders.AUTHORIZATION).get(0);
			
			String expected = serverKeys.get(deviceId);
			/*
			log.info("receivedKey = "+ receivedKey);
			log.info("expected    = "+ expected);
			*/
			if(!expected.equals(receivedKey)) {
				log.error("bad key");
			}
		}
		
		return true;
	}
	
	private boolean OnDevice_appendDeviceKeyToRequest(HttpHeaders headers, String deviceId) {
		String deviceKey = deviceKeys.get(deviceId);
		
		headers.set(HttpHeaders.AUTHORIZATION, deviceKey);
		
		return true;
	}
	
	private boolean sendDataUpdatedRequest(DeviceRequest req) {
		
		DeviceRequest request = DeviceRequest.builder()
				.deviceId(req.getDeviceId()).build();
		
		RestTemplate restTemplate = new RestTemplate();
		String url = ROOT + DEVICES_DATA_UPDATE_ROOT + req.getDeviceId();
		
		HttpHeaders headers = new HttpHeaders();
		
		OnDevice_appendDeviceKeyToRequest(headers, req.getDeviceId());
		
		HttpEntity ent = new HttpEntity(headers);
		Map<String,String> params = new HashMap<>();
		
		
		
		ResponseEntity<String> result = restTemplate.exchange(url, HttpMethod.GET, ent, String.class, params);
		
		return result.getStatusCode().equals(HttpStatus.OK);
	}
	
	private String hash(String value) {
		return Hashing.sha1().hashString(value, StandardCharsets.UTF_8).toString();
	}
	
}