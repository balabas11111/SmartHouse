package com.balabas.smarthouse.server.controller;

import java.io.UnsupportedEncodingException;
import java.net.UnknownHostException;
import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.validation.Valid;

import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import com.balabas.smarthouse.server.auth.ServerAuthService;
import com.balabas.smarthouse.server.exception.DeviceOnServerAuthorizationException;
import com.balabas.smarthouse.server.exception.ResourceNotFoundException;
import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.request.DeviceRequest;
import com.balabas.smarthouse.server.model.request.DeviceRegistrationResult;
import com.balabas.smarthouse.server.service.DeviceService;
import com.balabas.smarthouse.server.util.ServerValuesMockUtil;

import lombok.extern.log4j.Log4j2;
import static com.balabas.smarthouse.server.DeviceConstants.GROUP;

@CrossOrigin(origins = "http://localhost:4200")
@RestController
@RequestMapping("/api/v1")
@Log4j2
public class DeviceController {

	@Autowired
	private DeviceService deviceService;

	@Autowired
	private ServerAuthService authService;

	@GetMapping("/devices")
	public List<Device> getAllDevices() {
		log.info("Get all devices=");
		return deviceService.getDevices();
	}

	@GetMapping("/devices{deviceId}")
	public ResponseEntity<Device> getDeviceById(@PathVariable(value = "deviceId") String deviceId)
			throws ResourceNotFoundException {
		Device device = deviceService.getDeviceByDeviceId(deviceId)
				.orElseThrow(() -> new ResourceNotFoundException(deviceId));

		return ResponseEntity.ok().body(device);
	}

	@PostMapping("/devices/register")
	public ResponseEntity<DeviceRegistrationResult> registerDevice(@Valid @RequestBody DeviceRequest registrRequest,
			HttpServletRequest request) throws UnknownHostException, DeviceOnServerAuthorizationException {
		log.info("DeviceController registerDevice Addr=" + request.getRemoteAddr());

		registrRequest.setIp(request.getRemoteAddr());
		authService.checkDeviceRegistrationRequest(registrRequest);

		DeviceRegistrationResult result = deviceService.registerDevice(registrRequest);
		return ResponseEntity.ok().body(result);
	}

	@GetMapping("/devices/data")
	public ResponseEntity<String> dataChangeDispatchedOnDeviceStr(@RequestParam(value = "deviceId") String deviceId,
			HttpServletRequest request) throws ResourceNotFoundException, DeviceOnServerAuthorizationException {
		log.info("DataChanged GET dispatched on " + deviceId);
		
		DeviceRequest deviceRequest = new DeviceRequest();
		deviceRequest.setDeviceId(deviceId);
		deviceRequest.setIp(request.getRemoteAddr());
		
		deviceService.processDeviceDataUpdateDispatched(deviceRequest, false);

		return ResponseEntity.ok().body("OK");
	}

	@PostMapping("/devices/data")
	public ResponseEntity<String> dataChangeDispatchedOnDevice(@RequestBody DeviceRequest deviceRequest,
			HttpServletRequest request) throws ResourceNotFoundException, DeviceOnServerAuthorizationException {
		log.info("DataChanged dispatched on " + deviceRequest.getDeviceId() + " " + deviceRequest.getJsonOrData());
		deviceRequest.setIp(request.getRemoteAddr());

		deviceService.processDeviceDataUpdateDispatched(deviceRequest, true);

		return ResponseEntity.ok().body("OK");
	}

	@GetMapping("/devices/getData_{deviceId}")
	public ResponseEntity<String> executeGetData(@PathVariable(value = "deviceId") String deviceId,
			@RequestParam(value = GROUP, required = false) String devEntGroup) throws ResourceNotFoundException {
		JSONObject result = deviceService.executeGetData(deviceId, devEntGroup);
		return ResponseEntity.ok().body(result.toString());
	}

	@GetMapping("/devices/getDataOnDevice_{deviceId}")
	public ResponseEntity<String> executeGetDataOnDevice(@PathVariable(value = "deviceId") String deviceId,
			@RequestParam(value = GROUP, required = false) String devEntGroup) throws UnsupportedEncodingException {

		return ResponseEntity.ok().body(deviceService.executeGetDataOnDevice(deviceId, devEntGroup));
	}

	@GetMapping("/devices/mock_{deviceId}")
	public ResponseEntity<String> executeMockGetDataOnDevice(@PathVariable(value = "deviceId") String deviceId,
			@RequestParam(value = GROUP, required = false) String devEntGroup) throws UnsupportedEncodingException {

		String result = ((devEntGroup == null || devEntGroup.isEmpty())
				? ServerValuesMockUtil.getSettingsSensors(deviceId) : ServerValuesMockUtil.getSensors(deviceId))
						.toString();

		log.info("Mock result =" + result);

		return ResponseEntity.ok().body(result);
	}
}
