package com.balabas.smarthouse.server.service;


import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Service;
import org.springframework.web.client.RestTemplate;

import com.balabas.smarthouse.server.model.Device;

@Service
public class DeviceUpdateScheduler {

    @Autowired
    DeviceService deviceService;
    
    @Scheduled(fixedRate = 2000)
    public void selectDevicesWithTimedOutData() {
        deviceService.requestAllDevicesData();
    }
    
    @Scheduled(fixedRate = 10000)
    public void mockDeviceDataUpdateRequest() {
        if(deviceService.isMock()){
        	for(Device device: deviceService.getDevices()){
	        	RestTemplate restTemplate = new RestTemplate();
	    		String execUrl = "http://localhost/api/v1/devices/data?deviceId="+device.getDeviceId();
	    		
	    		restTemplate.getForEntity(execUrl, String.class);
        	}
        }
    }
}
