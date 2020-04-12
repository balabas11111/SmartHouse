package com.balabas.smarthouse.server.controller;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.util.StringUtils;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;

import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.virtual.IVirtualEntityService;
import com.balabas.smarthouse.server.entity.service.IDeviceManageService;
import com.balabas.smarthouse.server.service.IServerManageService;
import com.balabas.smarthouse.server.util.DateTimeUtil;
import com.balabas.smarthouse.server.view.DeviceEntityFieldActionHolder;

import java.util.Collection;

import static com.balabas.smarthouse.server.controller.ControllerConstants.ATTR_SERVER_NAME;
import static com.balabas.smarthouse.server.controller.ControllerConstants.URL_REDIRECT_PREFFIX;

import static com.balabas.smarthouse.server.controller.ControllerConstants.DEVICES_ROOT;

import static com.balabas.smarthouse.server.controller.ControllerConstants.ATTR_PAGE_TITLE;
import static com.balabas.smarthouse.server.controller.ControllerConstants.ATTR_PAGE_URL;

import static com.balabas.smarthouse.server.controller.ControllerConstants.ATTR_DEVICES;
import static com.balabas.smarthouse.server.controller.ControllerConstants.ATTR_VIRTUAL;
import static com.balabas.smarthouse.server.controller.ControllerConstants.ATTR_PAGE_DATETIME;

@Controller
public class ViewDeviceController {
	
	private static final String DEVICES_HTML = "devices/devices.html";
	private static final String DEVICE_HTML = "devices/device.html";

	@Value("${smarthouse.server.name:#{null}}")
	private String serverName;
	
	@Value("${smarthouse.server.view.page.device.refresh.interval.sec:60}")
	private Long deviceViewRefreshInterval;

	@Autowired
	private IDeviceManageService deviceService;
	
	@Autowired
	private IVirtualEntityService virtualDeviceService;
	
	@Autowired
	private IServerManageService serverManageService;

	@GetMapping("/")
	public String getRoot(Model model) {
		return URL_REDIRECT_PREFFIX + DEVICES_ROOT;
	}
	
	@GetMapping("/manage")
	public String getManagePage(Model model) {
		return "manage.html";
	}

	@GetMapping("/index")
	public String getDevicesIndex(Model model) {
		model.addAttribute(ATTR_SERVER_NAME, serverName);
		model.addAttribute("realDevices", deviceService.getDevicesNotVirtualized());
		model.addAttribute("virtualDevices", virtualDeviceService.getDevices());
		
		return "index.html";
	}
	
	@GetMapping(DEVICES_ROOT)
	public String getDevices(@RequestParam(name = "v", required = false) Boolean virtual, Model model) {
		if(virtual==null) {
			virtual=true;
		}
		model.addAttribute(ATTR_SERVER_NAME, serverName);
		model.addAttribute(ATTR_PAGE_TITLE, virtual ? "Гаджеты сервера":"Устройства сервера");
		model.addAttribute(ATTR_PAGE_URL, DEVICES_ROOT);
		
		addRefreshIntervalToModel(model);
		
		Collection<IDevice> devices = virtual ? virtualDeviceService.getDevices() : deviceService.getDevicesNotVirtualized();
		
		model.addAttribute(ATTR_DEVICES, devices);
		model.addAttribute(ATTR_VIRTUAL, virtual);
		
		return DEVICES_HTML;
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

		addRefreshIntervalToModel(model);
		
		DeviceEntityFieldActionHolder holder = deviceService.getValueActionHolder(device.getId()); 
		
		model.addAttribute(ATTR_SERVER_NAME, serverName);
		model.addAttribute(ATTR_PAGE_TITLE, (device.isVirtualized() ? "Гаджет ":"Устройство ") + device.getEmojiDescriptionByDescriptionField());
		model.addAttribute(ATTR_PAGE_URL, DEVICES_ROOT + "?v="+device.isVirtualized());
		model.addAttribute(ATTR_PAGE_DATETIME, DateTimeUtil.getDateTimeStr());
		model.addAttribute("holder", holder);

		return DEVICE_HTML;
	}

	@GetMapping("/deleteDevice_{deviceId}")
	public String deleteDevice(@PathVariable(name = "deviceId") Long id, Model model) {
		deviceService.deleteDeviceById(id);

		return getRoot(model) + "?v=true";
	}

	
	@GetMapping("/executeAction_{deviceId}_{entityId}_{action}_{entityFieldId}_")
	public String executeEntityAction(@PathVariable(name = "deviceId") Long deviceId,
			@PathVariable(name = "entityId") Long entityId,
			@PathVariable(name = "action") String action,
			@PathVariable(name = "entityFieldId") Long entityFieldId,Model model) {

		deviceService.sendActionToDevice(deviceId, entityId, entityFieldId, action);

		return "redirect:/device?id=" + Long.toString(deviceId);

	}

	@GetMapping("/settings")
	public String getDevicesSettings(Model model) {
		model.addAttribute("serverName", serverName);
		return "settings.html";
	}
	
	@PostMapping("/restartServer") 
	public String restartServer(Model model){
		serverManageService.restartServer(5);
		return "redirect:/manage";
	}
	
	private void addRefreshIntervalToModel(Model model) {
		if (deviceViewRefreshInterval != null && deviceViewRefreshInterval > 0) {
			model.addAttribute(ControllerConstants.ATTR_PAGE_REFRESH_INTERVAL, deviceViewRefreshInterval);
		}
	}
	
}

