package com.balabas.smarthouse.server.entity.alarmV2;

import javax.persistence.Entity;
import javax.persistence.FetchType;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityField;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.view.MessageHolder;

import lombok.Getter;
import lombok.Setter;

@Entity
@SuppressWarnings("rawtypes")
public class AlarmOfEntityField extends AlarmV2 {
	
	@ManyToOne(fetch = FetchType.EAGER)
    @JoinColumn(name="entity_field_id", nullable=false)
	@Getter @Setter
	private EntityField entityField;
	
	@Override
	public Class<?> getTargetItemClass() {
		return EntityField.class;
	}
	
	@Override
	public IItemAbstract getItem() {
		return entityField;
	}

	@Override
	public void setItem(IItemAbstract item) {
		this.entityField = (EntityField) item;		
	}
	
	protected IEntityField getItemAsEntityField(IItemAbstract item) {
		return (IEntityField) item;
	}
	
	protected MessageHolder getMessageHolder(IItemAbstract item, String status) {
		IEntityField field = getItemAsEntityField(item);
		
		String description = field.getDescriptionByDescriptionField(); 
		MessageHolder holder = new MessageHolder(field.getEntity().getDevice().getDescription(), description, status, field.getEmoji());
		
		return holder;
	}

}
