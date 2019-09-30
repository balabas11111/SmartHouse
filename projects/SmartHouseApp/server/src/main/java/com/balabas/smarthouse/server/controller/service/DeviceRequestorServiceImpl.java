package com.balabas.smarthouse.server.controller.service;

import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_FIELD_GROUP;

import java.util.Collections;
import java.util.Map;

import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.security.DeviceSecurityService;
import com.balabas.smarthouse.server.service.HttpRequestExecutor;

import lombok.extern.log4j.Log4j2;

@Log4j2
@Service
public class DeviceRequestorServiceImpl implements DeviceRequestorService {

	@Autowired
	HttpRequestExecutor executor;
	
	@Autowired
	DeviceSecurityService securityService;

	@Override
	public String executeGetDataOnDevice(Device device, String deviceEntityGroup) {
		if (device != null) {
			if (device.getDataUrl() != null) {
				Map<String, String> params = (deviceEntityGroup != null && !deviceEntityGroup.isEmpty())
						? Collections.singletonMap(DEVICE_FIELD_GROUP, deviceEntityGroup)
						: Collections.emptyMap();

				String serverKey = securityService.getServerKey(device.getDeviceId());		
				
				if(serverKey == null) {
					log.error("Empty Server key " + device.getDeviceId());
					return null;
				}
				
				HttpHeaders headers = new HttpHeaders();
				headers.set(HttpHeaders.AUTHORIZATION, serverKey);
						
				ResponseEntity<String> result = executor.executeGetRequest(device.getDataUrl(), headers, params);

				if (result.getStatusCode().equals(HttpStatus.OK)) {
					return result.getBody();
				}

			} else {
				log.error("Empty data url deviceId=" + device.getDeviceId());
			}
		}

		log.error("Empty device");

		return null;
	}
	
	@Override
	public String executePostDataOnDevice(Device device, JSONObject json){
		HttpHeaders headers = new HttpHeaders();
		headers.setContentType(MediaType.APPLICATION_JSON);
		headers.add(HttpHeaders.AUTHORIZATION, securityService.getServerKey(device.getDeviceId()));
		
		return executor.executePostRequest(device.getDataUrl(), headers, json.toString()).getBody();
	}
}
