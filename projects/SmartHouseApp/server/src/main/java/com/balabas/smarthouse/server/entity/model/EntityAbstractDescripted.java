package com.balabas.smarthouse.server.entity.model;

import com.balabas.smarthouse.server.entity.model.descriptor.EntityDescriptor;

import lombok.Getter;
import lombok.Setter;

public class EntityAbstractDescripted extends EntityAbstract {
	@Getter @Setter
	EntityDescriptor descriptor;
}
