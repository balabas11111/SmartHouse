package com.balabas.smarthouse.server.controller;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.springframework.web.servlet.config.annotation.InterceptorRegistry;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurer;

@Component
public class DeviceServiceInterceptorAppConfig implements WebMvcConfigurer{

	@Autowired
	RequestDeviceValuesInterceptor deviceServiceInterceptor;
	
	@Override
	public void addInterceptors(InterceptorRegistry registry) {
		registry.addInterceptor(deviceServiceInterceptor).addPathPatterns(ControllerConstants.DEVICES_ROOT_V1 + "/dataChanged");
	}
}
