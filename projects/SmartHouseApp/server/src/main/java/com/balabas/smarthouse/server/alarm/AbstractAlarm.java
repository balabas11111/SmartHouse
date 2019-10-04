package com.balabas.smarthouse.server.alarm;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;
import java.util.List;

import org.springframework.core.GenericTypeResolver;

import com.balabas.smarthouse.server.model.SmartHouseItem;
import com.balabas.smarthouse.server.notification.AbstractNotification;
import com.balabas.smarthouse.server.notification.Message;
import com.balabas.smarthouse.server.notification.Notification;
import com.balabas.smarthouse.server.notification.Message.MessageParent;
import com.balabas.smarthouse.server.notification.Message.MessageSeverity;
import com.fasterxml.jackson.annotation.JsonIgnore;
import com.google.common.base.Strings;

import lombok.Getter;
import lombok.Setter;
import lombok.extern.log4j.Log4j2;

@Log4j2
public abstract class AbstractAlarm<T extends SmartHouseItem, V extends Object> implements Alarm<T> {

	@JsonIgnore
	@Getter
	private Class<T> clazz;

	@JsonIgnore
	@Getter 
	protected T item;
	
	@JsonIgnore
	@Getter @Setter
	private Date createDate;
	
	@JsonIgnore
	@Getter @Setter
	private Date notifyDate;
	
	@JsonIgnore
	@Getter @Setter
	private boolean active = false;
	
	@JsonIgnore
	@Getter
	private boolean alarmDetected;
	
	@Getter @Setter
	private String deviceId;
	
	@Getter @Setter
	private String itemName;
	
	@Getter @Setter
	private String descriptionHead;
	
	@Getter
	private Long notifyInterval = 30000L;
	
	@Getter
	@JsonIgnore
	protected List<Message> messages = new ArrayList<>();
	
	@SuppressWarnings("unchecked")
	public AbstractAlarm() {
		Class<T> c = (Class<T>)GenericTypeResolver.resolveTypeArguments(getClass(), AbstractAlarm.class)[0];
		this.clazz = c;
	}

	@Override 
	public void setItem(T item) throws IllegalArgumentException {
		validateItem(item);
		this.item = item;
		notifyDate = null;
		if(this.descriptionHead==null){
			this.descriptionHead = item.getDescription();
		}
	}
	
	@Override
	public Notification<T> checkItemForAlarmAndReschedule(){
		boolean wasInAlarm = alarmDetected;
		
		checkItemForAlarm();
		
		if(sendRequired()) {
			reschedule();
			return new AbstractNotification<>(item, getAlarmHeaderMessage(), this.messages);
		}
		
		if(wasInAlarm){
			return new AbstractNotification<>(item, getNoAlarmHeaderMessage(), Collections.emptyList());
		}
		return new AbstractNotification<>();
	}
	
	@Override
	public boolean checkItemForAlarm() {
		if(getItem() == null) {
			return false;
		}
		
		this.messages.clear();
		
		alarmDetected = doCheckItem();
		if(notifyDate == null && alarmDetected) {
			notifyDate = new Date();
		}
		
		return alarmDetected;
	}

	@Override
	public boolean sendRequired() {
		if(!alarmDetected || notifyDate==null){
			return false;
		}
		Date now = new Date();
		boolean dateBefore = notifyDate.before(now);
		
		if(dateBefore){
			log.info("Send required now="+now+" notify="+notifyDate);
		}
		
		return dateBefore;
	}
	
	@Override
	public void reschedule() {
		this.notifyDate = (alarmDetected)?new Date(new Date().getTime() + notifyInterval):null;
	}
	
	protected boolean validateItem(T item) throws IllegalArgumentException {
		if(this.itemName==null || this.deviceId==null || (!this.itemName.equals(item.getName()))
				|| (!this.deviceId.equals(item.getDeviceId()))) {
			throw new IllegalArgumentException("Alarm is not target for specified item");
		}
		return true;
	}
	
	protected boolean checkPutValue(String key, V obj){
		return key!=null && obj!=null;
	}
	
	protected abstract boolean doCheckItem();
	
	protected Message getAlarmHeaderMessage(){
		String target =(Strings.isNullOrEmpty(descriptionHead)?item.getDescription():descriptionHead);
		target = String.format("%s - Режим тревоги ", target);
		
		return new Message(MessageSeverity.ERROR, MessageParent.DEVICE, target);
	}
	
	protected Message getNoAlarmHeaderMessage(){
		String target =(Strings.isNullOrEmpty(descriptionHead)?item.getDescription():descriptionHead);
		target = String.format("%s - значения в пределах нормы", target);
		
		return new Message(MessageSeverity.INFO, MessageParent.DEVICE, target);
	}
	
	protected abstract V doCast(String value) throws Exception;
	
	protected void putErrorMessage(String message){
		this.messages.add(new Message(MessageSeverity.ERROR, MessageParent.DEVICE, message));
	}

}
