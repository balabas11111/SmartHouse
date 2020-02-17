package com.balabas.entities.controller;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;

@Controller
public class ControllerBase {

	@Value("${entities.service.base.entity}")
	private String entityBase;
	
	@GetMapping("/")
	public String getRoot(Model model) {
		return "redirect:/" + entityBase;
	}
}
