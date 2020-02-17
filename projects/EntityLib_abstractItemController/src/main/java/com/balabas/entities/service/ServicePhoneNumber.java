package com.balabas.entities.service;

import org.springframework.stereotype.Service;

import com.balabas.abstr.service.AbstractService;
import com.balabas.abstr.service.AbstractServiceImpl;
import com.balabas.entities.model.PhoneCompany;
import com.balabas.entities.model.PhoneNumber;

@Service
public class ServicePhoneNumber extends AbstractServiceImpl<PhoneNumber> implements AbstractService<PhoneNumber> {

	@Override
	public PhoneNumber create() {
		PhoneNumber result = new PhoneNumber();
		result.setCompany(new PhoneCompany());
		
		return result;
	}
}
