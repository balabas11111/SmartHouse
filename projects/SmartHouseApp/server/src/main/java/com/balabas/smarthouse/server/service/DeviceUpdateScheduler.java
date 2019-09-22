package com.balabas.smarthouse.server.service;


import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Service;

@Service
public class DeviceUpdateScheduler {

    @Autowired
    DeviceService deviceService;
    
    @Scheduled(fixedRate = 2000)
    public void selectDevicesWithTimedOutData() {
        deviceService.requestAllDevicesData();
    }
}
