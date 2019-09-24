package com.balabas.smarthouse.server.model.request;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class DeviceRequest {

	private String deviceId;
	
	private String deviceFirmware;
	
	private String deviceDescr;
	
	private String serverKey;
	
	private String deviceKey;
	
	private String rootUrl;
	
	private String dataUrl;
	
	private String ip;
	
	private String data;
	
	public boolean hasData(){
        return data!=null && !data.isEmpty();
    }
}