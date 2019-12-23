package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.List;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;

public interface IAlarmV2Service {

	boolean checkForAlarm(IItemAbstract item);
	
	List<IAlarmV2> getEnabledAlarms(IItemAbstract item);
	
	void saveAlarm(IAlarmV2 alarm);
	
	void deleteAlarm(IAlarmV2 alarm);
}
