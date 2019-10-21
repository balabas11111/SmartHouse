package com.balabas.smarthouse.server.entity.model;

import java.util.Set;
import java.util.stream.Collectors;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Table;
import javax.persistence.Transient;

import com.balabas.smarthouse.server.entity.model.descriptor.ActionTimer;
import com.balabas.smarthouse.server.entity.model.descriptor.State;
import com.fasterxml.jackson.annotation.JsonIgnore;

import lombok.Data;
import lombok.EqualsAndHashCode;

@Data
@EqualsAndHashCode(callSuper = true)
@Entity
@Table(name="device")
public class Device extends ItemContainer<IGroup> implements IDevice {

	@Column
	private String firmware;
	
	@Column
	private String ip;
	@Column
	private String dataUrl;
	
	@JsonIgnore
	@Transient
	private ActionTimer timer;
	
	private State state;
	
	private boolean initialized;
	
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
	public Set<IEntity> getEntities() {
		return getGroups().stream().flatMap( group -> group.getEntities().stream())
				.collect(Collectors.toSet());
	}

	@Override
	public void setDeviceName(String deviceName) {
		setName(deviceName);
	}

	@Override
	public boolean isRegistered() {
		return state!=null && state.compareTo(State.REGISTERED) >= 0;
	}

	@Override
	public IEntity getEntity(String entityName) {
		return getGroups().stream().flatMap(group -> group.getEntities().stream()).filter( e -> e.getName().equals(entityName)).findFirst().orElse(null);
	}

	@Override
	public IEntity getEntity(int remoteId) {
		return getGroups().stream().flatMap(group -> group.getEntities().stream()).filter( e -> e.getRemoteId() == remoteId).findFirst().orElse(null);
	}

	@Override
	public IDevice update(IDevice device) {
		this.firmware = device.getFirmware();
		this.description = device.getDescription();
		this.ip = device.getIp();
		this.dataUrl = device.getDataUrl();
		this.timer = device.getTimer();
		return this;
	}

}
