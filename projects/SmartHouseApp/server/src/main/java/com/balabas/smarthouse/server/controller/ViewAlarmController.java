package com.balabas.smarthouse.server.controller;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.util.StringUtils;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestParam;

import com.balabas.smarthouse.server.entity.alarm.IEntityAlarm;
import com.balabas.smarthouse.server.entity.alarm.IEntityAlarmService;
import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.service.IDeviceManageService;
import com.balabas.smarthouse.server.view.DeviceEntityAlarmHolder;

@Controller
public class ViewAlarmController {

	@Value("${smarthouse.server.name:#{null}}")
	private String serverName;

	@Autowired
	private IDeviceManageService deviceService;

	@Autowired
	private IEntityAlarmService alarmService;

	@GetMapping("/alarms")
	public String getDevicesAlarms(Model model) {
		model.addAttribute("serverName", serverName);
		List<Device> devices = deviceService.getDevices();

		model.addAttribute("devices", devices);
		model.addAttribute("alarms", alarmService.getDeviceAlarmsHoldersGroupped(devices));

		return "alarms.html";
	}

	@GetMapping("/alarmEdit")
	public String getDevice(@RequestParam(name = "id", required = true) Long deviceId, Model model) {

		IDevice device = deviceService.getDeviceById(deviceId);
		
		if (device == null) {
			throw new IllegalArgumentException("Not found");
		}
		
		List<IEntityAlarm> alarms = alarmService.getAlarms(device);
		DeviceEntityAlarmHolder holder = alarmService.getDeviceAlarmsHolder(device);
		
		//key entityFieldId
		Map<Long, Long> attachedFieldAlarms = new HashMap<>();
		//key entityId
		Map<Long, List<IEntityField>> possibleFieldAlarms = new HashMap<>();
		
		alarms.stream().flatMap( entityAlarm -> entityAlarm.getAlarms().stream()).forEach(efa -> attachedFieldAlarms.put(efa.getWatchedItem().getId(), efa.getId()));
		
		for(IEntity entity : device.getEntities()) {
			List<IEntityField> possibleFields = alarmService.getEntityFieldsWithPossibleAlarms(entity);
			
			if(!possibleFieldAlarms.containsKey(entity.getId())) {
				possibleFieldAlarms.put(entity.getId(), new ArrayList<>());
			}
			
			List<IEntityField> enabledAndNotPresented = possibleFieldAlarms.get(entity.getId());
			
			possibleFields.stream().forEach( ef -> {
				if(!attachedFieldAlarms.containsKey(ef.getId())) {
					enabledAndNotPresented.add(ef);
				}
			});
		}

		model.addAttribute("serverName", serverName);
		model.addAttribute("device", device);
		model.addAttribute("alarms", alarms);
		model.addAttribute("possibleFieldAlarms", possibleFieldAlarms);
		model.addAttribute("holder", holder);

		return "devices/deviceAlarms.html";
	}

}
