package com.balabas.smarthouse.server.entity.alarmV2;

import org.springframework.util.StringUtils;

import lombok.Getter;
import lombok.Setter;

public abstract class AlarmStateChangeEventProcessor implements IAlarmStateChangeEventProcessor {

	@Getter @Setter
	private String name;

	@Override
	public boolean isTarget(IAlarmStateChangeEvent event) {
		return event!=null && !StringUtils.isEmpty(event.getChangeAction().getTargetBeanName())
				&& name.equals(event.getChangeAction().getTargetBeanName());
	}

}
