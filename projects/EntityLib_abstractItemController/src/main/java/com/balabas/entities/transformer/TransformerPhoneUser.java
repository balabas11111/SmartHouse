package com.balabas.entities.transformer;

import java.util.ArrayList;
import java.util.List;

import org.json.JSONArray;
import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.balabas.abstr.model.ItemAbstract;
import com.balabas.abstr.transformer.AbstractTransformer;
import com.balabas.abstr.transformer.TransformerJson;
import com.balabas.entities.model.PhoneNumber;
import com.balabas.entities.model.PhoneUser;

@Service
public class TransformerPhoneUser extends TransformerJson<PhoneUser> implements AbstractTransformer<PhoneUser> {

	@Autowired
	TransformerPhoneNumber transformarPhoneNumber;
	
	@SuppressWarnings({ "rawtypes", "unchecked" })
	@Override
	public PhoneUser objectToEntity(PhoneUser entity, JSONObject obj) {
		super.objectToEntity(entity, obj);
		
		if(entity.getNumbers() == null || entity.getNumbers().size() == 0) {
			List<PhoneNumber> numbers = new ArrayList();
			JSONArray numbersObj = obj.optJSONArray(PhoneUser.PROP_NUMBERS);
			
			for(Object number : numbersObj) {
				JSONObject numberObj = (JSONObject) number;
				Long numberId = numberObj.optLong(ItemAbstract.PROP_ID, -1);
				 
				if(numberId>0) {
					PhoneNumber existNumber = transformarPhoneNumber.getService().get(numberId);
					
					if(existNumber!=null) {
						numbers.add(existNumber);
					}
				}
			}
			
			entity.setNumbers(numbers);
		}
		
		return entity;
	}
}
