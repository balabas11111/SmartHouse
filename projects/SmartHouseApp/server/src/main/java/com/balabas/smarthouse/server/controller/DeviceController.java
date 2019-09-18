package com.balabas.smarthouse.server.controller;

import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.validation.Valid;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import com.balabas.smarthouse.server.exception.ResourceNotFoundException;
import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.DeviceRegistrationResult;
import com.balabas.smarthouse.server.service.DeviceService;
import com.balabas.smarthouse.server.service.DeviceServiceImpl;

import lombok.extern.log4j.Log4j2;

@CrossOrigin(origins = "http://localhost:4200")
@RestController
@RequestMapping("/api/v1")
@Log4j2
public class DeviceController {

	@Autowired
	private DeviceService deviceService;
	
	@GetMapping("/devices")
	public List<Device> getAllDevices(){
		return deviceService.getDevices();
	}
	
	@GetMapping("/devices/{deviceId}")
	public ResponseEntity<Device> getDeviceById(@PathVariable(value = "deviceId") String deviceId)
			throws ResourceNotFoundException {
		Device device = deviceService.getDeviceByDeviceId(deviceId).orElseThrow(()->new ResourceNotFoundException(deviceId));
		
		return ResponseEntity.ok().body(device);
	}
	
	@PostMapping("/devices/register")
	public ResponseEntity<DeviceRegistrationResult> registerDevice(@Valid @RequestBody Device device, HttpServletRequest request){
		log.info("DeviceController registerDevice Addr=" + request.getRemoteAddr());
		DeviceRegistrationResult result = deviceService.registerDevice(device);		
		return ResponseEntity.ok().body(result);
	}
	
	@PostMapping("/devices/registerStr")
	public ResponseEntity<DeviceRegistrationResult> registerDeviceStr(@Valid @RequestBody String device){
		log.info("DeviceController registerDeviceStr "+device);
		return ResponseEntity.ok().body(new DeviceRegistrationResult());
	}
}
