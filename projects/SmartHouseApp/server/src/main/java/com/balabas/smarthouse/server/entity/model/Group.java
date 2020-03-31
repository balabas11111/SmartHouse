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

import com.balabas.smarthouse.server.entity.model.descriptor.AbstractItemType;
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
	
	@JsonIgnore
	@ManyToOne(fetch = FetchType.EAGER)
    @JoinColumn(name="device_id", nullable=true)
	Device device;

	@OneToMany(mappedBy="group", fetch = FetchType.EAGER, cascade = CascadeType.ALL)
	protected Set<Entity> entities;
	
	@Override
	public Entity getEntity(String entityName) {
		if(getEntities() == null) {
			return null;
		}
		return getEntities().stream().filter( e -> e.getName().equals(entityName)).findFirst().orElse(null);
	}
	
	@Override
	public void setParent(IItemAbstract parent) {
		if(parent!=null && !Device.class.isAssignableFrom(parent.getClass())) {
			throw new IllegalArgumentException("IEntity expected");
		}
		setDevice((Device) parent);
	}
	
	@Override
	public AbstractItemType getItemAbstractType() {
		return AbstractItemType.GROUP;
	}
	
	@Override
	public String getParentNamesChain() {
		return getDevice().getParentNamesChain() + "-" + getDescriptionByDescriptionField();
	}
}
