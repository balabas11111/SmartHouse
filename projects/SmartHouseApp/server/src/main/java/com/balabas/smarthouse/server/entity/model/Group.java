package com.balabas.smarthouse.server.entity.model;

import java.util.Set;

import javax.persistence.CascadeType;
import javax.persistence.EnumType;
import javax.persistence.Enumerated;
import javax.persistence.FetchType;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;
import javax.persistence.Table;
import javax.persistence.Transient;

import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;
import com.fasterxml.jackson.annotation.JsonIgnore;

import lombok.Data;
import lombok.EqualsAndHashCode;

@Data
@EqualsAndHashCode(callSuper = true, exclude = "entities")
@javax.persistence.Entity
@Table(name = "groups")
public class Group extends ItemAbstract implements IGroup {

	@Enumerated(EnumType.STRING)
	private ItemType type;
	
	//private String deviceName;

	@JsonIgnore
	@ManyToOne(fetch = FetchType.EAGER)
    @JoinColumn(name="device_id", nullable=false)
	Device device;
	
	@Transient
	private ActionTimer timer;
	
	@OneToMany(mappedBy="group", fetch = FetchType.EAGER, cascade = CascadeType.ALL)
	protected Set<Entity> entities;

	@Override
	public ActionTimer getTimer() {
		if(this.timer == null && this.getType()!=null) {
			this.timer = new ActionTimer(this.getType().getRefreshInterval());
		}
		return this.timer;
	}
	
	@Override
	public Entity getEntity(String entityName) {
		if(getEntities() == null) {
			return null;
		}
		return getEntities().stream().filter( e -> e.getName().equals(entityName)).findFirst().orElse(null);
	}
	
}
