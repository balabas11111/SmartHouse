package com.balabas.abstr.transformer;

import java.io.IOException;
import java.util.List;

import org.json.JSONObject;
import org.springframework.web.multipart.MultipartFile;

import com.balabas.abstr.model.ItemAbstract;
import com.balabas.abstr.service.AbstractService;

public interface AbstractTransformer<T extends ItemAbstract> {

	List<T> transform(MultipartFile file) throws IOException;
	
	T objectToEntity(T entity, JSONObject obj);
	
	AbstractService<T> getService();
}
