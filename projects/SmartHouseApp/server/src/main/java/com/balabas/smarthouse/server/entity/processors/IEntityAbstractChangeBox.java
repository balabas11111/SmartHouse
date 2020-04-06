package com.balabas.smarthouse.server.entity.processors;

import org.json.JSONObject;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;

import lombok.Getter;
import lombok.Setter;

public abstract class IEntityAbstractChangeBox {

	@Getter
	@Setter
	private JSONObject params;
	
	@Getter
	@Setter
	private boolean change = false;
	
	public abstract IEntityAbstractChangeBox build(IEntityAbstractProvider p, IItemAbstract item);
	
	public abstract boolean check();
}
