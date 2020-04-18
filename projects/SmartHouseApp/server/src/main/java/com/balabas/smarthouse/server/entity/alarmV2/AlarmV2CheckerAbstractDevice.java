package com.balabas.smarthouse.server.entity.alarmV2;

import com.balabas.smarthouse.server.entity.alarmV2.model.IAlarmV2;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.util.MathUtil;

public abstract class AlarmV2CheckerAbstractDevice {

	protected abstract boolean checkItemValue(IAlarmV2 alarm);
	
	public Class<?> getItemClass() {
		return IDevice.class;
	}
	
	public void process(IAlarmV2 alarm) {
		alarm.setAlarmStateByBooleanFlagAlarm(checkItemValue(alarm));
	}
	
	public boolean isParameterValid(String value) {
		return MathUtil.isValidFloat(value);
	}
	
	protected IDevice getItemAsDevice(IAlarmV2 alarm) {
		return (IDevice) alarm.getItem();
	}

}
