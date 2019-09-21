package com.balabas.smarthouse.server.controller;

import java.io.UnsupportedEncodingException;
import java.net.UnknownHostException;
import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.validation.Valid;

import org.json.JSONException;
import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
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
import com.balabas.smarthouse.server.exception.ResourceNotFoundException;
import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.DeviceOnDataUpdatedRequest;
import com.balabas.smarthouse.server.model.DeviceRegistrationRequest;
import com.balabas.smarthouse.server.model.DeviceRegistrationResult;
import com.balabas.smarthouse.server.model.DeviceRegistrationResult.DeviceRegistrationStatus;
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

	@PostMapping("/devices/registerStr")
    public ResponseEntity<String> registerDeviceStr(@RequestBody String deviceRequest,
            HttpServletRequest request) {
        log.info("registerDeviceStr DeviceController registerDevice Addr=" + request.getRemoteAddr());
        log.info("deviceRequest=" + deviceRequest);
        
        return ResponseEntity.ok().body(deviceRequest);
    }
	
	@PostMapping("/devices/registerSimple")
    public ResponseEntity<DeviceRegistrationResult> registerDeviceSimple(@RequestBody DeviceRegistrationRequest deviceRequest,
            HttpServletRequest request) throws UnknownHostException {
        log.info("registerDeviceSimple DeviceController registerDevice Addr=" + request.getRemoteAddr());
        
        if(!authService.checkHashedKey(deviceRequest.getServerKey())){
            //return ResponseEntity.status(HttpStatus.FORBIDDEN).body(DeviceRegistrationResult.build(DeviceRegistrationStatus.FORBIDDEN));
        }
        
        DeviceRegistrationResult result = deviceService.registerDevice(deviceRequest);
        return ResponseEntity.ok().body(result);
    }
	
	@PostMapping("/devices/register")
	public ResponseEntity<DeviceRegistrationResult> registerDevice(@Valid @RequestBody DeviceRegistrationRequest deviceRequest,
			HttpServletRequest request) throws UnknownHostException {
		log.info("DeviceController registerDevice Addr=" + request.getRemoteAddr());
		
		if(!authService.checkHashedKey(deviceRequest.getServerKey())){
			//return ResponseEntity.status(HttpStatus.FORBIDDEN).body(DeviceRegistrationResult.build(DeviceRegistrationStatus.FORBIDDEN));
		}
		deviceRequest.setIp(request.getRemoteAddr());
		
		DeviceRegistrationResult result = deviceService.registerDevice(deviceRequest);
		return ResponseEntity.ok().body(result);
	}
	
	@PostMapping("/devices/data")
    public ResponseEntity<String> dataChangeDispatchedOnDevice(@RequestBody DeviceOnDataUpdatedRequest request) {
	    log.info("DataChanged dispatched on "+request.getDeviceId()+" hasData "+request.hasData());
	    
        deviceService.setDeviceDataRequestCompleted(request.getDeviceId(), request.getData());
        
        return ResponseEntity.ok().body("OK");
    }

	@PostMapping("/devices/activate{deviceId}")
	public ResponseEntity<Boolean> activateDevice(@PathVariable(value = "deviceId") String deviceId) {
		Boolean result = deviceService.activateDevice(deviceId);
		return ResponseEntity.ok().body(result);
	}

	@GetMapping("/devices/getData_{deviceId}")
	public ResponseEntity<String> executeGetDataOnDevice(@PathVariable(value = "deviceId") String deviceId,
			@RequestParam(value = GROUP, required = false) String devEntGroup) throws UnsupportedEncodingException {

		return deviceService.executeGetDataOnDevice(deviceId, devEntGroup);
	}

	@GetMapping("/devices/mock_{deviceId}")
	public ResponseEntity<String> executeMockGetDataOnDevice(@PathVariable(value = "deviceId") String deviceId,
			@RequestParam(value = GROUP, required = false) String devEntGroup)
			throws UnsupportedEncodingException, JSONException {
		
		String result = ((devEntGroup==null || devEntGroup.isEmpty())?
							ServerValuesMockUtil.getSettingsSensors(deviceId):
							ServerValuesMockUtil.getSensors(deviceId)).toString();
		
		log.info("Mock result =" + result);

		return ResponseEntity.ok().body(result);
	}
}
