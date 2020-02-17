package com.balabas.abstr.transformer;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;

import org.apache.commons.io.IOUtils;
import org.json.JSONArray;
import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.multipart.MultipartFile;

import com.balabas.abstr.model.ItemAbstract;
import com.balabas.abstr.service.AbstractService;

import lombok.Getter;

public class TransformerJson<T extends ItemAbstract> implements AbstractTransformer<T> {

	@Getter
	@Autowired
	private AbstractService<T> service;
	
	@Override
	public List<T> transform(MultipartFile file) throws IOException {
		
		String str = IOUtils.toString(file.getInputStream(), StandardCharsets.UTF_8.name());
		List<T> result = new ArrayList<T>(); 
		JSONArray listArr = new JSONArray(str);
		
		listArr.forEach(item -> {
			JSONObject obj = null; 
			
			T entity = service.create();
			
			if(!JSONObject.class.isAssignableFrom(item.getClass())) {
				obj = new JSONObject(item);
			} else {
				obj = (JSONObject) item;
			}
			
			
			objectToEntity(entity, obj);
			
			T entitySimilar = service.getSimilarTo(entity);
			
			if(entitySimilar!=null && entitySimilar.hasId()) {
				entity = service.join(entity, entitySimilar);
			}
			
			result.add(entity);
		});
		
		return result;
	}
	
	public T objectToEntity(T entity, JSONObject obj) {
		if(entity.getId() == null) {
			entity.setId(obj.optLong(ItemAbstract.PROP_ID, 0L));
		}
		if(entity.getName() == null || !entity.getName().equals(obj.getString(ItemAbstract.PROP_NAME)))
		entity.setName(obj.getString(ItemAbstract.PROP_NAME));
		return entity;
	}

}
