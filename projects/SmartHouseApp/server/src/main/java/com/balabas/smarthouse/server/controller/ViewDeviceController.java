package com.balabas.smarthouse.server.controller;

import java.util.List;

import org.json.JSONArray;
import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.util.StringUtils;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestParam;

import com.balabas.smarthouse.server.entity.alarm.IEntityAlarmService;
import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.model.EntityFieldValue;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.service.IDeviceManageService;
import com.balabas.smarthouse.server.entity.service.IEntityFieldService;

@Controller
@SuppressWarnings("rawtypes")
public class ViewDeviceController {

	@Value("${smarthouse.server.name:#{null}}")
	private String serverName;

	@Autowired
	private IDeviceManageService deviceService;

	@Autowired
	private IEntityAlarmService alarmService;
	
	@Autowired
	private IEntityFieldService entityFieldService;

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

	@GetMapping("/entityField")
	public String getEntityField(@RequestParam(name = "id", required = false) Long entityFieldId, Model model) {

		if (entityFieldId==null || entityFieldId==0) {
			throw new IllegalArgumentException("Bad arguments");
		}

		IEntityField entityField = deviceService.getDevices().stream().flatMap(device -> device.getEntities().stream())
				.flatMap( entity -> entity.getEntityFields().stream()).filter(ef -> ef.getId().equals(entityFieldId)).findFirst().orElse(null);
		
		if (entityField==null) {
			throw new IllegalArgumentException("Bad arguments");
		}
		
		IDevice device = entityField.getEntity().getDevice();

		List<EntityFieldValue> values = entityFieldService.getEntityFieldValuesForEntityField(entityField.getId());
		
		JSONArray jsonData = new JSONArray();
		
		for(EntityFieldValue value: values) {
			JSONObject obj = new JSONObject().put("t", value.getDate().getTime()).put("y", value.getValue());
			jsonData.put(obj);
		}
		
		String jsonStr = jsonData.toString();
		
		model.addAttribute("serverName", serverName);
		model.addAttribute("device", device);
		model.addAttribute("entityField", entityField);
		model.addAttribute("values", values);
		
		model.addAttribute("chartDataHeader", entityField.getEmoji() + " " + entityField.getDescriptionByDescriptionField());
		model.addAttribute("chartDataY", entityField.getName());
		model.addAttribute("chartData", jsonStr);

		return "entityFields/history.html";
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

	@GetMapping("/alarms")
	public String getDevicesAlarms(Model model) {
		model.addAttribute("serverName", serverName);
		List<Device> devices = deviceService.getDevices();

		model.addAttribute("devices", devices);
		model.addAttribute("alarms", alarmService.getDeviceAlarmsHoldersGroupped(devices));

		return "devices/alarms.html";
	}

	@GetMapping("/settings")
	public String getDevicesSettings(Model model) {
		model.addAttribute("serverName", serverName);
		return "devices/settings.html";
	}

}
