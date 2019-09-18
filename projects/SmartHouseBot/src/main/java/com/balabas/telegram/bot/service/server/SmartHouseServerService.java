package com.balabas.telegram.bot.service.server;

import java.util.List;

import com.balabas.smarthouse.server.model.Device;

public interface SmartHouseServerService {
    
    List<Device> getDevices();

}
