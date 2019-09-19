package com.balabas.telegram.bot.service.server;

import java.io.UnsupportedEncodingException;
import java.util.List;

import org.json.JSONException;
import org.json.JSONObject;

import com.balabas.smarthouse.server.model.Device;

public interface SmartHouseServerService {
    
    List<Device> getDevices() throws UnsupportedEncodingException;
    
    Device getDevice(String deviceId) throws UnsupportedEncodingException;
    
    JSONObject getDataOnDevice(String deviceId, String groupId) throws UnsupportedEncodingException, JSONException;

}
