package com.balabas.smarthouse.server.entity.model.collectors;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import lombok.extern.log4j.Log4j2;

@Service
@Log4j2
public class DataTransformerService implements IDataTransformerService{

	@SuppressWarnings({ "rawtypes", "unchecked" })
	private Map<String, IDataTransformer> transformers = new HashMap();

	@Autowired
	public void setTransformers(List<IDataTransformer> list) {
		for(IDataTransformer dt : list) {
			transformers.put(dt.getName(), dt);
		}
		log.info("Total transformers " + list.size());
	}
	
	@Override
	public IDataTransformer getTransformer(String name) {
		return transformers.getOrDefault(name, null);
	}

	@Override
	public Set<String> getTransformerNames() {
		return transformers.keySet();
	}
}
