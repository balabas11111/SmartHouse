package com.balabas.smarthouse.server.mqtt.subscribers;

import com.balabas.smarthouse.server.model.DeviceEntity;
import com.balabas.smarthouse.server.mqtt.MqttMessageSubscribtion;
import com.balabas.smarthouse.server.service.IDeviceMessageService;

import lombok.extern.log4j.Log4j2;

@Log4j2
public class DataEntitySubscribtion extends MqttMessageSubscribtion {

	private DeviceEntity entity;
		
	public DataEntitySubscribtion(String topicName, DeviceEntity entity, IDeviceMessageService messageService){
		this.topicName = topicName;
		this.entity = entity;
		this.service = messageService;
	}
	
	@Override
	public void onMessageReceived(String topic, String message) {
		log.debug("Received Entity:" + entity.getName() + " message:" + message);
		service.onDeviceEntityDataReceived(entity, message);
	}

}
