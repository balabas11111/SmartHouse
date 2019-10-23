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

import com.balabas.smarthouse.server.entity.alarm.EntityAlarm;
import com.balabas.smarthouse.server.entity.alarm.EntityFieldMaxValueAlarm;
import com.balabas.smarthouse.server.entity.alarm.IAlarm;
import com.balabas.smarthouse.server.entity.alarm.IEntityAlarm;
import com.balabas.smarthouse.server.entity.alarm.IEntityAlarmService;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.service.IDeviceManageService;
import com.balabas.smarthouse.server.model.request.DeviceRequest;
import com.balabas.smarthouse.server.security.DeviceSecurityService;
import com.google.common.hash.Hashing;

import lombok.extern.log4j.Log4j2;

@Component
// @Profile(value = "mock")
@Log4j2
@SuppressWarnings("rawtypes")
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

	private int initStep = 0;

	@Autowired
	private IEntityAlarmService alarmService;

	@Autowired
	private IDeviceManageService deviceService;

	@Autowired
	private DeviceSecurityService secService;

	private List<DeviceRequest> reqs;

	private Map<String, String> serverKeys = new HashMap<>();
	private Map<String, String> deviceKeys = new HashMap<>();

	private boolean alarmRegistered = false;

	@Override
	public void afterPropertiesSet() throws Exception {
		this.serverKeyHash = hash(serverKey);
		log.info("Mock Key hashed =" + this.serverKeyHash);
	}

	public void initMocks() throws IOException {
		log.info("-----Server context was started MockedDevice-----");
		reqs = ServerValuesMockUtil.getDevicesMock(5);

		ThreadPoolExecutor executor = (ThreadPoolExecutor) Executors.newFixedThreadPool(reqs.size());
		
		reqs.stream().forEach(request -> {
			RegisterTask task = new RegisterTask(request, this);
			executor.execute(task);
		});

		executor.shutdown();
		initDone = true;
	}

	public void initAlarms() throws IOException {
		List<IEntityAlarm> alarms = alarmService.getActiveEntityAlarms();

		if (alarms != null && alarms.isEmpty()) {
			DeviceRequest req = reqs.get(0);
			IDevice device = deviceService.getManagedDeviceByName(req.getDeviceId());

			if (device != null) {
			String deviceName = device.getName();
			String entityName = "bme280";
			String entityFieldName = "t";

			

				IAlarm entityFieldAlarm = new EntityFieldMaxValueAlarm(entityFieldName, 31);

				IEntityAlarm alarm = new EntityAlarm();
				alarm.setDeviceName(deviceName);
				alarm.setEntityName(entityName);
				alarm.putAlarm(entityFieldAlarm);

				//alarmService.registerAlarm(alarm);
				//alarmService.activateAlarms(device);
				
				alarmRegistered = true;
			}
		}
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
			} else {
				if (!alarmRegistered) {
					initAlarms();
				}
			}
		}

		log.debug("Mock data update dispatched");
		if (initDone) {
			reqs.stream().forEach(request -> {
				sendDataUpdatedRequest(request);
			});
		}
	}

	public boolean sendIsOnlineRequest(DeviceRequest request) {
		RestTemplate restTemplate = new RestTemplate();
		String url = ROOT + DEVICES_IS_SERVER_ONLINE_ROOT + "?deviceId=" + request.getDeviceId();

		HttpHeaders headers = new HttpHeaders();

		HttpEntity ent = new HttpEntity<>(headers);
		Map<String, String> params = new HashMap<>();

		ResponseEntity<String> result = restTemplate.exchange(url, HttpMethod.GET, ent, String.class, params);

		return result.getStatusCode().equals(HttpStatus.OK);
	}

	@SuppressWarnings("unchecked")
	public boolean sendRegistrationRequest(DeviceRequest request) {
		RestTemplate restTemplate = new RestTemplate();
		String url = ROOT + DEVICES_REGISTER_ROOT;

		HttpHeaders headers = new HttpHeaders();

		HttpEntity ent = new HttpEntity(request, headers);
		Map<String, String> params = new HashMap<>();

		ResponseEntity<String> result = restTemplate.exchange(url, HttpMethod.POST, ent, String.class, params);

		OnDeviceProcessRegistrationRequest(result, request);

		return result.getStatusCode().equals(HttpStatus.OK);
	}

	private boolean OnDeviceProcessRegistrationRequest(ResponseEntity<String> result, DeviceRequest request) {

		List<String> authHeaders = result.getHeaders().getOrEmpty(HttpHeaders.AUTHORIZATION);
		String serverToken = (!authHeaders.isEmpty()) ? authHeaders.get(0) : result.getBody();

		String uDeviceToken = serverKeyHash + serverToken + request.getDeviceKey();
		String uServerToken = serverToken + serverKey + request.getDeviceId();

		String deviceHash = hash(uDeviceToken);
		String serverHash = hash(uServerToken);

		log.info("---------------Mocked service Generated--------------");
		log.info("serverToken = " + serverToken);
		log.info("deviceHash = " + deviceHash);
		log.info("serverHash = " + serverHash);
		log.info("---------------Security context--------------");
		log.info(secService.getDeviceSecurityContext(request.getDeviceId()));

		serverKeys.put(request.getDeviceId(), serverHash);
		deviceKeys.put(request.getDeviceId(), deviceHash);

		return true;
	}

	public boolean OnDeviceValidateServerKey(HttpHeaders headers, String deviceId) {
		boolean hasAuthHeader = headers != null && headers.containsKey(HttpHeaders.AUTHORIZATION)
				&& !headers.get(HttpHeaders.AUTHORIZATION).isEmpty();

		if (hasAuthHeader) {
			String receivedKey = headers.get(HttpHeaders.AUTHORIZATION).get(0);

			String expected = serverKeys.get(deviceId);
			if (!expected.equals(receivedKey)) {
				log.error("bad key");
			}
		}

		return true;
	}

	private boolean OnDeviceAppendDeviceKeyToRequest(HttpHeaders headers, String deviceId) {
		String deviceKey = deviceKeys.get(deviceId);

		headers.set(HttpHeaders.AUTHORIZATION, deviceKey);

		return true;
	}

	@SuppressWarnings({ "unchecked" })
	private boolean sendDataUpdatedRequest(DeviceRequest req) {

		RestTemplate restTemplate = new RestTemplate();
		String url = ROOT + DEVICES_DATA_UPDATE_ROOT + req.getDeviceId();

		HttpHeaders headers = new HttpHeaders();

		OnDeviceAppendDeviceKeyToRequest(headers, req.getDeviceId());

		HttpEntity ent = new HttpEntity(headers);
		Map<String, String> params = new HashMap<>();

		ResponseEntity<String> result = restTemplate.exchange(url, HttpMethod.GET, ent, String.class, params);

		return result.getStatusCode().equals(HttpStatus.OK);
	}

	private String hash(String value) {
		return Hashing.sha1().hashString(value, StandardCharsets.UTF_8).toString();
	}

	@EventListener(classes = { ContextRefreshedEvent.class })
	public void onContextStarted() {
		log.debug("-----Server context was started-----");
		this.contStarted = true;
	}

}
