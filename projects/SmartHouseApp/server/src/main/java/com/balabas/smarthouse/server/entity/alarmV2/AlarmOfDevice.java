package com.balabas.smarthouse.server.entity.alarmV2;

import javax.persistence.Entity;
import javax.persistence.FetchType;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;

import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;

import lombok.Getter;
import lombok.Setter;

@Entity
public class AlarmOfDevice extends AlarmV2 {

	@ManyToOne(fetch = FetchType.EAGER)
    @JoinColumn(name="device_id", nullable=false)
	@Getter @Setter
	private Device device;
	
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
