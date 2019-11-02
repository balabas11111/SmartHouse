package com.balabas.smarthouse.server.controller;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;

import com.balabas.smarthouse.server.entity.service.DeviceManageService;

@Controller
@RequestMapping(ControllerConstants.DEVICES_ROOT)
public class ViewDeviceController {

	@Autowired
	private DeviceManageService deviceService;
	
	@GetMapping("/")
    public String getDevicesView(Model model) {
        model.addAttribute("devices", deviceService.getDevices());
        return ControllerConstants.TEMPLATE_DEVICES_VIEW;
    }
	
	@GetMapping("/device_{deviceId}")
    public String getDevice(@PathVariable(name = "deviceId") Long id, Model model) {
        model.addAttribute("device", deviceService.getDeviceById(id));
        return ControllerConstants.TEMPLATE_DEVICE_VIEW;
    }
}
