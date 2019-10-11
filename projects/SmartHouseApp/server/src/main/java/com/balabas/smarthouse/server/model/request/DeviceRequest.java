package com.balabas.smarthouse.server.model.request;

import org.springframework.http.HttpHeaders;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.google.common.collect.Lists;

import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;
import lombok.ToString;

import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_FIELD_DATA;

import java.util.List;

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
	
	@JsonIgnore
	private HttpHeaders headers;
	
	@JsonIgnore
	private DeviceRequestType requestType;
	
	public boolean hasData(){
        return (data!=null && !data.isEmpty());
    }
	
	public String getData(){
		if(headers!=null && headers.containsKey(DEVICE_FIELD_DATA)){
			List<String> dataHead = headers.get(DEVICE_FIELD_DATA);
			if(!dataHead.isEmpty()){
				return dataHead.get(0);
			}
		}
	    if(hasData()){
	        return this.data;
	    }
	    return null;
	}
	
	public boolean isValid() {
		return Lists.newArrayList(DeviceRequestType.values())
		.stream().anyMatch(t->t.equals(getRequestType()));
	}
}
