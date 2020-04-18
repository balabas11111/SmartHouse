package com.balabas.smarthouse.server.entity.service;

import com.balabas.smarthouse.server.entity.alarmV2.model.IAlarmV2;
import com.balabas.smarthouse.server.entity.model.ActionTimer;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IGroup;
import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;

public interface IActionTimerService {

	ActionTimer getActionTimer(ItemType itemType, Long itemId);
	
	ActionTimer getActionTimer(IDevice device);
	
	ActionTimer getActionTimer(IGroup group);
	
	ActionTimer getActionTimer(IEntity entity);
	
	ActionTimer getActionTimer(IAlarmV2 entity);
	
	void setActionSuccess(IDevice device);
	
	void setActionSuccess(IGroup group);
	
	void setActionSuccess(IEntity entity);
	
	void setActionSuccess(ActionTimer timer);

	void setActionForced(IDevice device);

	void setActionForced(IAlarmV2 alarm);

	ActionTimer getActionTimer(ItemType itemType, Long itemId, Long updateInterval);

	void initAlarmScheduling(IAlarmV2 alarm);

	void nextAlarmScheduling(IAlarmV2 alarm);

	boolean requireScheduledCheck(IAlarmV2 alarm);
	
}
