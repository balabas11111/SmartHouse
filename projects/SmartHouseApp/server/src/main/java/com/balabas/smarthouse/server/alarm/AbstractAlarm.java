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

	private static final Long MIN_SEND_INTERVAL_SEC = 60L;

	@JsonIgnore
	@Getter
	private Class<T> clazz;

	@JsonIgnore
	@Getter
	protected T item;

	@JsonIgnore
	@Getter
	@Setter
	private Date createDate;

	@JsonIgnore
	@Getter
	@Setter
	private Date notifyDate;

	@JsonIgnore
	@Getter
	@Setter
	private boolean active = false;

	@JsonIgnore
	@Getter
	private boolean alarmDetected;

	private boolean errorNotificationWasSent;

	@Getter
	@Setter
	private String deviceId;

	@Getter
	@Setter
	private String itemName;

	@Getter
	@Setter
	private String descriptionHead;

	@Getter
	private Long interval;

	@Getter
	@JsonIgnore
	protected List<Message> messages = new ArrayList<>();

	@SuppressWarnings("unchecked")
	public AbstractAlarm() {
		Class<T> c = (Class<T>) GenericTypeResolver.resolveTypeArguments(getClass(), AbstractAlarm.class)[0];
		this.clazz = c;
	}

	public void activate(Long alarmIntervalSec, T item, String descriptionHead) {
		this.setNotifyIntervalSec(alarmIntervalSec);
		this.setItem(item);
		this.setDescriptionHead(descriptionHead);
		this.setActive(true);

		log.info("Alarm activated " + this.getItemName() + " " + descriptionHead);
	}

	@Override
	public void setItem(T item) throws IllegalArgumentException {
		validateItem(item);
		this.item = item;
		notifyDate = null;
		if (this.descriptionHead == null) {
			this.descriptionHead = item.getDescription();
		}
	}

	@Override
	public void setNotifyIntervalSec(Long sec) {
		if(this.interval == null) {
			this.interval = ((sec!=null && sec >= MIN_SEND_INTERVAL_SEC) ? sec : MIN_SEND_INTERVAL_SEC) * 1000 ;
		}
	}

	@Override
	public Notification<T> checkItemForAlarmAndReschedule() {
		if (!active) {
			return createEmptyNotification();
		}
		checkItemForAlarm();

		if (sendRequired()) {
			errorNotificationWasSent = true;
			reschedule();
			return new AbstractNotification<>(item, getAlarmHeaderMessage(), this.messages);
		}

		if (errorNotificationWasSent && !alarmDetected) {
			errorNotificationWasSent = false;
			return new AbstractNotification<>(item, getNoAlarmHeaderMessage(), Collections.emptyList());
		}
		return createEmptyNotification();
	}

	@Override
	public boolean checkItemForAlarm() {
		if (!active) {
			return false;
		}

		this.messages.clear();

		alarmDetected = doCheckItem();
		if (notifyDate == null && alarmDetected) {
			notifyDate = new Date();
		}

		return alarmDetected;
	}

	@Override
	public boolean sendRequired() {
		if (!alarmDetected || notifyDate == null) {
			return false;
		}

		return isNotifyDateBeforeNow();
	}

	protected boolean isNotifyDateBeforeNow() {
		if (notifyDate == null) {
			return true;
		}
		Date now = new Date();
		boolean dateBefore = notifyDate.before(now);

		if (dateBefore) {
			log.info("Send required now=" + now + " notify=" + notifyDate);
		}

		return dateBefore;
	}

	@Override
	public void reschedule() {
		this.notifyDate = (alarmDetected) ? new Date(new Date().getTime() + interval) : null;
	}

	protected boolean validateItem(T item) throws IllegalArgumentException {
		if (this.itemName == null || this.deviceId == null || (!this.itemName.equals(item.getName()))
				|| (!this.deviceId.equals(item.getDeviceId()))) {
			throw new IllegalArgumentException("Alarm is not target for specified item");
		}
		return true;
	}

	protected boolean checkPutValue(String key, V obj) {
		return key != null && obj != null;
	}

	protected abstract boolean doCheckItem();

	protected Message getAlarmHeaderMessage() {
		String target = (Strings.isNullOrEmpty(descriptionHead) ? item.getDescription() : descriptionHead);
		target = String.format("%s - Режим тревоги ", target);

		return new Message(MessageSeverity.ERROR, MessageParent.DEVICE, target);
	}

	protected Message getNoAlarmHeaderMessage() {
		String target = (Strings.isNullOrEmpty(descriptionHead) ? item.getDescription() : descriptionHead);
		target = String.format("%s - значения в пределах нормы", target);

		return new Message(MessageSeverity.INFO, MessageParent.DEVICE, target);
	}

	protected abstract V doCast(String value) throws Exception;

	protected void putErrorMessage(String message) {
		this.messages.add(new Message(MessageSeverity.ERROR, MessageParent.DEVICE, message));
	}

	private Notification<T> createEmptyNotification() {
		return new AbstractNotification<>();
	}

}
