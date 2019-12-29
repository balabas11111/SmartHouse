package com.balabas.smarthouse.server.entity.model.collectors;

import java.util.Set;

public interface IDataTransformerService {

	IDataTransformer getTransformer(String name);
	
	Set<String> getTransformerNames();
}
