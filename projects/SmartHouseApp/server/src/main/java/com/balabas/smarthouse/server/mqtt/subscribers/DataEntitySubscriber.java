package com.balabas.smarthouse.server.mqtt.subscribers;

import com.balabas.smarthouse.server.model.DeviceEntity;
import com.balabas.smarthouse.server.mqtt.MqttMessageSubscriber;

import lombok.extern.log4j.Log4j2;

@Log4j2
public class DataEntitySubscriber extends MqttMessageSubscriber {

	private DeviceEntity entity;
	
	public DataEntitySubscriber(String topicName, DeviceEntity entity){
		this.topicName = topicName;
		this.entity = entity;
	}
	
	@Override
	public boolean onMessageReceived(String topic, String message) {
		log.info("Received Entity:" + entity.getName() + " message:" + message);
		
		return true;
	}

}
