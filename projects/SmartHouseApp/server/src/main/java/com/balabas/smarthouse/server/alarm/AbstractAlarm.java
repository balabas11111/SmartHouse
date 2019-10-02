package com.balabas.smarthouse.server.alarm;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import org.springframework.core.GenericTypeResolver;

import com.balabas.smarthouse.server.model.SmartHouseItem;
import com.balabas.smarthouse.server.notification.Message;

import lombok.Getter;
import lombok.Setter;

public abstract class AbstractAlarm<T extends SmartHouseItem> implements Alarm<T> {

	@Getter
	private Class<T> clazz;
	
	@Getter 
	protected T item;
	
	@Getter @Setter
	private Date createDate;
	
	@Getter @Setter
	private Date notifyDate;
	
	@Getter
	private boolean active;
	
	@Getter
	private boolean alarmDetected;
	
	@Getter @Setter
	private String deviceId;
	
	@Getter @Setter
	private String itemName;
	
	private Long notifyInterval = 30000L;
	
	@Getter
	protected List<Message> messages;
	
	@SuppressWarnings("unchecked")
	public AbstractAlarm() {
		this.clazz = (Class<T>)GenericTypeResolver.resolveTypeArgument(getClass(), AbstractAlarm.class);
	}

	@Override 
	public void setItem(T item) throws IllegalArgumentException {
		validateItem(item);
		this.item = item;
		notifyDate = null;
	}
	
	@Override
	public List<Message> checkItemForAlarmAndReschedule(){
		checkItemForAlarm();
		
		if(sendRequired()) {
			reschedule();
			return this.messages;
		}
		return null;
	}
	
	@Override
	public boolean checkItemForAlarm() {
		if(getItem() == null) {
			return false;
		}
		
		this.messages = new ArrayList<>();
		
		alarmDetected = doCheckItem();
		if(notifyDate == null && alarmDetected) {
			notifyDate = new Date();
		}
		
		return alarmDetected;
	}

	@Override
	public boolean sendRequired() {
		return (alarmDetected && notifyDate!=null && notifyDate.before(new Date()));
	}
	
	@Override
	public void reschedule() {
		this.notifyDate = (alarmDetected)?new Date(new Date().getTime() + notifyInterval):null;
	}
	
	protected boolean validateItem(T item) throws IllegalArgumentException {
		if(this.itemName==null || this.deviceId==null || (this.itemName!=null && !this.itemName.equals(item.getName()))
				|| (this.deviceId!=null && !this.deviceId.equals(item.getDeviceId()))) {
			throw new IllegalArgumentException("Alarm is not target for specified item");
		}
		return true;
	}
	
	protected abstract boolean doCheckItem();

}
