package com.balabas.smarthouse.server.entity.alarmV2.model;

import javax.persistence.Entity;
import javax.persistence.FetchType;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.Transient;

import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;

import lombok.Getter;
import lombok.Setter;

@Entity
public class AlarmOfDevice extends AlarmV2 {

	@ManyToOne(fetch = FetchType.EAGER)
    @JoinColumn(name="device_id", nullable=false)
	@Getter @Setter
	private Device device;

	@Getter
	@Transient
	private final Class<?> targetItemClass = Device.class;
	
	@Getter
	@Transient
	private final ItemType itemType = ItemType.DEVICE;
	
	@Override
	public IItemAbstract getItem() {
		return device;
	}

	@Override
	public void setItem(IItemAbstract item) {
		this.device = (Device) item;		
	}
	
}
