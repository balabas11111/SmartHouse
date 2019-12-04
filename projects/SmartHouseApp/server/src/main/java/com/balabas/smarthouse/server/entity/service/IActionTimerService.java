package com.balabas.smarthouse.server.entity.service;

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
	
	void setActionSuccess(IDevice device);
	
	void setActionSuccess(IGroup group);
	
	void setActionSuccess(IEntity entity);
	
	void setActionSuccess(ActionTimer timer);

	void setActionForced(IDevice device);
}
