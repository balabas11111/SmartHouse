package com.balabas.entities.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;

import com.balabas.abstr.controller.AbstractController;
import com.balabas.abstr.controller.AbstractControllerImpl;
import com.balabas.entities.model.PhoneCompany;

@Controller
@RequestMapping(value = "/PhoneCompany")
public class ControllerPhoneCompany extends AbstractControllerImpl<PhoneCompany> implements AbstractController<PhoneCompany>{

}
