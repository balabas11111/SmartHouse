package com.balabas.smarthouse.server.controller;

import org.springframework.web.servlet.support.ServletUriComponentsBuilder;
import org.thymeleaf.util.StringUtils;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;

public class BaseController {

	private String baseUrl;

	protected String getBaseUrl() {
		if(StringUtils.isEmpty(baseUrl)) {
			baseUrl = ServletUriComponentsBuilder.fromCurrentContextPath().build().toUriString();
		}
		
		return baseUrl;
	}
	
	protected String getPageHeader(String preffix, IItemAbstract item) {
		return preffix + " " + item.getDescriptionByDescriptionField();
	}
}
