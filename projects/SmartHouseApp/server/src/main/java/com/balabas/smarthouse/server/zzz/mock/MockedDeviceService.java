package com.balabas.smarthouse.server.zzz.mock;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadPoolExecutor;

import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.event.ContextRefreshedEvent;
import org.springframework.context.event.EventListener;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpMethod;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Component;
import org.springframework.web.client.RestTemplate;

import com.balabas.smarthouse.server.entity.service.DeviceSecurityService;
import com.balabas.smarthouse.server.model.request.DeviceRequest;
import com.google.common.hash.Hashing;

import lombok.extern.log4j.Log4j2;

@Component
// @Profile(value = "mock")
@Log4j2
@SuppressWarnings("rawtypes")
public class MockedDeviceService implements InitializingBean {

	private static final String ROOT = "http://localhost/api/v1";

	private static final String DEVICES_REGISTER_ROOT = "/devices/register?deviceId=";

	@Value("${smarthouse.server.mock}")
	private boolean mockActive;
	
	@Value("${smarthouse.server.mock.count:0}")
	private int mockCount;

	@Value("${smarthouse.server.key}")
	private String serverKey;

	private String serverKeyHash;

	private boolean contStarted = false;

	private boolean initDone = false;

	private int initStep = 0;
	
	@Autowired
	private DeviceSecurityService securityService;

	private List<DeviceRequest> reqs;

	@Override
	public void afterPropertiesSet() throws Exception {
		this.serverKeyHash = hash(serverKey);
		log.info("Mock Key hashed =" + this.serverKeyHash);
	}

	public void initMocks() throws IOException {
		log.info("-----Server context was started MockedDevice-----");
		reqs = ServerValuesMockUtil.getDevicesMockedRegisterRequest(mockCount);

		if(reqs.size()>0) {
			ThreadPoolExecutor executor = (ThreadPoolExecutor) Executors.newFixedThreadPool(reqs.size());
	
			reqs.stream().forEach(request -> {
				RegisterTask task = new RegisterTask(request, this);
				executor.execute(task);
			});
	
			executor.shutdown();
		}
		initDone = true;
	}

	@Scheduled(fixedRate = 10000)
	public void mockDeviceDataUpdateRequest() throws IOException {
		if (!mockActive || !contStarted) {
			return;
		}
		if (initStep < 1) {
			initStep++;
		} else {
			if (!initDone) {
				initMocks();
			}
		}

		log.debug("Mock data update dispatched");
	}

	@SuppressWarnings("unchecked")
	public boolean sendRegistrationRequest(DeviceRequest request) {
		RestTemplate restTemplate = new RestTemplate();
		String url = ROOT + DEVICES_REGISTER_ROOT + request.getDeviceId();

		HttpHeaders headers = new HttpHeaders();

		HttpEntity ent = new HttpEntity(request, headers);
		Map<String, String> params = new HashMap<>();

		ResponseEntity<String> result = restTemplate.exchange(url, HttpMethod.GET, ent, String.class, params);

		OnDeviceProcessRegistrationRequest(result, request);

		return result.getStatusCode().equals(HttpStatus.OK);
	}

	private boolean OnDeviceProcessRegistrationRequest(ResponseEntity<String> result, DeviceRequest request) {

		//List<String> authHeaders = result.getHeaders().getOrEmpty(HttpHeaders.AUTHORIZATION);
		//String serverToken = (!authHeaders.isEmpty()) ? authHeaders.get(0) : result.getBody();
		log.info("OnDeviceProcessRegistrationRequest");

		return true;
	}

	public boolean OnDeviceValidateServerKey(HttpHeaders headers, String deviceId) {
		boolean hasAuthHeader = headers != null && headers.containsKey(HttpHeaders.AUTHORIZATION)
				&& !headers.get(HttpHeaders.AUTHORIZATION).isEmpty();

		if (hasAuthHeader) {
			String receivedKey = headers.get(HttpHeaders.AUTHORIZATION).get(0);

			String expected = securityService.getServerKey();
			if (!expected.equals(receivedKey)) {
				log.debug("bad key");
			}
		}

		return true;
	}

	private String hash(String value) {
		return Hashing.sha1().hashString(value, StandardCharsets.UTF_8).toString();
	}

	@EventListener(classes = { ContextRefreshedEvent.class })
	public void onContextStarted() {
		log.debug("-----Mocked context was started-----");
		this.contStarted = true;
	}

}
