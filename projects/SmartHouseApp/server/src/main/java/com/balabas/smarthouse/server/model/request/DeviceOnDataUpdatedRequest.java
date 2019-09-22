package com.balabas.smarthouse.server.model.request;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class DeviceOnDataUpdatedRequest {

    private String deviceId;
    
    private String data;
    
    public boolean hasData(){
        return data!=null && !data.isEmpty();
    }
}
