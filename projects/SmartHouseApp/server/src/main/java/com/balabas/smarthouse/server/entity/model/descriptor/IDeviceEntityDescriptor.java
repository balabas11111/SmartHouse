package com.balabas.smarthouse.server.entity.model.descriptor;

import java.util.Map;

public interface IDeviceEntityDescriptor {

	int getRemoteId();
	String getName();
	String getDescriptionField();
	Emoji getEmoji();
	
	Map<String, IEntityFieldDescriptor> getDescriptors();
}
