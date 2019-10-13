package com.balabas.smarthouse.server.service;

import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_ID;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_IP;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_DATA;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_ROOT;

import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_DEVICE_DEVICE_REGISTRATION_RESULT;
import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_FIELD_TOKEN;
import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_FIELD_URL_DATA;
import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_FIELD_URL_ROOT;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_STATUS;

import static com.balabas.smarthouse.server.DeviceConstants.HTTP_PREFFIX;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_DEVICE_DEVICE_KEY;

import org.eclipse.paho.client.mqttv3.MqttException;
import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.model.request.DeviceRequest;
import com.balabas.smarthouse.server.model.request.DeviceRequest.DeviceRequestType;
import com.balabas.smarthouse.server.mqtt.MqttService;
import com.balabas.smarthouse.server.mqtt.subscribers.DataDeviceSubscriber;
import com.balabas.smarthouse.server.security.DeviceSecurityContext;
import com.balabas.smarthouse.server.security.DeviceSecurityService;

import lombok.extern.log4j.Log4j2;

@Log4j2
@Service
public class DeviceMessageService implements IDeviceMessageService {

	@Value("${smarthouse.server.name}")
	private String serverName;

	@Autowired
	private DeviceService deviceService;

	@Autowired
	private DeviceSecurityService securityService;

	@Autowired
	private MqttService mqttService;

	@Override
	public boolean registerDevice(String message) {
		try {
			log.info("processRegistrationRequest " + message);
			JSONObject requestJson = new JSONObject(message);
			
			if (requestJson.has(ENTITY_FIELD_ID) && requestJson.has(ENTITY_FIELD_IP)) {
				
				String deviceId = requestJson.getString(ENTITY_FIELD_ID);
				String ip = requestJson.getString(ENTITY_FIELD_IP);
				String data = "";
				String deviceKey = requestJson.optString(ENTITY_DEVICE_DEVICE_KEY, "0");

					if(requestJson.has(ENTITY_FIELD_ROOT) && requestJson.has(ENTITY_FIELD_DATA)) {
						JSONObject dataJson = new JSONObject();
						
				    	if(requestJson.has(ENTITY_FIELD_ROOT)){
				    		String tmp = requestJson.getString(ENTITY_FIELD_ROOT);
				    		
				    		if(tmp!=null && !tmp.isEmpty() && !tmp.startsWith(HTTP_PREFFIX)) {
				    			tmp = HTTP_PREFFIX + ip + tmp;
				    			dataJson.put(DEVICE_FIELD_URL_ROOT, tmp);
				    		}
				    	}
				    	if(requestJson.has(ENTITY_FIELD_DATA)){
				    		String tmp = requestJson.getString(ENTITY_FIELD_DATA);
				    		
				    		if(tmp!=null && !tmp.isEmpty() && !tmp.startsWith(HTTP_PREFFIX)) {
				    			tmp = HTTP_PREFFIX + ip + tmp;
				    			dataJson.put(DEVICE_FIELD_URL_DATA, tmp);
				    		}
				    	}
				    	
				    	data = dataJson.toString();
					}
					/*				
					DeviceRequest request = DeviceRequest.builder().deviceId(deviceId)
							.deviceKey(deviceKey)
							.ip(ip)
							.data(data)
							.requestType(DeviceRequestType.REGISTER).build();

					String token = securityService.processDeviceRegistrationRequest(request);
					deviceService.registerDevice(request).getResult().toString();
*/
					sendRegisteredToDevice(deviceId, "1");

					return true;
			}

		} catch (Throwable t) {
			log.error(t);
		}
		return false;
	}

	@Override
	public boolean processData(String topicName, String message) {
		log.info("Data from topic " + topicName + " message: " + message);
		return true;
	}
	
	@Override
	public boolean sendRegisteredToDevice(String deviceId) throws MqttException {
		return sendRegisteredToDevice(deviceId, "0");
	}
	
	@Override
	public boolean sendRegisteredToDevice(String deviceId, String token) throws MqttException {
		String fromDeviceTopic = mqttService.getFromDeviceTopic(deviceId);
		
		String toDeviceTopic = mqttService.getToDeviceTopic(deviceId);
		String response = constructRegisterResponse(deviceId).toString();

		log.info("Device registered token = " + token  +
				" toDeviceTopic = " + toDeviceTopic+ " fromDeviceTopic = "
				+ fromDeviceTopic);
		
		subscribeToDevice(deviceId, fromDeviceTopic);
		
		mqttService.publish(toDeviceTopic, response);
		
		return true;
	}

	protected void subscribeToDevice(String deviceId, String toDeviceTopic) {
		mqttService.registerSubscriber(new DataDeviceSubscriber(toDeviceTopic, this));
	}

	protected JSONObject constructRegisterResponse(String deviceId) {
		JSONObject result = new JSONObject().put(ENTITY_DEVICE_DEVICE_REGISTRATION_RESULT, new JSONObject())
				.getJSONObject(ENTITY_DEVICE_DEVICE_REGISTRATION_RESULT);
		DeviceSecurityContext context = securityService.getDeviceSecurityContext(deviceId);

		if (context != null) {
			result.put(DEVICE_FIELD_TOKEN, context.getServerToken());
			result.put(ENTITY_FIELD_STATUS, "200");
		} else {
			result.put(ENTITY_FIELD_STATUS, "500");
		}

		return result;
	}

}
