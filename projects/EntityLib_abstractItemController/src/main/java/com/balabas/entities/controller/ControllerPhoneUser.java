package com.balabas.entities.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;

import com.balabas.abstr.controller.AbstractController;
import com.balabas.abstr.controller.AbstractControllerImpl;
import com.balabas.entities.model.PhoneUser;

@Controller
@RequestMapping(value = "/PhoneUser")
public class ControllerPhoneUser extends AbstractControllerImpl<PhoneUser> implements AbstractController<PhoneUser>{

}
