package com.balabas.smarthouse.server.entity.alarmV2;

import javax.persistence.MappedSuperclass;

import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;

@MappedSuperclass
public abstract class AlarmOfDevice extends AlarmV2 {

	protected Device device;
	
	@Override
	public Class<?> getItemClass() {
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
