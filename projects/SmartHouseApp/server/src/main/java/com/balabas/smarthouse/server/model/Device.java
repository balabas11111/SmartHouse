package com.balabas.smarthouse.server.model;

import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.Date;
import java.util.Set;

import org.json.JSONException;
import org.json.JSONObject;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
import lombok.extern.log4j.Log4j2;

@Log4j2
@Data
@AllArgsConstructor
@NoArgsConstructor
public class Device {
    
    public enum DeviceStatus{
        UNKNOWN,
        CONSTRUCTED,
        CONNECTED
    };

	private String deviceId;
	private String deviceFirmware;
	private String deviceDescr;
	
	private InetAddress ip;
	private String dataUrl;
	private String rootUrl;
	
	private DeviceStatus status = DeviceStatus.UNKNOWN;
	
	private Date registrationTime;
	
	private Date lastRequestTime;
	
	private boolean active;
	
	private boolean waitsForDeviceRequest;
	
	private JSONObject lastData;
	
	private Set<Group> groups;
	
	public boolean isTimeToUpdate(long timeBorder){
	    if(this.lastRequestTime == null){
	        return this.waitsForDeviceRequest;
	    }
	    long lastTime = this.lastRequestTime.getTime();
	    long now = (new Date()).getTime();
	    
	    boolean result = lastTime  < timeBorder;
        setWaitsForDeviceRequest(result || this.waitsForDeviceRequest);
        
        if(this.waitsForDeviceRequest){
            log.info("Time to update entity");
        }
        
        return this.waitsForDeviceRequest;
	}
	
	public void setDeviceDataRequestCompleted(String jsonStr) throws JSONException{
	    this.waitsForDeviceRequest = false;
	    this.active = true;
	    this.lastRequestTime = new Date();
	    
	    this.lastData = new JSONObject(jsonStr); 
	    
	    log.info(deviceId+" data update completed data = "+jsonStr);
	}
	
	public static Device from(DeviceRegistrationRequest request) throws UnknownHostException{
	    Device device = new Device();
	    
	    device.setDeviceId(request.getDeviceId());
	    device.setDeviceFirmware(request.getDeviceFirmware());
	    device.setDeviceDescr(request.getDeviceDescr());
	    
	    device.setIp(InetAddress.getByName(request.getIp()));
	    
	    device.setDataUrl(request.getDataUrl());
	    device.setRootUrl(request.getRootUrl());
	    device.setRegistrationTime(new Date());
	    device.setStatus(DeviceStatus.CONSTRUCTED);
	    
	    device.setWaitsForDeviceRequest(true);
	    
	    return device;
	}
}
