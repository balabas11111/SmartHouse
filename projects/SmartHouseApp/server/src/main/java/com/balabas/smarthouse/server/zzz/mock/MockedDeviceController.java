package com.balabas.smarthouse.server.zzz.mock;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestHeader;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import com.balabas.smarthouse.server.controller.ControllerConstants;
import com.balabas.smarthouse.server.exception.ResourceNotFoundException;
import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.service.DeviceService;

import lombok.extern.log4j.Log4j2;
import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_FIELD_GROUP;

@CrossOrigin(origins = {ControllerConstants.CROSS_ORIGIN_4200, ControllerConstants.CROSS_ORIGIN_80 })
@RestController
@RequestMapping(ControllerConstants.API_V1+ControllerConstants.DEVICES_ROOT)
@Log4j2
//@Profile(value = "mock")
public class MockedDeviceController {

	@Autowired
	private DeviceService deviceService;
	
	@Autowired
	private MockedDeviceService mockService;
	
	boolean doAlert;

	@GetMapping()
	public List<Device> getAllDevices() {
		log.info("Get all devices=");
		return deviceService.getDevices();
	}
	
	@GetMapping("/mock")
	public void mockAllDevices() throws IOException {
		log.info("mock all devices=");
		mockService.initMocks();
	}
	
	@GetMapping("/alertBme")
	public void noAlertBme(@RequestParam(value = "doAlert") boolean alert) throws IOException {
		log.info("Mocking doAlert = " + alert);
		doAlert=alert;
	}

	@GetMapping("{deviceId}")
	public ResponseEntity<Device> getDeviceById(@PathVariable(value = "deviceId") String deviceId)
			throws ResourceNotFoundException {
		Device device = deviceService.getDeviceByDeviceId(deviceId)
				.orElseThrow(() -> new ResourceNotFoundException(deviceId));

		return ResponseEntity.ok().body(device);
	}
	/*
	@GetMapping("/getData_{deviceId}")
	public ResponseEntity<String> executeGetData(
			@PathVariable(value = "deviceId") String deviceId,
			@RequestParam(value = DEVICE_FIELD_GROUP, required = false) String group) throws ResourceNotFoundException {
		JSONObject result = deviceService.executeGetData(deviceId, group);
		return ResponseEntity.ok().body(result.toString());
	}

	@GetMapping("/getDataOnDevice_{deviceId}")
	public ResponseEntity<String> executeGetDataOnDevice(@PathVariable(value = "deviceId") String deviceId,
			@RequestParam(value = DEVICE_FIELD_GROUP, required = false) String group) throws UnsupportedEncodingException, ResourceNotFoundException {

		return ResponseEntity.ok().body(deviceService.executeGetData(deviceId, group).toString());
	}
	*/
	@GetMapping("/mock_{deviceId}")
	public ResponseEntity<String> executeMockGetDataOnDevice(
			@PathVariable(value = "deviceId") String deviceId,
			@RequestHeader HttpHeaders headers,
			@RequestParam(value = DEVICE_FIELD_GROUP, required = false) String devEntGroup) throws UnsupportedEncodingException {

		String result = ((devEntGroup == null || devEntGroup.isEmpty())
				? ServerValuesMockUtil.getSettingsSensors(deviceId,doAlert) : ServerValuesMockUtil.getSensors(deviceId,doAlert))
						.toString();

		boolean isServerRequestValid = mockService.OnDeviceValidateServerKey(headers, deviceId);
		
		if(!isServerRequestValid) {
			return ResponseEntity.status(HttpStatus.UNAUTHORIZED).body("suck man");
		}
		
		log.debug("Mock result =" + result);

		return ResponseEntity.ok().body(result);
	}
	
	@PostMapping("/mock_{deviceId}")
	public ResponseEntity<String> executeMockPostDataOnDevice(
			@PathVariable(value = "deviceId") String deviceId,
			@RequestHeader HttpHeaders headers,
			@RequestBody String body,
			@RequestParam(value = DEVICE_FIELD_GROUP, required = false) String devEntGroup) throws UnsupportedEncodingException {


		log.info("POST Mock");

		return executeMockGetDataOnDevice(deviceId, headers, devEntGroup);
	}
	
}
