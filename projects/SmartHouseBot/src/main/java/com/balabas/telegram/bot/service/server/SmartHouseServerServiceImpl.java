package com.balabas.telegram.bot.service.server;

import java.util.ArrayList;
import java.util.List;

import lombok.extern.log4j.Log4j2;

import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.core.ParameterizedTypeReference;
import org.springframework.http.HttpMethod;
import org.springframework.stereotype.Service;
import org.springframework.web.client.RestTemplate;

import com.balabas.smarthouse.server.DeviceConstants;
import com.balabas.smarthouse.server.model.Device;

@Log4j2
@Service
public class SmartHouseServerServiceImpl implements InitializingBean, SmartHouseServerService {
    
    @Value("${smarthouse.server.base.url}")
    private String serverBaseUrl;
    
    @Value("${smarthouse.server.mock}")
    private boolean mock;
    
    @Override
    public void afterPropertiesSet() throws Exception {
        log.info("Base URL ="+this.serverBaseUrl+" mock="+mock);
    }
    
    public List<Device> getDevices(){
        if(mock){
            return getDevicesMock();
        }
        String url = serverBaseUrl + DeviceConstants.DEVICES_GET_URL;
        
        RestTemplate tmpl = new RestTemplate();
        
        return (tmpl.exchange(
                url,
                HttpMethod.GET,
                null,
                new ParameterizedTypeReference<List<Device>>(){})).getBody();
    }
    
    private List<Device> getDevicesMock(){
        log.info("Get mock devices list");
        List<Device> result = new ArrayList<Device>();
        
        for(int i=0; i<3; i++){
            result.add(new Device("deviceId"+i, "deviceFirmware"+i, "deviceDescr"+i, null, "dataUrl"+i, "rootUrl"+i, true));
        }
        
        return result;
    }

}
