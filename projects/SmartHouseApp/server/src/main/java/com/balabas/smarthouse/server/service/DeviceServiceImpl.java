package com.balabas.smarthouse.server.service;

import java.io.UnsupportedEncodingException;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Optional;

import org.json.JSONException;
import org.json.JSONObject;
import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.events.ChangedEvent;
import com.balabas.smarthouse.server.exception.ResourceNotFoundException;
import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.Device.DeviceState;
import com.balabas.smarthouse.server.model.Group;
import com.balabas.smarthouse.server.model.request.DeviceOnDataUpdatedRequest;
import com.balabas.smarthouse.server.model.request.DeviceRegistrationRequest;
import com.balabas.smarthouse.server.model.request.DeviceRegistrationResult;
import com.balabas.smarthouse.server.model.request.DeviceRegistrationResult.DeviceRegistrationStatus;

import lombok.extern.log4j.Log4j2;
import static com.balabas.smarthouse.server.DeviceConstants.GROUP;

@Log4j2
@Service
public class DeviceServiceImpl implements InitializingBean, DeviceService {

	@Value("${smarthouse.server.mock}")
    private boolean mock;
	
	@Value("${smarthouse.server.devices.request.interval}")
    private long deviceUpdateInterval;
	
	@Autowired
	HttpRequestExecutor executor;

	@Autowired
	GroupEntityUpdateService groupEntityUpdateService;
	
	@Autowired
	DataUpdateEventService dataUpdateEventService;
	
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
    public Device getDevice(String deviceId) throws ResourceNotFoundException {
	    Optional<Device> devOpt = getDeviceByDeviceId(deviceId);
	    
	    return devOpt.orElseThrow(()->new ResourceNotFoundException(deviceId));
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
			device.setState(DeviceState.REGISTERED);
		}

		log.info(device.getDeviceId()+ " REGISTERED " + device.toString() + " status = "
				+ result.getResult().toString());

		return result;
	}

	@Override
	public String executeGetDataOnDevice(String deviceId, String deviceEntityGroup) throws UnsupportedEncodingException {
		Optional<Device> deviceOpt = getDeviceByDeviceId(deviceId);

		if (deviceOpt.isPresent()) {
			Device device = deviceOpt.get();

			if (device.getDataUrl() != null) {
				Map<String, String> params = (deviceEntityGroup != null && !deviceEntityGroup.isEmpty())
						? Collections.singletonMap(GROUP, deviceEntityGroup) : null;

					ResponseEntity<String> result = executor.executeGetRequest(device.getDataUrl(), params);
					
					if(result.getStatusCodeValue()==HttpStatus.OK.value()){
					    try {
                            processDeviceDataUpdate(device, result.getBody());
                        } catch (ResourceNotFoundException e) {
                            log.error(e.getMessage());
                        }
					}
			}else{
				log.error("Empty data url deviceId=" +deviceId);
			}
		}

		return null;
	}
	
	@Override
    public JSONObject executeGetData(String deviceId,
            String deviceEntityGroup) throws ResourceNotFoundException {
        Device device = getDevice(deviceId);
        
        JSONObject result = new JSONObject();
        
        if(deviceEntityGroup==null){
           result = device.getData(); 
        }
        
        return result;
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
        Device device = Device.from(request, deviceUpdateInterval);
        return registerDevice(device);
    }

    @Override
    public void markDeviceAsWaitsForDataUpdate(DeviceOnDataUpdatedRequest request) throws ResourceNotFoundException {
        Device device = getDevice(request.getDeviceId());
        
        device.getTimer().setWaitsForDataUpdate(true);
    }

    @Override
    public void processDeviceDataUpdateDispatched(DeviceOnDataUpdatedRequest request) throws ResourceNotFoundException {
        processDeviceDataUpdateDispatched(request.getDeviceId(), request.getData());
    }
        
    @Override
    public void processDeviceDataUpdateDispatched(String deviceId, String deviceData) throws ResourceNotFoundException{
        Optional<Device> device = getDeviceByDeviceId(deviceId);
        
        if(device == null){
            log.error("DeviceId "+deviceId+" is not registered");
            throw new ResourceNotFoundException(Device.class, deviceId);
        }
        
        processDeviceDataUpdate(device.get(), deviceData);
    }
    
    @Override
    public void processDeviceDataUpdate(Device device, String deviceData) throws ResourceNotFoundException{
        
        if(device == null){
            log.error("Device is not registered");
            throw new ResourceNotFoundException(Device.class, "");
        }
        
        if(validateDeviceData(deviceData)){
            List<ChangedEvent<?>> events = groupEntityUpdateService.parseJsonToModel(device, new JSONObject(deviceData));
            dataUpdateEventService.processEvents(events);
        }else{
            device.getTimer().setWaitsForDataUpdate(true);
        }
    }
    
    @Override
    public boolean validateDeviceData(String data){
        boolean result = data!=null && !data.isEmpty();
        
        try{
            new JSONObject(data);
        }catch(JSONException | NullPointerException ex){
           result = false; 
        }
        
        return result;
        
    }

    @Override
    public void requestAllDevicesData() {
        if(getDevices().size()>0){
            
            Map<String,String> deviceIds = new HashMap<>();
            
            getDevices().stream()
                .filter(dev->dev.isRegistered() && dev.getTimer().isTimeToUpdate())
                .forEach(device->{
                        deviceIds.put(device.getDeviceId(), null);
                        requestDevicesValues(device);
                    });
            
            getDevices().stream()
            .filter(device->device.isRegistered() && device.getGroups()!=null 
                        && !deviceIds.containsKey(device.getDeviceId()))
            .forEach(device->{
                device.getGroups().stream()
                        .filter(group->(group.getTimer().isTimeToUpdate()))
                        .forEach(group->requestGroupValues(device,group));
            });
        }
    }
    
    @Override
    public void requestDevicesValues(Device device) {
        try {
            executeGetDataOnDevice(device.getDeviceId(),null);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    @Override
    public void requestGroupValues(Device device, Group group) {
        if(device!=null && group!=null){
            try {
                executeGetDataOnDevice(device.getDeviceId(), group.getName());
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
    
}
