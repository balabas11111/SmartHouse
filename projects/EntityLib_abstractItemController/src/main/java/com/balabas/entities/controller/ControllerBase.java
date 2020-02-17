package com.balabas.entities.controller;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.HttpStatus;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.servlet.ModelAndView;

@Controller
public class ControllerBase {

	@Value("${entities.service.base.entity}")
	private String entityBase;
	
	@GetMapping("/")
	public String getRoot(Model model) {
		return "redirect:/" + entityBase;
	}
	
	@ExceptionHandler(Exception.class)
    public ModelAndView handleCustomException(Exception ex) {

        ModelAndView model = new ModelAndView("error/generic__error");
        model.addObject("errCode", HttpStatus.INTERNAL_SERVER_ERROR.value());
        model.addObject("errMsg", ex.getMessage());

        return model;
    }
}
