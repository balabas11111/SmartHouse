package com.balabas.smarthouse.server.controller;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.lang.Nullable;
import org.springframework.stereotype.Component;
import org.springframework.web.servlet.HandlerInterceptor;

import com.balabas.smarthouse.server.entity.service.IDeviceManageService;

import lombok.extern.log4j.Log4j2;

@Component
@Log4j2
public class RequestDeviceValuesInterceptor implements HandlerInterceptor {

	@Autowired
	IDeviceManageService deviceService;
	
	@Override
	public void afterCompletion(HttpServletRequest request, HttpServletResponse response, Object handler,
			@Nullable Exception ex) throws Exception {
		String deviceName = request.getParameter(DeviceController.DEVICE_NAME_PARAM);
		
		deviceService.requestDevicesValues(deviceName);
		log.info("RequestDeviceValuesInterceptor handled");
	}
}
