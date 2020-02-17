package com.balabas.entities.service;

import org.springframework.stereotype.Service;

import com.balabas.abstr.service.AbstractService;
import com.balabas.abstr.service.AbstractServiceImpl;
import com.balabas.entities.model.PhoneUser;

@Service
public class ServicePhoneUser extends AbstractServiceImpl<PhoneUser> implements AbstractService<PhoneUser> {

	@Override
	public PhoneUser create() {
		return new PhoneUser();
	}

}
