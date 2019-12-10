package com.balabas.smarthouse.server.controller;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.util.StringUtils;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestParam;

import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.service.IDeviceManageService;

@Controller
public class ViewDeviceController {

	@Value("${smarthouse.server.name:#{null}}")
	private String serverName;

	@Autowired
	private IDeviceManageService deviceService;

	@GetMapping("/")
	public String getRoot(Model model) {
		return "redirect:/index";
	}

	@GetMapping("/index")
	public String getDevicesIndex(Model model) {
		model.addAttribute("serverName", serverName);
		model.addAttribute("devices", deviceService.getDevices());
		return "index.html";
	}

	@GetMapping("/device")
	public String getDevice(@RequestParam(name = "id", required = false) Long deviceId,
			@RequestParam(name = "name", required = false) String deviceName, Model model) {

		if (deviceId == null && StringUtils.isEmpty(deviceName)) {
			throw new IllegalArgumentException("Bad arguments");
		}

		Device device = (deviceId == null || deviceId == 0) ? deviceService.getDeviceByName(deviceName)
				: deviceService.getDeviceById(deviceId);

		if (device == null) {
			throw new IllegalArgumentException("Not found");
		}

		model.addAttribute("serverName", serverName);
		model.addAttribute("device", device);
		model.addAttribute("sensors", deviceService.getEntitiesForDevice(device.getId()));
		model.addAttribute("holder", deviceService.getValueActionHolder(device.getId()));

		return "devices/device.html";
	}

	@GetMapping("/deleteDevice_{deviceId}")
	public String deleteDevice(@PathVariable(name = "deviceId") Long id, Model model) {
		deviceService.deleteDeviceById(id);

		return "redirect:/index";
	}

	@GetMapping("/executeAction_{deviceId}_{entityId}_{action}_")
	public String executeEntityAction(@PathVariable(name = "deviceId") Long deviceId,
			@PathVariable(name = "entityId") Long entityId, @PathVariable(name = "action") String action, Model model) {

		deviceService.sendDataToDevice(deviceId, entityId, action);

		return "redirect:/device?id=" + Long.toString(deviceId);

	}

	@GetMapping("/settings")
	public String getDevicesSettings(Model model) {
		model.addAttribute("serverName", serverName);
		return "settings.html";
	}

}