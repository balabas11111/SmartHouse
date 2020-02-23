package com.balabas.smarthouse.server.entity.alarmV2;

import org.springframework.util.StringUtils;

import lombok.Getter;
import lombok.Setter;
import lombok.extern.log4j.Log4j2;

@Log4j2
public abstract class AlarmStateChangeEventProcessor implements IAlarmStateChangeEventProcessor {

	@Getter @Setter
	private String name;

	public AlarmStateChangeEventProcessor() {
		this.setName(this.getClass().getSimpleName());
	}

	@Override
	public void processEvent(IItemEvent event) {
		logEvent(event);
	}
	
	@Override
	public boolean isTarget(IItemEvent event) {
		return event!=null && !StringUtils.isEmpty(event.getChangeAction().getTargetBeanName())
				&& name.equals(event.getChangeAction().getTargetBeanName());
	}
	
	protected void logEvent(IItemEvent event) {
		log.info(getName() + " " + getStateChange(event) + " descr=" + event.getDescription() + " e=" + event);
	}
	
	protected String getStateChange(IItemEvent event) {
		return event.isSameState()?("state=" + event.getNewStateName()):("old=" + event.getOldStateName()
				+ " new=" + event.getNewStateName());
	}

}
