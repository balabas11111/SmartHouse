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

import static com.balabas.smarthouse.server.mqtt.MessageService.PING;

import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.model.DeviceEntity;
import com.balabas.smarthouse.server.mqtt.IMessageService;
import com.balabas.smarthouse.server.mqtt.subscribers.DataDeviceSubscribtion;
import com.balabas.smarthouse.server.mqtt.subscribers.DataEntitySubscribtion;
import com.balabas.smarthouse.server.security.DeviceSecurityContext;
import com.balabas.smarthouse.server.security.DeviceSecurityService;

import lombok.extern.log4j.Log4j2;

@Log4j2
@Service
public class DeviceMessageService implements IDeviceMessageService {

	@Value("${smarthouse.server.name}")
	private String serverName;

	@Autowired
	private DeviceSecurityService securityService;

	@Autowired
	private IMessageService messageService;

	@Override
	public boolean onRegistrationTopicMessageReceived(String message) {
		try {
			log.info("processRegistrationRequest " + message);
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
	}
	
	@Override
	public boolean publishToDeviceTopic(String deviceId, String message) {
		String topicName = messageService.getToDeviceTopicId(deviceId);
		return messageService.publishToTopic(topicName, message);
	}
	
	@Override
	public void initTopicsToFromDeviceEntity(DeviceEntity entity) {
		subscribeFromDeviceEntityTopic(entity);
		publishToDeviceEntityTopic(entity, PING);
	}
	
	@Override
	public void subscribeFromDeviceEntityTopic(DeviceEntity entity) {
		String topicName = messageService.getFromDeviceEntityTopicId(entity.getDeviceId(), entity.getName());
		messageService.registerSubscriberOrResubscribeExisting(new DataEntitySubscribtion(topicName, entity));
	}
	
	@Override
	public boolean publishToDeviceEntityTopic(DeviceEntity entity, String message) {
		String topicName = messageService.getToDeviceEntityTopicId(entity.getDeviceId(), entity.getName());
		return messageService.publishToTopic(topicName, message);
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