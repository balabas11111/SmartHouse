package com.balabas.smarthouse.server.entity.processors;

import org.springframework.beans.factory.annotation.Autowired;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.virtual.IVirtualEntityService;
import com.balabas.smarthouse.server.entity.service.IDeviceManageService;

import lombok.Getter;

@SuppressWarnings("unchecked")
public abstract class EntityAbstractChangeBoxProcessor<T extends IEntityAbstractChangeBox, S extends IItemAbstract> implements IEntityAbstractChangeBoxProcessor<T, S>  {

	@Getter
	@Autowired
	protected IVirtualEntityService virtualEntityService;
	
	@Autowired
	@Getter
	protected IDeviceManageService deviceService;
}
