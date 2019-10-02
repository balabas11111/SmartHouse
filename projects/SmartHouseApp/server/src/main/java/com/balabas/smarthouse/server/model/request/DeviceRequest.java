package com.balabas.smarthouse.server.model.request;

import org.springframework.http.HttpHeaders;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.node.JsonNodeType;
import com.google.common.collect.Lists;

import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;
import lombok.ToString;

@Data
@AllArgsConstructor
@NoArgsConstructor
@Builder
@ToString
public class DeviceRequest {

	public enum DeviceRequestType{
		UNKNOWN,
		CHECK_ONLINE,
		REGISTER,
		DATA_UPDATE_EVENT
	}
	
	private String deviceId;
	
	private String deviceFirmware;
	
	private String deviceDescr;
	
	private String deviceKey;
	
	private String ip;
	
	private String data;
	
	private JsonNode dataJson;
	
	@JsonIgnore
	private HttpHeaders headers;
	
	@JsonIgnore
	private DeviceRequestType requestType;
	
	public boolean hasData(){
        return (data!=null && !data.isEmpty());
    }
	
	public boolean hasDataJson(){
        return (dataJson!=null && JsonNodeType.OBJECT.equals(dataJson.getNodeType()));
    }
	
	public String getJsonOrData(){
	    if(hasDataJson()){
	        return dataJson.toString();
	    }
	    if(hasData()){
	        return data;
	    }
	    return null;
	}
	
	public boolean isValid() {
		return Lists.newArrayList(DeviceRequestType.values())
		.stream().anyMatch(t->t.equals(getRequestType()));
	}
}
