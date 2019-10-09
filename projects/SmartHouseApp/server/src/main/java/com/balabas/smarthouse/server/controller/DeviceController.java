package com.balabas.smarthouse.server.controller;

import javax.servlet.http.HttpServletRequest;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpHeaders;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestHeader;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import com.balabas.smarthouse.server.controller.service.DeviceControllerService;
import com.balabas.smarthouse.server.exception.DeviceOnServerAuthorizationException;
import com.balabas.smarthouse.server.exception.ResourceNotFoundException;
import com.balabas.smarthouse.server.model.request.DeviceRequest;
import com.balabas.smarthouse.server.model.request.DeviceRequest.DeviceRequestType;

import lombok.extern.log4j.Log4j2;

@CrossOrigin(origins = {ControllerConstants.CROSS_ORIGIN_4200, ControllerConstants.CROSS_ORIGIN_80 })
@RestController
@RequestMapping(ControllerConstants.API_V1+ControllerConstants.DEVICES_ROOT)
@Log4j2
public class DeviceController {
	
	@Autowired
	private DeviceControllerService service;

	@GetMapping("/data")
	public ResponseEntity<String> dataChangeDispatchedOnDeviceGet(
			@RequestParam(value = "deviceId") String deviceId,
			@RequestHeader HttpHeaders headers,
			HttpServletRequest request) throws ResourceNotFoundException, DeviceOnServerAuthorizationException {
		log.debug("DataChanged GET dispatched on " + deviceId + " from "+request.getRemoteAddr());
		
		DeviceRequest deviceRequest = new DeviceRequest();
		deviceRequest.setDeviceId(deviceId);
		deviceRequest.setIp(request.getRemoteAddr());
		deviceRequest.setHeaders(headers);
		deviceRequest.setRequestType(DeviceRequestType.DATA_UPDATE_EVENT);
		
		return service.processDataChangedOnDeviceRequest(deviceRequest, false).toResponseEntity();
	}

	@PostMapping("/data")
	public ResponseEntity<String> dataChangeDispatchedOnDevicePost(
			@RequestBody DeviceRequest deviceRequest,
			@RequestHeader HttpHeaders headers,
			HttpServletRequest request) throws ResourceNotFoundException, DeviceOnServerAuthorizationException {
		
		log.info("DataChanged dispatched on " + deviceRequest.getDeviceId() + " " + deviceRequest.getJsonOrData());
		deviceRequest.setIp(request.getRemoteAddr());
		deviceRequest.setHeaders(headers);
		deviceRequest.setRequestType(DeviceRequestType.DATA_UPDATE_EVENT);

		return service.processDataChangedOnDeviceRequest(deviceRequest, true).toResponseEntity();
	}
	
}
