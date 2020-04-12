package com.balabas.smarthouse.server.entity.alarmV2;

import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.INameable;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

public interface IAlarmStateChangeEventProcessor extends INameable {

	boolean isTarget(IItemEvent event);
	void process(IItemEvent event);
	
	default IEntity getItemAsEntity(IItemEvent event) {
		return (IEntity) event.getItem();
	}
	
	@SuppressWarnings("rawtypes")
	default IEntityField getItemAsEntityField(IItemEvent event) {
		return (IEntityField) event.getItem();
	}
}
