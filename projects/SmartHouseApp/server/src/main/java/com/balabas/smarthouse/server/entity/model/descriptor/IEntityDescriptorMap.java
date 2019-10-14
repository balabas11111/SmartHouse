package com.balabas.smarthouse.server.entity.model.descriptor;

import java.util.Map;

public interface IEntityDescriptorMap {

	int getRemoteId();
	String getName();
	
	Map<String, IEntityDescriptor> getDescriptors();
}
