package com.balabas.smarthouse.server.mqtt;

import java.util.List;
import java.util.Set;
import java.util.stream.Collectors;

import org.eclipse.paho.client.mqttv3.IMqttClient;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import lombok.extern.log4j.Log4j2;

@Log4j2
@Service
public class MessageService implements IMessageService {

	public static final String MQTT_TO_DEVICE_TOPIC = "to/%s";
	public static final String MQTT_FROM_DEVICE_TOPIC = "from/%s";

	public static final String MQTT_FROM_DEVICE_ENTITY_TOPIC = "%s/%s";

	public static final String PING = "ping";
	public static final String PONG = "pong";

	@Value("${smarthouse.server.mock}")
	private boolean mock;
	
	@Value("${mosquito.mqtt.enabled}")
	private boolean enabled;

	@Value("${mosquito.mqtt.publisher}")
	private String publisherId;

	@Value("${mosquito.mqtt.host}")
	private String host;

	@Value("${mosquito.mqtt.user}")
	private String user;

	@Value("${mosquito.mqtt.password}")
	private String password;

	private IMqttClient mqClient;

	@Autowired
	private Set<IMqttMessageSubscribtion> subscribers;

	@Override
	public void publish(String topic, String message) throws MqttException {
		if (!enabled) {
			log.error("Failed publish message");
			return;
		}

		getClient().publish(topic, new MqttMessage(message.getBytes()));
	}

	@Override
	public IMqttMessageSubscribtion getExistingSubscriber(IMqttMessageSubscribtion subscriber) {
		return this.subscribers.stream().filter(
				s -> (s.getClass().equals(subscriber.getClass()) && s.getTopicName().equals(subscriber.getTopicName())))
				.findFirst().orElse(null);
	}

	@Override
	public void registerSubscriberOrResubscribeExisting(IMqttMessageSubscribtion subscriber) {
		if (!enabled) {
			log.error("Failed register subscriber");
			return;
		}

		String message = subscriber.getClass().getSimpleName() + " topic :" + subscriber.getTopicName();
		IMqttMessageSubscribtion existing = getExistingSubscriber(subscriber);

		if (!subscribers.contains(subscriber) && existing == null) {

			subscribers.add(subscriber);
			log.info("Subscriber registered " + message);

			existing = subscriber;
		} else {
			log.info("Subscriber updated " + message);
		}

		subscribe(existing);
	}

	@Override
	public boolean publishToTopic(String topicName, String message) {
		try {
			publish(topicName, message);
		} catch (MqttException e) {
			log.error(e);
			return false;
		}
		return true;
	}

	@Override
	public <T extends IMqttMessageSubscribtion> List<T> getSubscribersByClass(Class<T> clazz) {
		return this.subscribers.stream().filter(clazz::isInstance).map(clazz::cast).collect(Collectors.toList());
	}

	@Override
	public <T extends IMqttMessageSubscribtion> List<T> getSubscribersByClass(Class<T> clazz, String topicNameLike) {
		return this.subscribers.stream().filter(clazz::isInstance).filter(s -> s.getTopicName().contains(topicNameLike))
				.map(clazz::cast).collect(Collectors.toList());
	}

	@Override
	public void subscribe(IMqttMessageSubscribtion subscriber) {
		try {
			getClient().subscribe(subscriber.getTopicName(), subscriber);
		} catch (MqttException e) {
			log.error(e);
		}
	}

	protected IMqttClient getClient() {
		try {
			if (this.mqClient == null) {
				this.mqClient = new MqttClient(host, publisherId);
			}

			MqttConnectOptions options = new MqttConnectOptions();
			options.setAutomaticReconnect(true);
			options.setCleanSession(true);
			options.setConnectionTimeout(10);
			options.setUserName(user);
			options.setPassword(password.toCharArray());

			if (!this.mqClient.isConnected()) {
				this.mqClient.connect(options);
			}
		} catch (MqttException e) {
			log.error(e);
		}

		return this.mqClient;
	}

	@Override
	public String getFromDeviceTopicId(String deviceId) {
		return String.format(MQTT_FROM_DEVICE_TOPIC, deviceId);
	}

	@Override
	public String getToDeviceTopicId(String deviceId) {
		return String.format(MQTT_TO_DEVICE_TOPIC, deviceId);
	}

	@Override
	public String getFromDeviceEntityTopicId(String deviceId, String entityName) {
		return String.format(MQTT_FROM_DEVICE_ENTITY_TOPIC, deviceId, entityName);
	}

}
