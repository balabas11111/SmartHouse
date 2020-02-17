package com.balabas.entities.transformer;

import java.util.List;

import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.balabas.abstr.transformer.AbstractTransformer;
import com.balabas.abstr.transformer.TransformerJson;
import com.balabas.entities.model.PhoneCompany;
import com.balabas.entities.model.PhoneNumber;

@Service
public class TransformerPhoneNumber extends TransformerJson<PhoneNumber> implements AbstractTransformer<PhoneNumber> {

	@Autowired
	TransformerPhoneCompany transformerPhoneCompany;
	
	@Override
	public PhoneNumber objectToEntity(PhoneNumber entity, JSONObject obj) {
		super.objectToEntity(entity, obj);
		
		entity.setNumber(obj.optString(PhoneNumber.PROP_NUMBER,""));
		
		if(entity.getCompany() == null || !entity.getCompany().hasId()) {
			PhoneCompany company = transformerPhoneCompany.getService().create();
			JSONObject companyObj = obj.getJSONObject(PhoneNumber.PROP_COMPANY);
			
			transformerPhoneCompany.objectToEntity(company, companyObj);
			
			List<PhoneCompany> companies = transformerPhoneCompany.getService().getByName(company.getName());
			
			if(companies.size()>0) {
				company = companies.get(0);
				
				if(!company.hasId()) {
					company = transformerPhoneCompany.getService().save(company);
				}
			} else {
				company = transformerPhoneCompany.getService().save(company);
			}
			
			entity.setCompany(company);
		}
		
		return entity;
	}

}
