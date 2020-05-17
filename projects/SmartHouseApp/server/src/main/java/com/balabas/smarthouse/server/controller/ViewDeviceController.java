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

import com.balabas.smarthouse.server.device.DeviceFile;
import com.balabas.smarthouse.server.device.DeviceFileManager;
import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.virtual.IVirtualEntityService;
import com.balabas.smarthouse.server.entity.service.IDeviceManageService;
import com.balabas.smarthouse.server.service.IServerManageService;
import com.balabas.smarthouse.server.util.DateTimeUtil;
import com.balabas.smarthouse.server.view.DeviceEntityFieldActionHolder;

import java.io.IOException;
import java.util.Collection;
import java.util.List;

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
	
	@Autowired
	private DeviceFileManager fileManager;
	
	@GetMapping("/")
	public String getRoot(Model model) {
		return URL_REDIRECT_PREFFIX + DEVICES_ROOT;
	}
	
	
	@GetMapping("/virtual/null/index")
	public String getIndexPageRedirect(Model model) {
		return "redirect:/";
	}
	
	
	@GetMapping("/virtual/null/manage")
	public String getManagePageRedirect(Model model) {
		return "redirect:/manage";
	}
	
	@GetMapping("/virtual/manage")
	public String getManagePageRedirect2(Model model) {
		return "redirect:/manage";
	}
	
	@GetMapping("/virtual/index")
	public String getManagePageRedirect3(Model model) {
		return "redirect:/index";
	}
	
	@GetMapping("/manage")
	public String getManagePage(Model model) {
		model.addAttribute(ATTR_SERVER_NAME, serverName);
		return "manage.html";
	}

	@GetMapping("/index")
	public String getDevicesIndex(Model model) {
		return getRoot(model);
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
		model.addAttribute(ATTR_PAGE_TITLE, device.getEmojiDescriptionByDescriptionField());
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
	
	@GetMapping("/fileManager")
	public String getDeviceFiles(@RequestParam(name="id") Long id, Model model) {
		
		IDevice device = deviceService.getDeviceById(id);
		
		List<DeviceFile> files = fileManager.getDeviceFiles(device);
		
		DeviceEntityFieldActionHolder holder = deviceService.getValueActionHolder(device.getId());
		
		model.addAttribute("device", device);
		model.addAttribute("holder", holder);
		model.addAttribute("files", files);
		
		return "devices/deviceFiles.html";
	}
	
	@PostMapping("/fileManager")
	public String postDeviceFiles(@RequestParam(name="id") Long id, @RequestParam(name="fileName") String fileName, Model model) throws IOException {
		
		fileManager.postFile(deviceService.getDeviceById(id), fileManager.getFromStorage(fileName), fileName);		
		return "redirect:/fileManager?id="+id;
	}
	
	@GetMapping("/deleteManager")
	public String deleteDeviceFiles(@RequestParam(name="id") Long id, @RequestParam(name="fileName") String fileName, Model model) {
		
		IDevice device = deviceService.getDeviceById(id);
		fileManager.deleteFile(device, fileName);
		
		return "redirect:/fileManager?id="+id;
	}

	@GetMapping("/settings")
	public String getDevicesSettings(Model model) {
		model.addAttribute("serverName", serverName);
		return "settings.html";
	}
	
	@PostMapping("/restartServer") 
	public String restartServer(Model model){
		serverManageService.restartServer(5);
		return "restartServer.html";
	}
	
	private void addRefreshIntervalToModel(Model model) {
		if (deviceViewRefreshInterval != null && deviceViewRefreshInterval > 0) {
			model.addAttribute(ControllerConstants.ATTR_PAGE_REFRESH_INTERVAL, deviceViewRefreshInterval);
		}
	}
	
}

