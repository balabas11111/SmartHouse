package com.balabas.smarthouse.server.mqtt.subscribers;

import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.service.IDeviceMqService;
import com.balabas.smarthouse.server.mqtt.MqttMessageSubscribtion;

import lombok.extern.log4j.Log4j2;

@Log4j2
public class DataEntitySubscribtion extends MqttMessageSubscribtion {

	private Entity entity;
		
	public DataEntitySubscribtion(String topicName, Entity entity, IDeviceMqService messageService){
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
