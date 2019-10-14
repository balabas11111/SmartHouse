package com.balabas.smarthouse.server.mqtt.subscribers;

import com.balabas.smarthouse.server.model.DeviceEntity;
import com.balabas.smarthouse.server.mqtt.MqttMessageSubscribtion;

import lombok.extern.log4j.Log4j2;

@Log4j2
public class DataEntitySubscribtion extends MqttMessageSubscribtion {

	private DeviceEntity entity;
	
	public DataEntitySubscribtion(String topicName, DeviceEntity entity){
		this.topicName = topicName;
		this.entity = entity;
	}
	
	@Override
	public boolean onMessageReceived(String topic, String message) {
		log.info("Received Entity:" + entity.getName() + " message:" + message);
		
		return true;
	}

}
