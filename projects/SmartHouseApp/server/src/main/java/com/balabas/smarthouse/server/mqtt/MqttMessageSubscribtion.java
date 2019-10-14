package com.balabas.smarthouse.server.mqtt;

import org.eclipse.paho.client.mqttv3.MqttMessage;

import com.balabas.smarthouse.server.service.IDeviceMessageService;

import lombok.Getter;

public abstract class MqttMessageSubscribtion implements IMqttMessageSubscribtion {

	@Getter
	protected String topicName;
	
	@SuppressWarnings("unused")
	protected IDeviceMessageService service;
	
	@Override
	public void messageArrived(String topic, MqttMessage message) throws Exception {
		if(topic.equals(getTopicName())){
			onMessageReceived(topic, new String(message.getPayload()));
		}
	}
}
