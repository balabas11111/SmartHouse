package com.balabas.smarthouse.server.controller;

import java.net.UnknownHostException;
import java.util.List;

import javax.servlet.http.HttpServletRequest;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;
import org.thymeleaf.util.StringUtils;

import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.model.EntityFieldValue;
import com.balabas.smarthouse.server.entity.model.Group;
import com.balabas.smarthouse.server.entity.service.DeviceManageService;
import com.balabas.smarthouse.server.exception.DeviceOnServerAuthorizationException;

import lombok.extern.log4j.Log4j2;

@CrossOrigin(origins = { ControllerConstants.CROSS_ORIGIN_4200, ControllerConstants.CROSS_ORIGIN_80 })
@RestController
@RequestMapping(ControllerConstants.DEVICES_ROOT_V1)
@Log4j2
public class DeviceController {

	public static final String DEVICE_NAME_PARAM = "deviceId";
	
	@Autowired
	private DeviceManageService deviceService;

	@GetMapping("/lastEntityFieldValues")
	public ResponseEntity<List<EntityFieldValue>> getLastEntityFieldValuesByEntityId(
			@RequestParam("entityId") Long id) {

		return ResponseEntity.ok().body(deviceService.getLastEntityFieldValuesForEntity(id));
	}
	
	@GetMapping("/lastDeviceEntityFieldValues")
	public ResponseEntity<List<EntityFieldValue>> getLastEntityFieldValuesByDeviceId(
			@RequestParam("deviceId") Long id) {

		return ResponseEntity.ok().body(deviceService.getLastEntityFieldValuesForDevice(id));
	}

	@GetMapping("/register")
	public ResponseEntity<String> registerDevice(@RequestParam(DEVICE_NAME_PARAM) String name, HttpServletRequest httpRequest)
			throws UnknownHostException, DeviceOnServerAuthorizationException {

		log.debug("/register");

		Device device = new Device();
		device.setName(name);
		device.setIp(httpRequest.getRemoteAddr());

		if (StringUtils.isEmpty(device.getName())) {
			throw new IllegalArgumentException(device.getName());
		}
		
		if (StringUtils.isEmpty(device.getIp())) {
			throw new UnknownHostException(device.getName());
		}

		boolean exists = deviceService.processRegistrationRequest(device);

		HttpStatus status = exists?HttpStatus.OK:HttpStatus.CREATED; 
		
		return ResponseEntity.status(status).build();

	}
	
	@GetMapping("/dataChanged")
	public ResponseEntity<String> dataChangeDispatchedOnDevice(@RequestParam(DEVICE_NAME_PARAM) String name, HttpServletRequest httpRequest)
			throws UnknownHostException, DeviceOnServerAuthorizationException {

		log.info("/dataChanged " + name);

		return ResponseEntity.ok().build();
	}

	@GetMapping("/all")
	public ResponseEntity<List<Device>> getAllDevices(
			@RequestParam(value = DEVICE_NAME_PARAM, required = false) String deviceName,
			@RequestParam(value = "groupId", required = false) String groupName) {

		return ResponseEntity.ok().body(deviceService.getDevices());
	}

	@GetMapping("/requireUpdateDevices")
	public ResponseEntity<List<Device>> getAllDevicesRequireUpdate() {
		return ResponseEntity.ok().body(deviceService.getDevicesRequireUpdate());
	}

	@GetMapping("/requireUpdateGroups")
	public ResponseEntity<List<Group>> getAllGroupsRequireUpdate() {
		return ResponseEntity.ok().body(deviceService.getGroupsRequireUpdate());
	}

}
