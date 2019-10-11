package com.balabas.smarthouse.server.mqtt;

import org.eclipse.paho.client.mqttv3.MqttMessage;

import lombok.Getter;

public abstract class MqttMessageSubscriberAbstract implements MqttMessageSubscriber {

	@Getter
	protected String topicName;
	
	@Override
	public void messageArrived(String topic, MqttMessage message) throws Exception {
		if(topic.equals(getTopicName())){
			onMessageReceived(new String(message.getPayload()));
		}
	}
}
