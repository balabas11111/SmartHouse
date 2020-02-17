package com.balabas.smarthouse.server.entity.alarmV2;

import javax.persistence.Entity;

import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;

@Entity
public class AlarmOfDevice extends AlarmV2 {

	protected Device device;
	
	@Override
	public Class<?> getTargetItemClass() {
		return Device.class;
	}
	
	@Override
	public IItemAbstract getItem() {
		return device;
	}

	@Override
	public void setItem(IItemAbstract item) {
		this.device = (Device) item;		
	}
	
}
