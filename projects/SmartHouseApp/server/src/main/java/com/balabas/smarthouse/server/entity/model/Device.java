package com.balabas.smarthouse.server.entity.model;

import java.util.Set;

import com.balabas.smarthouse.server.entity.model.descriptor.ActionTimer;
import com.fasterxml.jackson.annotation.JsonIgnore;

import lombok.Data;
import lombok.EqualsAndHashCode;

@Data
@EqualsAndHashCode(callSuper = true)
public class Device extends ItemContainer<IGroup> implements IDevice {

	private String firmware;
	
	private String ip;
	private String dataUrl;
	
	@JsonIgnore
	private ActionTimer timer;
	
	private State state;
	
	@JsonIgnore
	@Override
	public String getDeviceName() {
		return getName();
	}
	
	@Override
	public Set<IGroup> getGroups(){
		return getChildren();
	}

	@Override
	public void setDeviceName(String deviceName) {
		setName(deviceName);
	}

	@Override
	public boolean isInitialized() {
		return state!=null && state.compareTo(State.REGISTERED) > 0;
	}

	@Override
	public boolean isRegistered() {
		return state!=null && state.compareTo(State.REGISTERED) >= 0;
	}
}
