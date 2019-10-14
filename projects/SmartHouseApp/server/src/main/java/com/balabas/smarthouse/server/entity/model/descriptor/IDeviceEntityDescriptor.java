package com.balabas.smarthouse.server.entity.model.descriptor;

import java.util.Map;

public interface IDeviceEntityDescriptor {

	int getRemoteId();
	String getName();
	IEntityDescriptor getDescriptionDescriptor();
	Emoji getEmoji();
	
	Map<String, IEntityDescriptor> getDescriptors();
}
