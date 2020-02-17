package com.balabas.entities.controller;

import java.util.stream.Collectors;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;

import com.balabas.abstr.controller.AbstractController;
import com.balabas.abstr.controller.AbstractControllerImpl;
import com.balabas.entities.model.PhoneNumber;
import com.balabas.entities.service.ServicePhoneCompany;

@Controller
@RequestMapping(value = "/PhoneNumber")
public class ControllerPhoneNumber extends AbstractControllerImpl<PhoneNumber> implements AbstractController<PhoneNumber>{
	
	@Autowired
	ServicePhoneCompany servicePhoneCompany;
	
	@Override
	public void addItemToModel(Model model, PhoneNumber entity) {
		super.addItemToModel(model, entity);
		model.addAttribute("companyList", servicePhoneCompany.list().stream().map( company -> company.getName()).collect(Collectors.toList()));
	}
}
