package com.balabas.smarthouse.server.model;

import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.Set;

import org.json.JSONObject;

import com.balabas.smarthouse.server.model.request.DeviceRequest;
import com.fasterxml.jackson.annotation.JsonIgnore;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;


@Data
@NoArgsConstructor
@AllArgsConstructor
public class Device implements NameAble, JsonDataContainer {
    
    public enum DeviceState{
        UNKNOWN,
        CONSTRUCTED,
        REGISTERED,
        CONNECTED,
        TIMED_OUT,
        WAITS_FOR_DEVICE_RESPONSE
    };

	private String deviceId;
	private String deviceFirmware;
	private String deviceDescr;
	
	private InetAddress ip;
	private String dataUrl;
	private String rootUrl;
	
	@JsonIgnore
	private String deviceKey;
	
	private DeviceState state = DeviceState.UNKNOWN;
	
	private JSONObject data;
	
	private Set<Group> groups;
	
	private UpdateTimer timer;
	
	public Device(String deviceId, long updateInterval){
	    this.deviceId = deviceId;
	    timer = new UpdateTimer(this, updateInterval);
	}
	
	public boolean isRegistered(){
	    return this.state!=DeviceState.UNKNOWN
	                && this.state!=DeviceState.CONSTRUCTED;
	}
	
	public boolean isInitialDataReceived(){
        return isRegistered() && this.state!=DeviceState.REGISTERED;
    }
	
	public void setDataUpdateCompleted(){
	    this.state=DeviceState.CONNECTED;
	    
	    timer.setDataReceived();
	}
	
	public static Device from(DeviceRequest request, long updateInterval) throws UnknownHostException{
	    Device device = new Device(request.getDeviceId(), updateInterval);
	    
	    device.setDeviceFirmware(request.getDeviceFirmware());
	    device.setDeviceDescr(request.getDeviceDescr());
	    
	    device.setIp(InetAddress.getByName(request.getIp()));
	    
	    device.setDataUrl(request.getDataUrl());
	    device.setRootUrl(request.getRootUrl());
	    device.setState(DeviceState.CONSTRUCTED);
	    
	    device.setDeviceKey(request.getDeviceKey());
	    
	    device.getTimer().setWaitsForDataUpdate(true);
	    
	    return device;
	}

    @Override
    public String getName() {
        return getDeviceId();
    }
}
