package com.balabas.smarthouse.server.model;

import java.util.Set;

import org.json.JSONObject;

import com.fasterxml.jackson.annotation.JsonIgnore;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;


@Data
@NoArgsConstructor
@AllArgsConstructor
public class Device implements SmartHouseItem, JsonDataContainer {
    
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
	
	private String ip;
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
	
	public void setDataUpdateFailed(){
	    timer.setDataUpdateFailed();
	    
	    this.getGroups().stream().forEach(group -> getTimer().setDataUpdateFailed());
	}
	
	public Group getGroup(String name){
	    return getGroups().stream().filter(group->group.getName().equals(name))
	    .findFirst().orElse(null);
	}
	
	public Device updateDevice(Device from){
	    this.deviceDescr = from.deviceDescr;
	    this.deviceFirmware = from.deviceFirmware;
	    this.deviceKey = from.deviceKey;
	    
	    return this;
	}

    @Override
    public String getName() {
        return getDeviceId();
    }
    
    @Override
    public String getDescription() {
        return getDeviceDescr();
    }
}
