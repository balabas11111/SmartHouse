package com.balabas.smarthouse.server.entity.service;

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

import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.mqtt.IMessageService;
import com.balabas.smarthouse.server.mqtt.subscribers.DataDeviceSubscribtion;
import com.balabas.smarthouse.server.mqtt.subscribers.DataEntitySubscribtion;
import com.balabas.smarthouse.server.security.DeviceSecurityContext;
import com.balabas.smarthouse.server.security.DeviceSecurityService;

import lombok.extern.log4j.Log4j2;

@Log4j2
@Service
public class DeviceMqService implements IDeviceMqService {

	@Value("${smarthouse.server.name}")
	private String serverName;

	@Autowired
	private DeviceSecurityService securityService;

	@Autowired
	private IMessageService messageService;
	
	@Autowired
	private IDeviceManageService deviceService;

	@Override
	public boolean onRegistrationTopicMessageReceived(String message) {
		try {
			log.info("register Device MQ " + message);
			JSONObject requestJson = new JSONObject(message);
			
			if (requestJson.has(ENTITY_FIELD_ID) && requestJson.has(ENTITY_FIELD_IP)) {
				
				String deviceId = requestJson.getString(ENTITY_FIELD_ID);
				String ip = requestJson.getString(ENTITY_FIELD_IP);

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
				    	
					}

					initTopicsToFromDevice(deviceId);

					return true;
			}

		} catch (Throwable t) {
			log.error(t);
		}
		return false;
	}
	
	@Override
	public void initTopicsToFromDevice(String deviceId) {
		String response = constructRegisterResponse(deviceId).toString();
		
		subscribeFromDeviceTopic(deviceId);
		publishToDeviceTopic(deviceId, response);
	}
	
	@Override
	public void subscribeFromDeviceTopic(String deviceId) {
		String topicName = messageService.getFromDeviceTopicId(deviceId);
		
		messageService.registerSubscriberOrResubscribeExisting(new DataDeviceSubscribtion(topicName, this));
		
		deviceService.getDeviceByName(deviceId).getEntities().stream()
			.forEach(entity -> subscribeFromDeviceEntityTopic(entity));
	}
	
	@Override
	public boolean publishToDeviceTopic(String deviceId, String message) {
		String topicName = messageService.getToDeviceTopicId(deviceId);
		return messageService.publishToTopic(topicName, message);
	}
	
	public void initTopicsToFromDeviceEntity(Entity entity) {
		subscribeFromDeviceEntityTopic(entity);
		
		publishToDeviceTopic(entity.getGroup().getDevice().getName(), 
				new JSONObject()
				.put(ENTITY_FIELD_ID, entity.getRemoteId())
				.put(ENTITY_FIELD_STATUS, 200).toString());
	}
	
	public void subscribeFromDeviceEntityTopic(Entity entity) {
		String topicName = messageService.getFromDeviceEntityTopicId(entity.getGroup().getDevice().getName(), entity.getName());
		messageService.registerSubscriberOrResubscribeExisting(new DataEntitySubscribtion(topicName, entity, this));
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

	public void onDeviceEntityDataReceived(Entity entity, String message) {
		try {
			JSONObject messageJson = new JSONObject(message);
			
			if(messageJson != null && !messageJson.isEmpty()) {
				
				JSONObject data = new JSONObject()
						.put(entity.getGroup().getName(), 
								new JSONObject().put(entity.getName(), messageJson));
				
				log.debug("MQ Data received : " + data);
				
				deviceService.processDataReceivedFromEntity(entity, data);
			}
			
		}catch(Exception e) {
			log.error(e);
		}
	}

}
