package com.balabas.smarthouse.server.controller;

import java.io.UnsupportedEncodingException;
import java.net.UnknownHostException;

import javax.servlet.http.HttpServletRequest;
import javax.validation.Valid;

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
import com.balabas.smarthouse.server.model.request.DeviceRequest;
import com.balabas.smarthouse.server.model.request.DeviceRequestResult;
import com.balabas.smarthouse.server.model.request.DeviceRequest.DeviceRequestType;

import lombok.extern.log4j.Log4j2;

@CrossOrigin(origins = {ControllerConstants.CROSS_ORIGIN_4200, ControllerConstants.CROSS_ORIGIN_80 })
@RestController
@RequestMapping(ControllerConstants.API_V1+ControllerConstants.SECURITY_ROOT)
@Log4j2
public class SecurityController {

	@Autowired
	private DeviceControllerService service;
	
	@GetMapping("/online")
	public ResponseEntity<String> checkIsOnline(
			@RequestParam(value = "deviceId", required = false) String deviceId,
			@RequestHeader HttpHeaders headers) throws UnsupportedEncodingException {

		log.debug("/online");
		
		DeviceRequest request = DeviceRequest.builder()
				.deviceId(deviceId)
				.headers(headers)
				.requestType(DeviceRequestType.CHECK_ONLINE).build();
		
		DeviceRequestResult<String> result = 
				service.processIsServerOnlineRequest(request);
		
		return result.toResponseEntity();
	}
	
	@GetMapping("/register")
	public ResponseEntity<String> isDeviceRegistered(
			@RequestParam(value = "deviceId", required = false) String deviceId,
			@RequestHeader HttpHeaders headers) throws UnknownHostException, DeviceOnServerAuthorizationException {

		log.debug("/register");
		
		DeviceRequest request = DeviceRequest.builder()
				.deviceId(deviceId)
				.headers(headers)
				.requestType(DeviceRequestType.CHECK_ONLINE).build();

		DeviceRequestResult<String> result =
				service.processDeviceIsRegisteredRequest(request);
		
		return result.toResponseEntity();
	}
	
	@PostMapping("/register")
	public ResponseEntity<String> registerDevice(
			@Valid @RequestBody DeviceRequest request,
			@RequestHeader HttpHeaders headers,
			HttpServletRequest httpRequest) throws UnknownHostException, DeviceOnServerAuthorizationException {

		log.debug("/register");
		
		request.setIp(httpRequest.getRemoteAddr());
		request.setHeaders(headers);
		request.setRequestType(DeviceRequestType.REGISTER);

		DeviceRequestResult<String> result =
				service.processDeviceRegistrationRequest(request);
		
		return result.toResponseEntity();
	}
	
}
