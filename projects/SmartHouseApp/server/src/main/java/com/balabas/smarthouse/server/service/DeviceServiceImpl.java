package com.balabas.smarthouse.server.service;

import java.io.UnsupportedEncodingException;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.stream.Collector;
import java.util.stream.Collectors;

import org.json.JSONObject;
import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Service;
import org.springframework.util.ObjectUtils;

import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.DeviceOnDataUpdatedRequest;
import com.balabas.smarthouse.server.model.DeviceRegistrationRequest;
import com.balabas.smarthouse.server.model.DeviceRegistrationResult;
import com.balabas.smarthouse.server.model.DeviceRegistrationResult.DeviceRegistrationStatus;

import lombok.extern.log4j.Log4j2;
import static com.balabas.smarthouse.server.DeviceConstants.GROUP;

@Log4j2
@Service
public class DeviceServiceImpl implements InitializingBean, DeviceService {

	@Value("${smarthouse.server.mock}")
    private boolean mock;
	
	@Value("${smarthouse.server.devices.request.interval}")
    private long timeToLive;
	
	@Autowired
	HttpRequestExecutor executor;

	@Autowired
	GroupEntityService groupEntityService;
	
	private List<Device> devices;

	public DeviceServiceImpl() {
		this.devices = new ArrayList<>();
	}

	@Override
	public List<Device> getDevices() {
		return this.devices;
	}
	
	@Override
	public void afterPropertiesSet() throws Exception {
		if(this.mock){
			log.warn("Device service filled with mock data");
			this.devices = getDevicesMock();
		}
	}

	@Override
	public Optional<Device> getDeviceByDeviceId(String deviceId) {
		return devices.stream().filter(dev -> (dev.getDeviceId() != null && dev.getDeviceId().equals(deviceId)))
				.findFirst();
	}
	
	@Override
    public Device getDevice(String deviceId) {
	    Optional<Device> devOpt = getDeviceByDeviceId(deviceId);
	    
	    return devOpt.get();
    }

	@Override
	public DeviceRegistrationResult registerDevice(Device device) {
		DeviceRegistrationResult result = new DeviceRegistrationResult();

		if (isDeviceAlreadyRegistered(device)) {
			result.setResult(DeviceRegistrationStatus.ALREADY_REGISTERED);
		}

		if (!isDeviceRegistrationAllowed(device)) {
			result.setResult(DeviceRegistrationStatus.NOT_ALLOWED);
		}

		if (result.isOkOrUnknown()) {
			addDeviceTo(device);
			result.setResult(DeviceRegistrationStatus.SUCCESS);
		}

		log.info("DeviceServiceImpl Registering device " + device.toString() + " status = "
				+ result.getResult().toString());

		return result;
	}

	@Override
	public boolean activateDevice(String deviceId) {
		Optional<Device> deviceOpt = getDeviceByDeviceId(deviceId);

		deviceOpt.ifPresent(device -> device.setActive(true));

		return deviceOpt.isPresent();
	}

	@Override
	public ResponseEntity<String> executeGetDataOnDevice(String deviceId, String deviceEntityGroup) throws UnsupportedEncodingException {
		Optional<Device> deviceOpt = getDeviceByDeviceId(deviceId);

		if (deviceOpt.isPresent()) {
			Device device = deviceOpt.get();

			if (device.getDataUrl() != null) {
				Map<String, String> params = (deviceEntityGroup != null && !deviceEntityGroup.isEmpty())
						? Collections.singletonMap(GROUP, deviceEntityGroup) : null;

					ResponseEntity<String> result = executor.executeGetRequest(device.getDataUrl(), params);
					
					if(result.getStatusCodeValue()==HttpStatus.OK.value()){
					    setDeviceDataRequestCompleted(device, result.getBody());
					}
			}else{
				log.error("Empty data url deviceId=" +deviceId);
			}
		}

		return null;
	}

	private boolean isDeviceAlreadyRegistered(Device device) {
		return devices.stream().anyMatch(dev -> dev.equals(device));
	}

	private boolean isDeviceRegistrationAllowed(Device device) {
		return device != null;
	}

	private void addDeviceTo(Device device) {
		log.info("Device added :" + device.toString());
		devices.add(device);
	}
	
	private List<Device> getDevicesMock(){
        log.info("Get mock devices list");
        List<Device> result = new ArrayList<>();
        
        for(int i=0; i<3; i++){
            //result.add(new Device("deviceId"+i, "deviceFirmware"+i, "deviceDescr"+i, null, "http://localhost:8080/api/v1/devices/mock_deviceId"+i, "rootUrl"+i, true));
        }
        
        return result;
    }

    @Override
    public DeviceRegistrationResult registerDevice(
            DeviceRegistrationRequest request) throws UnknownHostException {
        Device device = Device.from(request);
        return registerDevice(device);
    }

    @Override
    public void markDeviceAsWaitsForRequest(DeviceOnDataUpdatedRequest request) {
        Device device = getDevice(request.getDeviceId());
        
        device.setWaitsForDeviceRequest(true);
    }

    @Override
    public void setDeviceDataRequestCompleted(Device device, String data) {
        if(data!=null && !data.isEmpty()){
            device.setDeviceDataRequestCompleted(data);
            JSONObject json = device.getLastData();
            
            groupEntityService.parseJsonToModel(device, json);
        }else{
            device.setWaitsForDeviceRequest(true);
            log.info("device "+device.getDeviceId()+" marked to be updated");
        }
    }

    @Override
    public void setDeviceDataRequestCompleted(String deviceId, String data) {
        Device device = getDevice(deviceId);
        
        setDeviceDataRequestCompleted(device, data);
        
    }

    @Override
    @Scheduled(fixedRate = 2000)
    public void processDevicesDataChangedDispatched() {
        long now = (new Date()).getTime();
        long timeBorder = now - timeToLive;
        
        List<Device> devicesChanged = getDevices().stream()
        .filter(device->(device.isTimeToUpdate(timeBorder))).collect(Collectors.toList());
        
        requestDevicesValues(devicesChanged);
    }

    @Override
    public void requestDevicesValues(List<Device> devices) {
        if(devices.size()>0){
            log.info(devices.size()+" to be updated");
            
            devices.forEach(device->{
                try {
                    executeGetDataOnDevice(device.getDeviceId(),null);
                } catch (Exception e) {
                    e.printStackTrace();
                }
                }
            );

        }
    }
}
