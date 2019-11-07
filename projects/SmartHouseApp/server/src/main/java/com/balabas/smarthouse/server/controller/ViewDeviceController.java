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
	
	@GetMapping("/")
    public String getRoot(Model model) {
        return "redirect:/index";
    }
	
	@GetMapping("/index")
    public String getDevicesIndex(Model model) {
		model.addAttribute("serverName", serverName);
		model.addAttribute("devices", deviceService.getDevices());
        return "devices/index.html";
    }

	@GetMapping("/device_{deviceId}")
    public String getDevice(@PathVariable(name = "deviceId") Long id, Model model) {
		model.addAttribute("serverName", serverName);
        model.addAttribute("device", deviceService.getDeviceById(id));
        model.addAttribute("sensors", deviceService.getEntitiesForDevice(id));
        model.addAttribute("holder", deviceService.getValueActionHolder(id));
        
        return "devices/device.html";
    }
	
	@GetMapping("/deleteDevice_{deviceId}")
    public String deleteDevice(@PathVariable(name = "deviceId") Long id, Model model) {
        deviceService.deleteDeviceById(id);
        
        return "redirect:/index";
    }
	
	@GetMapping("/executeAction_{deviceId}_{entityId}_{action}_")
	public String executeEntityAction(@PathVariable(name = "deviceId") Long deviceId,
			@PathVariable(name = "entityId") Long entityId, 
			@PathVariable(name = "action") String action, Model model) {
		
		deviceService.sendDataToDevice(deviceId, entityId, action);
		
		return "redirect:/device_" + Long.toString(deviceId);
		
	}
	
	@GetMapping("/alarms")
    public String getDevicesAlarms(Model model) {
		model.addAttribute("serverName", serverName);
        return "devices/alarms.html";
    }
	
	@GetMapping("/settings")
    public String getDevicesSettings(Model model) {
		model.addAttribute("serverName", serverName);
        return "devices/settings.html";
    }

}
