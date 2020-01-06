package com.balabas.smarthouse.server.controller;

import org.springframework.web.servlet.support.ServletUriComponentsBuilder;
import org.thymeleaf.util.StringUtils;

public class BaseController {

	public static final String BASE_URL_PROPERTY = "baseUrl";

	private String baseUrl;

	protected String getBaseUrl() {
		if(StringUtils.isEmpty(baseUrl)) {
			baseUrl = ServletUriComponentsBuilder.fromCurrentContextPath().build().toUriString();
		}
		
		return baseUrl;
	}
}
