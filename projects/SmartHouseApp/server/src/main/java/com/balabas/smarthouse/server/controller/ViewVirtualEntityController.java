package com.balabas.smarthouse.server.controller;

import java.io.IOException;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;

import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.Group;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IGroup;
import com.balabas.smarthouse.server.entity.model.collectors.IVirtualEntityService;

@Controller
public class ViewVirtualEntityController {

	@Value("${smarthouse.server.name:#{null}}")
	private String serverName;

	@Autowired
	private IVirtualEntityService entityService;

	@GetMapping("/virtual/devices")
	public String getVirtualDevices(Model model) throws IOException {

		List<IDevice> devices = entityService.getDevices();
		
		model.addAttribute("devices", devices);
		return "virtual/devices.html";
	}
	
	@GetMapping(value = "/virtual/editDevice")
	public String editVirtualDevice(@RequestParam(name = "id", required = true) Long id,
			Model model) {

			IDevice dev = entityService.getDeviceById(id);
			if(dev==null) {
				dev = entityService.createDevice("Виртуальное устройство", "Виртуальное устройство");
			}
			
			List<IGroup> allGroups = entityService.getGroups();
			
			Map<Long, Long> groupIds = new HashMap<Long, Long>();
			
			if(dev.getGroups()!=null) {
				dev.getGroups().stream().forEach(group -> groupIds.put(group.getId(), group.getId()));
			}
			
			model.addAttribute("device", dev);
			model.addAttribute("groups", allGroups);
			model.addAttribute("groupIds", groupIds);
		
		return "virtual/device.html";
	}

	@PostMapping(value = "/virtual/saveDevice")
	public String saveVirtualDevice(@ModelAttribute("device") Device device,
			@RequestParam(value = "groups", required = false) long[] entityFieldIds, Model model) {

		Set<Group> groups = new HashSet<Group>();
		
		if(entityFieldIds!=null) {
			for(long id : entityFieldIds) {
				Group group = (Group) entityService.getGroupById(id);
				groups.add(group);
			}
		}
		
		if(device.getId()==null || device.getId().equals(0L)) {
			groups.stream().forEach(group -> group.setDevice(device));
			device.setGroups(groups);
			entityService.saveDevice(device);
		} else {
			Device dev = (Device) entityService.getDeviceById(device.getId());
			
			dev.setName(device.getName());
			dev.setDescription(device.getDescription());
			groups.stream().forEach(group -> group.setDevice(dev));
			dev.setGroups(groups);
			
			entityService.saveDevice(dev);
		}
		
		return "redirect:/virtual/devices";
	}
	
	@GetMapping("/virtual/groups")
	public String getVirtualGroups(Model model) throws IOException {

		List<IGroup> groups = entityService.getGroups();
		
		model.addAttribute("groups", groups);
		return "virtual/groups.html";
	}
	
	@GetMapping(value = "/virtual/editGroup")
	public String editVirtualGroup(@RequestParam(name = "id", required = true) Long id,
			Model model) {

			IGroup ent = entityService.getGroupById(id);
			
			if(ent==null) {
				ent = entityService.createGroup("sensors", "Виртуальная группа");
			} 
			if(ent.getDevice()!=null) {
				ent.setDeviceId(ent.getDevice().getId());
			}
			
			
			List<IEntity> all = entityService.getEntities();
			
			Map<Long, Long> ids = new HashMap<Long, Long>();
			
			if(ent.getEntities()!=null) {
				ent.getEntities().stream().forEach(e -> ids.put(e.getId(), e.getId()));
			}
			
			List<IDevice> devices = entityService.getDevices();
			
			Map<Long, Long> devicesIds = new HashMap<Long, Long>();
			
			if(ent.getDevice()!=null && ent.getDevice().getId()!=null) {
				devicesIds.put(ent.getDevice().getId(), ent.getDevice().getId());
			}
			
			model.addAttribute("group", ent);
			model.addAttribute("all", all);
			model.addAttribute("ids", ids);
			
			model.addAttribute("allDevices", devices);
			model.addAttribute("devicesIds", devicesIds);
		
		return "virtual/group.html";
	}
	

	@PostMapping(value = "/virtual/saveGroup")
	public String saveVirtualGroup(@ModelAttribute("group") Group group,
			@RequestParam(value = "entities", required = false) long[] entityFieldIds,
			@RequestParam(value = "devices", required = false) long[] devicesIds, Model model) {

		Set<Entity> entities = new HashSet<Entity>();
		
		if(entityFieldIds!=null) {
			for(long id : entityFieldIds) {
				Entity entity = (Entity) entityService.getEntityById(id);
				entities.add(entity);
			}
		}
		
		Long deviceId = group.getDeviceId();
		if(deviceId==null && devicesIds!=null && devicesIds.length>0) {
			deviceId = devicesIds[0];
		}
		
		IDevice idevice =  entityService.getDeviceById(deviceId);
		Device device = (idevice==null)?null:(Device)idevice; 
		
		if(group.getId()==null || group.getId().equals(0L)) {
			group.setDevice(device);
			entities.stream().forEach(entity -> entity.setGroup(group));
			entityService.saveDevice(device);
		} else {
			Group gr = (Group) entityService.getGroupById(group.getId());
			
			gr.setName(group.getName());
			gr.setDescription(group.getDescription());
			group.setDevice(device);
			
			entities.stream().forEach(entity -> entity.setGroup(group));
			entityService.saveDevice(device);
		}
		
		return "redirect:/virtual/groups";
	}

}
