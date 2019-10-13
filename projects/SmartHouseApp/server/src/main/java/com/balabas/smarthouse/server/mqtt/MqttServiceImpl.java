package com.balabas.smarthouse.server.mqtt;

import java.util.Set;

import org.eclipse.paho.client.mqttv3.IMqttClient;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import lombok.extern.log4j.Log4j2;

@Log4j2
@Service
public class MqttServiceImpl implements InitializingBean, MqttService {
	
	public static final String MQTT_TOPIC_REGISTRATION = "fromDevice/register";
	public static final String MQTT_TOPIC_DATA = "fromDevice/data";
	public static final String MQTT_TO_DEVICE_TOPIC_TMPL = "toDevice/%s";
	public static final String MQTT_FROM_DEVICE_TOPIC_TMPL = "fromDevice/%s";
	
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
	private Set<IMqttMessageSubscriber> subscribers;
		
	
	@Override
	public void afterPropertiesSet() throws Exception {
		subscribers.stream().forEach(this::subscribe);	
	}
	
	
	@Override
	public void publish(String topic, String message) throws MqttException {
		if(!enabled){
			log.error("Failed publish message");
			return;
		}
		
		getClient().publish(topic, new MqttMessage(message.getBytes()));
	}

	@Override
	public void registerSubscriber(IMqttMessageSubscriber subscriber) {
		if(!enabled){
			log.error("Failed register subscriber");
			return;
		}
		
		if(!subscribers.contains(subscriber)){
			subscribers.add(subscriber);
			subscribe(subscriber);
			log.info("Subscriber registered " + subscriber.getClass().getSimpleName() + " topic :"+subscriber.getTopicName());
		}
	}

	@Override
	public String getFromDeviceTopic(String deviceId) {
		return String.format(MQTT_FROM_DEVICE_TOPIC_TMPL, deviceId);
	}

	@Override
	public String getToDeviceTopic(String deviceId) {
		return String.format(MQTT_TO_DEVICE_TOPIC_TMPL, deviceId);
	}
	
	protected void subscribe(IMqttMessageSubscriber subscriber){
		try {
			getClient().subscribe(subscriber.getTopicName(), subscriber);
		} catch (MqttException e) {
			log.error(e);
		}
	}
	
	protected IMqttClient getClient(){
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

}
