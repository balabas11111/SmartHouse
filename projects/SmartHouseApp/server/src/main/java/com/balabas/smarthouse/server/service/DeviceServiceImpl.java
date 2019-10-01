package com.balabas.smarthouse.server.service;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.stream.Collectors;

import org.json.JSONException;
import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.controller.service.DeviceRequestorService;
import com.balabas.smarthouse.server.events.ChangedEvent;
import com.balabas.smarthouse.server.exception.ResourceNotFoundException;
import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.Device.DeviceState;
import com.balabas.smarthouse.server.model.Group;
import com.balabas.smarthouse.server.model.request.DeviceRequest;
import com.balabas.smarthouse.server.model.request.DeviceRegistrationResult;
import com.balabas.smarthouse.server.model.request.DeviceRegistrationResult.DeviceRegistrationStatus;
import com.balabas.smarthouse.server.security.DeviceSecurityService;
import com.balabas.smarthouse.server.service.events.EventProcessorsService;
import com.balabas.smarthouse.server.view.Action;

import lombok.extern.log4j.Log4j2;


import static com.balabas.smarthouse.server.DeviceConstants.HTTP_PREFFIX;
import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_URL_ROOT;
import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_URL_DATA;

@Log4j2
@Service
public class DeviceServiceImpl implements DeviceService {

	@Value("${smarthouse.server.devices.request.interval}")
    private long deviceUpdateInterval;
	
	@Autowired
	DeviceSecurityService authService;
	
	@Autowired
	GroupEntityUpdateService groupEntityUpdateService;
	
	@Autowired
	EventProcessorsService eventProcessService;
	
	@Autowired
	DeviceRequestorService deviceRequestor;
	
	private List<Device> devices;

	public DeviceServiceImpl() {
		this.devices = new ArrayList<>();
	}

	@Override
	public List<Device> getDevices() {
		return this.devices;
	}
	@Override
	public List<Device> getDevicesInitialized(){
		return this.devices.stream().filter(Device::isInitialDataReceived).collect(Collectors.toList());
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
    public DeviceRegistrationResult registerDevice(DeviceRequest request) {
    	Device device = new Device(request.getDeviceId(), this.deviceUpdateInterval);
	    
	    device.setDeviceFirmware(request.getDeviceFirmware());
	    device.setDeviceDescr(request.getDeviceDescr());
	    
	    device.setIp(request.getIp());
	    
	    String baseUrl = HTTP_PREFFIX + request.getIp();
	    
	    device.setDataUrl(baseUrl + DEVICE_URL_DATA);
	    device.setRootUrl(baseUrl + DEVICE_URL_ROOT);
	    device.setState(DeviceState.CONSTRUCTED);
	    
	    device.setDeviceKey(request.getDeviceKey());
	    
	    device.getTimer().setWaitsForDataUpdate(true);
        return registerDevice(device);
    }
	
	@Override
	public DeviceRegistrationResult registerDevice(Device device) {
		DeviceRegistrationResult result = new DeviceRegistrationResult();

		if (!isDeviceRegistrationAllowed(device)) {
			result.setResult(DeviceRegistrationStatus.NOT_ALLOWED);
		}
		
		Optional<Device> regDevice = getDeviceByDeviceId(device.getDeviceId());
		
		if (result.isOkOrUnknown()) {
		    if(regDevice.isPresent()){
		        device = regDevice.get().updateDevice(device);
		        
		        result.setResult(DeviceRegistrationStatus.ALREADY_REGISTERED);
		        log.info("Device ReRegistered :" + device.toString());
		    }else{
		    		log.info("Device added :" + device.toString());
		    		devices.add(device);
		    		result.setResult(DeviceRegistrationStatus.SUCCESS);
		    }
		    device.setState(DeviceState.REGISTERED);
		}

		log.debug(device.getDeviceId()+ " REGISTERED " + device.toString() + " status = "
				+ result.getResult().toString());

		return result;
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

	private boolean isDeviceRegistrationAllowed(Device device) {
		return device != null;
	}

    @Override
    public void markDeviceAsWaitsForDataUpdate(DeviceRequest request) throws ResourceNotFoundException {
        Device device = getDevice(request.getDeviceId());
        
        device.getTimer().setWaitsForDataUpdate(true);
    }

    @Override
    public void processDeviceDataUpdateDispatched(DeviceRequest request) throws ResourceNotFoundException {
    	Device device = getDeviceByDeviceId(request.getDeviceId()).orElse(null);
        
        String deviceData = request.getJsonOrData();
        processDataReceivedFromDevice(device, deviceData);
    }
    
    @Override
    public void processDataReceivedFromDevice(Device device, String deviceData) throws ResourceNotFoundException{
        
        if(device == null){
            log.error("Device is not registered");
            throw new ResourceNotFoundException(Device.class, "");
        }
        
        if(validateDeviceData(deviceData)){
            List<ChangedEvent<?>> events = groupEntityUpdateService.parseJsonToModel(device, new JSONObject(deviceData));
            eventProcessService.processEvents(events);
        }else{
            device.getTimer().setWaitsForDataUpdate(true);
        }
    }
    
    private boolean validateDeviceData(String data){
        boolean result = data!=null && !data.isEmpty();
        
        try{
            new JSONObject(data);
        }catch(JSONException | NullPointerException ex){
           result = false; 
        }
        
        return result;
        
    }

    @Override
    public void requestAllDevicesDataWithUpdateRequired() {
        if(!getDevices().isEmpty()){
            
            Map<String,String> deviceIds = new HashMap<>();
            
            getDevices().stream()
                .filter(dev->dev.isRegistered() && dev.getTimer().isTimeToUpdate())
                .forEach(device->{
                        deviceIds.put(device.getDeviceId(), null);
                        requestDevicesValues(device, null);
                    });
            
            getDevices().stream()
            .filter(device->device.isRegistered() && device.getGroups()!=null 
                        && !deviceIds.containsKey(device.getDeviceId()))
            .forEach(device->
                device.getGroups().stream()
                        .filter(group->(group.getTimer().isTimeToUpdate()))
                        .forEach(group->requestDevicesValues(device,group))
            );
        }
    }
    
    @Override
    public void requestDevicesValues(Device device, Group group) {
        try {
            String groupId = group!=null?group.getName():null;
            
            String result = deviceRequestor.executeGetDataOnDevice(device, groupId);
            processDataReceivedFromDevice(device, result);
        } catch (Exception e) {
        	log.error("Error request device values",e);
        }
    }
    
    @Override
    public void processDeviceAction(Action action)  throws Throwable{
    	log.debug("Action received :"+action.getCallbackData());
    	Map<String,Object> params = (new JSONObject(action.getData())).toMap(); 
    	
		sendDataToDevice(action.getDeviceId(), action.getGroupId(), action.getEntityId(), params);
    }

	@Override
	public String sendDataToDevice(String deviceId, String groupId, String entityId, Map<String, Object> values)
			throws ResourceNotFoundException {
		
		JSONObject json = new JSONObject();
		json.put(groupId, new JSONObject().put(entityId, values));
		
		Optional<Device> device = getDeviceByDeviceId(deviceId);
		
		if(!device.isPresent()) {
			throw new ResourceNotFoundException(deviceId);
		}
		
		String result = deviceRequestor.executePostDataOnDevice(device.get(), json);
		processDataReceivedFromDevice(device.get(), result);
		
		return result;
		
	}
    
}
