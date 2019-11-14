package com.balabas.smarthouse.server.entity.service;

import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.mqtt.IMessageService;
import com.balabas.smarthouse.server.mqtt.subscribers.DataDeviceSubscribtion;
import com.balabas.smarthouse.server.mqtt.subscribers.DataEntitySubscribtion;

import lombok.extern.log4j.Log4j2;

@Log4j2
@Service
public class DeviceMqService implements IDeviceMqService {

	@Value("${smarthouse.server.name}")
	private String serverName;

	@Value("${smarthouse.server.mock}")
	private boolean mock;

	@Value("${mosquito.mqtt.enabled}")
	private boolean enabled;

	@Autowired
	private IMessageService messageService;

	@Autowired
	private IDeviceManageService deviceService;

	@Override
	public void initTopicsToFromDevice(String deviceId) {
		if (!enabled) {
			log.info("Init to/from device topics DISABLED " + deviceId);
			return;
		}

		IDevice device = deviceService.getDeviceByName(deviceId);

		if (device != null) {
			
			boolean hasDeviceTopic = false;
			for(IEntity entity : device.getEntities()) {
				hasDeviceTopic = entity.isHasMq() || hasDeviceTopic;
			}
			
			if(hasDeviceTopic) {
				log.info("Init to/from device topics " + deviceId);
	
				String topicName = messageService.getFromDeviceTopicId(device.getName());
	
				messageService.registerSubscriberOrResubscribeExisting(new DataDeviceSubscribtion(topicName, this));
	
				deviceService.getDeviceByName(device.getName()).getEntities().stream()
						.filter(IEntity::isHasMq)
						.forEach(entity -> subscribeFromDeviceEntityTopic(entity));
			} else {
				log.info("Device has no topics");
			}
		}
	}

	@Override
	public boolean publishToDeviceTopic(String deviceId, String message) {
		String topicName = messageService.getToDeviceTopicId(deviceId);
		return messageService.publishToTopic(topicName, message);
	}

	public void subscribeFromDeviceEntityTopic(Entity entity) {
			String topicName = messageService.getFromDeviceEntityTopicId(entity.getGroup().getDevice().getName(),
					entity.getName());
			messageService.registerSubscriberOrResubscribeExisting(new DataEntitySubscribtion(topicName, entity, this));
	}

	public void onDeviceEntityDataReceived(Entity entity, String message) {
		try {
			JSONObject messageJson = new JSONObject(message);

			if (messageJson != null && !messageJson.isEmpty()) {

				JSONObject data = new JSONObject().put(entity.getGroup().getName(),
						new JSONObject().put(entity.getName(), messageJson));

				log.debug("MQ Data received : " + data);

				deviceService.processDataReceivedFromEntity(entity, data);
			}

		} catch (Exception e) {
			log.error(e);
		}
	}

}
