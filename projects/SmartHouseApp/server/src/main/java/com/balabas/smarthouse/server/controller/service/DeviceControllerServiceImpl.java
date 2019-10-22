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

	@Autowired
	private IDeviceManageService deviceService;

	@Autowired
	private DeviceSecurityService securityService;

	private boolean isOnline = false;

	@Override
	public DeviceRequestResult<String> processIsServerOnlineRequest(DeviceRequest request) {

		if (!isOnline) {
			return DeviceRequestResult.from(HttpStatus.NOT_ACCEPTABLE, IS_OFFLINE_MESSAGE);
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
				IDevice device = deviceService.getManagedDeviceByName(request.getDeviceId());
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
