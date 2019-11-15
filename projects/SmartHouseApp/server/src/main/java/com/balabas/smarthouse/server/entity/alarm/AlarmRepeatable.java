package com.balabas.smarthouse.server.entity.alarm;

import javax.persistence.MappedSuperclass;
import javax.persistence.Transient;

import com.balabas.smarthouse.server.entity.model.ActionTimer;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.fasterxml.jackson.annotation.JsonIgnore;

import lombok.Getter;
import lombok.Setter;

@MappedSuperclass
public abstract class AlarmRepeatable<T extends IItemAbstract> extends Alarm<T> implements IAlarmRepeatable<T> {

	public static final Integer NO_MESSAGE_SEND_REPEATS = -1;
	public static final String ALARM_FINISHED_STATUS = "показатели в норме";
	
	@Transient
	@Getter
	@Setter
	private ActionTimer timer;
	
	@Getter
	@Setter
	private boolean sound;
	
	@Setter @Getter
	protected boolean activated;
	
	@Transient
	@Getter @Setter
	boolean sendAlarmStartedMessage = false;
	@Transient
	@Getter @Setter
	boolean sendAlarmFinishedMessage = false;
	
	// if -1 send only one message
	@Getter
	Integer messageInterval;
	
	@Getter @Setter
	@Transient
	boolean logAlarmCheck;
	
	@Override
	public void setMessageInterval(Integer messageInterval) {
		this.messageInterval = messageInterval;
		
		if(this.getTimer() == null) {
			setTimer(new ActionTimer(1000 * messageInterval));
		} else {
			getTimer().setMessageInterval(1000 * messageInterval);
		}
	}
	
	@Override
	public boolean isNotificationRepeatable() {
		return messageInterval!=null && messageInterval.compareTo(NO_MESSAGE_SEND_REPEATS)>0;
	}

	@JsonIgnore
	@Override
	public boolean isAlarmStarted() {
		// need to send alarm started message
		return isActivated() && getTimer().isForcedAndTimeToExecute() && sendAlarmStartedMessage;
	}

	@JsonIgnore
	@Override
	public boolean isAlarmFinished() {
		// need to send alarm finished message
		return isActivated() && getTimer().isForcedAndTimeToExecute() && sendAlarmFinishedMessage;
	}

	@Override
	public void setAlarmStartedSent(boolean notified) {
		if (notified) {
			if (messageInterval == -1) {
				sendAlarmStartedMessage = false;
			} else {
				getTimer().update(getTimer().getInterval(), true);
			}
		}
	}

	@Override
	public void setAlarmFinishedSent(boolean notified) {
		if (notified) {
			sendAlarmFinishedMessage = false;
			getTimer().setActionForced(false);
		}
	}

}
