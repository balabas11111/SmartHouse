package com.balabas.smarthouse.server.mqtt;

import java.util.List;

import org.eclipse.paho.client.mqttv3.MqttException;

public interface IMessageService {
	
	void publish(String topic, String message) throws MqttException;
	
	void subscribe(IMqttMessageSubscribtion subscriber);
	
	void registerSubscriberOrResubscribeExisting(IMqttMessageSubscribtion subscriber);
	
	boolean publishToTopic(String topicName, String message);
	
	<T extends IMqttMessageSubscribtion> List<T> getSubscribersByClass(Class<T> clazz);

	<T extends IMqttMessageSubscribtion> List<T> getSubscribersByClass(Class<T> clazz, String topicNameLike);

	IMqttMessageSubscribtion getExistingSubscriber(IMqttMessageSubscribtion subscriber);

	String getFromDeviceTopicId(String deviceId);
	
	String getToDeviceTopicId(String deviceId);
	
	String getFromDeviceEntityTopicId(String deviceId, String entityName);
	
	String getToDeviceEntityTopicId(String deviceId, String entityName);
}
