package com.balabas.smarthouse.server.controller;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;

import com.balabas.smarthouse.server.entity.service.DeviceManageService;

@Controller
public class ViewDeviceController {
		
	@Value("${smarthouse.server.name:#{null}}")
	private String serverName;
	
	@Autowired
	private DeviceManageService deviceService;
	
	@GetMapping("/index")
    public String getDevicesIndex(Model model) {
		model.addAttribute("serverName", serverName);
		model.addAttribute("devices", deviceService.getDevices());
        return "devices/index.html";
    }

	@GetMapping("/alarms")
    public String getDevicesalarms(Model model) {
        return "devices/alarms.html";
    }
	
	@GetMapping("/settings")
    public String getDevicesSettings(Model model) {
        return "devices/settings.html";
    }
	
	@GetMapping("/device_{deviceId}")
    public String getDevice(@PathVariable(name = "deviceId") Long id, Model model) {
		model.addAttribute("serverName", serverName);
        model.addAttribute("device", deviceService.getDeviceById(id));
        model.addAttribute("sensors", deviceService.getEntitiesForDevice(id));
        model.addAttribute("values", deviceService.getLastEntityFieldValuesForDeviceGroupped(id));
        
        return "devices/device.html";
    }
	
	@GetMapping("/deleteDevice_{deviceId}")
    public String deleteDevice(@PathVariable(name = "deviceId") Long id, Model model) {
        deviceService.deleteDeviceById(id);
        
        return "redirect:/index";
    }
}
