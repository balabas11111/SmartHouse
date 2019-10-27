package com.balabas.smarthouse.server.controller.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.event.ContextClosedEvent;
import org.springframework.context.event.ContextRefreshedEvent;
import org.springframework.context.event.ContextStoppedEvent;
import org.springframework.context.event.EventListener;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpStatus;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.service.IDeviceManageService;
import com.balabas.smarthouse.server.exception.DeviceRequestValidateException;
import com.balabas.smarthouse.server.model.request.DeviceRequest;
import com.balabas.smarthouse.server.model.request.DeviceRequestResult;
import com.balabas.smarthouse.server.security.DeviceSecurityService;


import lombok.extern.log4j.Log4j2;

@Log4j2
@Service
public class DeviceControllerServiceImpl implements DeviceControllerService {

	public static final String IS_OFFLINE_MESSAGE = "{\"status\":\"OFFLINE\"}";
	public static final String IS_ONLINE_MESSAGE = "{\"status\":\"ONLINE\"}";
	public static final String STATUS_MESSAGE = "{\"status\":\"%s\"}";
	public static final String OK_MESSAGE = "{\"status\":\"OK\"}";
	
	@Autowired
	private IDeviceManageService deviceService;

	@Autowired
	private DeviceSecurityService securityService;

	private boolean isOnline = false;

	@Override
	public DeviceRequestResult<String> processIsServerOnlineRequest(DeviceRequest request) {

		if (!isOnline) {
			return DeviceRequestResult.from(HttpStatus.SERVICE_UNAVAILABLE, IS_OFFLINE_MESSAGE);
		}
		
		return DeviceRequestResult.from(HttpStatus.OK, IS_ONLINE_MESSAGE);

		/*
		 * try { securityService.validateDeviceRequestBase(request); return
		 * DeviceRequestResult.from(HttpStatus.OK, IS_ONLINE_MESSAGE);
		 * 
		 * } catch (DeviceRequestValidateException e) { return
		 * DeviceRequestResult.from(e.getStatus()); }
		 */
	}
	
	@Override
	public DeviceRequestResult<String> processDeviceIsRegisteredRequest(DeviceRequest request) {
		
		if (request.getDeviceId() == null) {
			return DeviceRequestResult.from(HttpStatus.BAD_REQUEST, IS_ONLINE_MESSAGE);
		}
		
		HttpStatus status = deviceService.getDeviceByName(request.getDeviceId())!=null?
							HttpStatus.NOT_ACCEPTABLE:HttpStatus.OK;
		String message = String.format(STATUS_MESSAGE, status.name());
		return DeviceRequestResult.from(status, message);
		
	}

	@Override
	public DeviceRequestResult<String> processDeviceRegistrationRequest(DeviceRequest request) {
		try {
			log.info("Register device " + request.toString());
			String token = securityService.processDeviceRegistrationRequest(request);

			deviceService.processRegistrationRequest(request);

			HttpHeaders headers = new HttpHeaders();
			headers.set(HttpHeaders.AUTHORIZATION, token);

			return DeviceRequestResult.from(HttpStatus.OK, token);

		} catch (DeviceRequestValidateException e) {
			return DeviceRequestResult.from(e.getStatus());
		}
	}

	@Override
	public DeviceRequestResult<String> processDataChangedOnDeviceRequest(DeviceRequest request, boolean withData) {
		try {
			if(request.hasData()) {
				securityService.validateDeviceRequestDataUpdate(request);
				IDevice device = deviceService.getDeviceByName(request.getDeviceId());
				device.getTimer().setActionForced(true);
			}
			return DeviceRequestResult.from(HttpStatus.OK);
			

		} catch (DeviceRequestValidateException e) {
			return DeviceRequestResult.from(e.getStatus());
		}
	}

	@EventListener(classes = { ContextRefreshedEvent.class })
	public void onContextStarted() {
		log.info("-----Server context was started-----");
		this.isOnline = true;
	}

	@EventListener(classes = { ContextStoppedEvent.class, ContextClosedEvent.class })
	public void onContextDestroyed() {
		log.info("-----Server context was destroyed-----");
		this.isOnline = false;
	}

}
