package com.balabas.entities.service;

import org.springframework.stereotype.Service;

import com.balabas.abstr.service.AbstractService;
import com.balabas.abstr.service.AbstractServiceImpl;
import com.balabas.entities.model.PhoneCompany;

@Service
public class ServicePhoneCompany extends AbstractServiceImpl<PhoneCompany> implements AbstractService<PhoneCompany> {

	@Override
	public PhoneCompany create() {
		return new PhoneCompany();
	}

}
