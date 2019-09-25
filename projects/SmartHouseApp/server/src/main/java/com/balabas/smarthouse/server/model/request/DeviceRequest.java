package com.balabas.smarthouse.server.model.request;

import org.json.JSONObject;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.node.JsonNodeType;

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
	
	private JsonNode json;
	
	public boolean hasData(){
        return (data!=null && !data.isEmpty());
    }
	
	public boolean hasJson(){
        return (json!=null && JsonNodeType.OBJECT.equals(json.getNodeType()));
    }
	
	public String getJsonOrData(){
	    if(hasJson()){
	        return json.toString();
	    }
	    if(hasData()){
	        return data;
	    }
	    return null;
	}
}
