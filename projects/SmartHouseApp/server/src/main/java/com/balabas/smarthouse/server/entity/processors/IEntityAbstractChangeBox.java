package com.balabas.smarthouse.server.entity.processors;

import org.json.JSONObject;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

import lombok.Getter;
import lombok.Setter;

public abstract class IEntityAbstractChangeBox {

	@Getter
	@Setter
	private JSONObject params;
	
	@Getter
	@Setter
	private boolean change = false;
	
	public abstract String getDefaultParameter();
	
	public abstract IEntityAbstractChangeBox build(IEntityAbstractProvider p, IItemAbstract item);
	
	public abstract boolean check();

	protected abstract boolean executeChange(IEntityAbstractProvider p);
	
	protected boolean getValOrFalse(IEntityField<Boolean> field) {
		return field!=null && field.getValue()!=null ? field.getValue().booleanValue() : false;
	}
}
