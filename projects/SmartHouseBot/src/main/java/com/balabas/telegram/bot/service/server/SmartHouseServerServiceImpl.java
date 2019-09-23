package com.balabas.telegram.bot.service.server;

import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;

import lombok.extern.log4j.Log4j2;

import org.json.JSONException;
import org.json.JSONObject;
import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.DeviceConstants;
import com.balabas.smarthouse.server.model.Device;

import static com.balabas.smarthouse.server.DeviceConstants.GROUP;

@Log4j2
@Service
public class SmartHouseServerServiceImpl implements InitializingBean, SmartHouseServerService {
    
    @Value("${smarthouse.server.base.url}")
    private String serverBaseUrl;
    
    @Value("${smarthouse.server.mock}")
    private boolean mock;
    
    @Autowired
	HttpRequestExecutor executor;
    
    @Override
    public void afterPropertiesSet() throws Exception {
        log.info("Base URL ="+this.serverBaseUrl+" mock="+mock);
    }
    
    public List<Device> getDevices() throws UnsupportedEncodingException{
        if(mock){
            return getDevicesMock();
        }
        String url = serverBaseUrl + DeviceConstants.DEVICES_GET_URL;
        
        return executor.executeGetRequestDeviceList(url, null).getBody();
    }
    
    private List<Device> getDevicesMock(){
        log.info("Get mock devices list");
        List<Device> result = new ArrayList<>();
        
        for(int i=0; i<3; i++){
            //result.add(new Device("deviceId"+i, "deviceFirmware"+i, "deviceDescr"+i, null, "dataUrl"+i, "rootUrl"+i, true));
        }
        
        return result;
    }

	@Override
	public Device getDevice(String deviceId) throws UnsupportedEncodingException {
		String url = serverBaseUrl + DeviceConstants.DEVICES_GET_DEVICE_URL.replace(DeviceConstants.DEVICE_ID_PARAM, deviceId);
		return executor.executeGetRequestDevice(url, null).getBody();
	}

	@Override
	public JSONObject getDataOnDevice(String deviceId, String groupId) throws UnsupportedEncodingException, JSONException {
		String url = serverBaseUrl + DeviceConstants.DEVICES_GET_DATA_DEVICE_URL.replace(DeviceConstants.DEVICE_ID_PARAM, deviceId);
		
		Map<String, String> params = (groupId != null && !groupId.isEmpty())
				? Collections.singletonMap(GROUP, groupId) : null;
		
		String response = executor.executeGetRequest(url, params).getBody();
		
		log.info("response="+response);
		
		return new JSONObject(response);
	}

}
