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
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;

import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.Group;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IGroup;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.collectors.IVirtualEntityService;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityField;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

import static com.balabas.smarthouse.server.entity.model.collectors.VirtualEntityService.VIRTUAL_DEVICE_NAME;
import static com.balabas.smarthouse.server.entity.model.collectors.VirtualEntityService.VIRTUAL_DEVICE_DESCR;
import static com.balabas.smarthouse.server.entity.model.collectors.VirtualEntityService.VIRTUAL_DEVICE_FIRMWARE;

import static com.balabas.smarthouse.server.entity.model.collectors.VirtualEntityService.VIRTUAL_GROUP_NAME;
import static com.balabas.smarthouse.server.entity.model.collectors.VirtualEntityService.VIRTUAL_GROUP_DESCR;

import static com.balabas.smarthouse.server.entity.model.collectors.VirtualEntityService.VIRTUAL_ENTITY_NAME;
import static com.balabas.smarthouse.server.entity.model.collectors.VirtualEntityService.VIRTUAL_ENTITY_DESCR;

import static com.balabas.smarthouse.server.entity.model.collectors.VirtualEntityService.VIRTUAL_ENTITY_FIELD_NAME;
import static com.balabas.smarthouse.server.entity.model.collectors.VirtualEntityService.VIRTUAL_ENTITY_FIELD_DESCR;

@Controller
@RequestMapping(value = "/virtual")
@SuppressWarnings("rawtypes")
public class ViewVirtualEntityController {

	@Value("${smarthouse.server.name:#{null}}")
	private String serverName;

	@Autowired
	private IVirtualEntityService entityService;

	@GetMapping("/devices")
	public String getVirtualDevices(Model model) throws IOException {

		List<IDevice> devices = entityService.getDevices();

		model.addAttribute("devices", devices);
		return "virtual/devices.html";
	}

	@GetMapping(value = "/editDevice")
	public String editVirtualDevice(@RequestParam(name = "id", required = true) Long id, Model model) {

		IDevice dev = entityService.getDeviceById(id);
		if (dev == null) {
			dev = entityService.createDevice(VIRTUAL_DEVICE_NAME, VIRTUAL_DEVICE_DESCR, VIRTUAL_DEVICE_FIRMWARE);
		}

		List<IGroup> allGroups = entityService.getGroups();

		Map<Long, Long> groupIds = getEntitiesMap(dev.getGroups());

		model.addAttribute("device", dev);
		model.addAttribute("groups", allGroups);
		model.addAttribute("groupIds", groupIds);

		return "virtual/device.html";
	}

	@PostMapping(value = "/saveDevice")
	public String saveVirtualDevice(@ModelAttribute("device") Device device,
			@RequestParam(value = "groups", required = false) long[] entityFieldIds, Model model) {

		Set<Group> groups = new HashSet<Group>();

		if (entityFieldIds != null) {
			for (long id : entityFieldIds) {
				Group group = (Group) entityService.getGroupById(id);
				groups.add(group);
			}
		}

		if (device.getId() == null || device.getId().equals(0L)) {
			groups.stream().forEach(group -> group.setDevice(device));
			device.setGroups(groups);
			entityService.save(device);
		} else {
			Device dev = (Device) entityService.getDeviceById(device.getId());

			dev.setName(device.getName());
			dev.setDescription(device.getDescription());
			groups.stream().forEach(group -> group.setDevice(dev));
			dev.setGroups(groups);

			entityService.save(dev);
		}

		return "redirect:/virtual/devices";
	}

	@GetMapping("/groups")
	public String getVirtualGroups(Model model) throws IOException {

		List<IGroup> groups = entityService.getGroups();

		model.addAttribute("groups", groups);
		return "virtual/groups.html";
	}

	@GetMapping(value = "/editGroup")
	public String editVirtualGroup(@RequestParam(name = "id", required = true) Long id, Model model) {

		IGroup group = entityService.getGroupById(id);

		if (group == null) {
			group = entityService.createGroup(VIRTUAL_GROUP_NAME, VIRTUAL_GROUP_DESCR);
		}

		Long deviceId = getItemId(group.getDevice());

		List<IEntity> allEntities = entityService.getEntities();

		Map<Long, Long> ids = getEntitiesMap(group.getEntities());

		List<IDevice> devices = entityService.getDevices();

		model.addAttribute("group", group);
		model.addAttribute("entities", allEntities);
		model.addAttribute("selectedEntities", ids);

		model.addAttribute("allDevices", devices);
		model.addAttribute("deviceId", deviceId);

		return "virtual/group.html";
	}

	@PostMapping(value = "/saveGroup")
	public String saveVirtualGroup(@ModelAttribute("group") Group group,
			@RequestParam(value = "entities", required = false) long[] entityIds,
			@RequestParam(value = "devices", required = false) long[] devicesIds, Model model) {

		Set<Entity> entities = new HashSet<Entity>();

		if (entityIds != null) {
			for (long id : entityIds) {
				Entity entity = (Entity) entityService.getEntityById(id);
				entities.add(entity);
			}
		}

		Long deviceId = getFirstIdOrNull(devicesIds);

		IDevice iDevice = entityService.getDeviceById(deviceId);
		Device device = (iDevice == null) ? null : (Device) iDevice;

		if (group.getId() == null || group.getId().equals(0L)) {
			group.setDevice(device);
			entities.stream().forEach(entity -> entity.setGroup(group));
			group.setEntities(entities);
			entityService.save(group);
		} else {
			Group gr = (Group) entityService.getGroupById(group.getId());

			gr.setName(group.getName());
			gr.setDescription(group.getDescription());
			gr.setDevice(device);

			entities.stream().forEach(entity -> entity.setGroup(gr));
			gr.setEntities(entities);
			
			entityService.save(gr);
		}

		return "redirect:/virtual/groups";
	}

	@GetMapping("/entities")
	public String getVirtualEntities(Model model) throws IOException {

		List<IEntity> entities = entityService.getEntities();

		model.addAttribute("entities", entities);
		return "virtual/entities.html";
	}

	@GetMapping(value = "/editEntity")
	public String editVirtualEntities(@RequestParam(name = "id", required = true) Long id, Model model) {

		IEntity entity = entityService.getEntityById(id);

		if (entity == null) {
			entity = entityService.createEntity(VIRTUAL_ENTITY_NAME, VIRTUAL_ENTITY_DESCR);
		}

		Long groupId = getItemId(entity.getGroup());

		List<IEntityField> allEntityFields = entityService.getEntityFields();

		Map<Long, Long> ids = getEntitiesMap(entity.getEntityFields());

		List<IGroup> groups = entityService.getGroups();

		model.addAttribute("entity", entity);
		model.addAttribute("entityFields", allEntityFields);
		model.addAttribute("selectedEntityFields", ids);

		model.addAttribute("allGroups", groups);
		model.addAttribute("groupId", groupId);

		return "virtual/entity.html";
	}

	@PostMapping(value = "/saveEntity")
	public String saveVirtualEntities(@ModelAttribute("entity") Entity entity,
			@RequestParam(value = "entityFields", required = false) long[] entityFieldIds,
			@RequestParam(value = "groups", required = false) long[] groupIds, Model model) {

		Set<IEntityField> entityFields = new HashSet<IEntityField>();

		if (entityFieldIds != null) {
			for (long id : entityFieldIds) {
				EntityField ef = (EntityField) entityService.getEntityFieldById(id);
				entityFields.add(ef);
			}
		}

		Long groupId = getFirstIdOrNull(groupIds);

		IGroup iGroup = entityService.getGroupById(groupId);
		Group group = (iGroup == null) ? null : (Group) iGroup;

		if (entity.getId() == null || entity.getId().equals(0L)) {
			entity.setGroup(group);
			entityFields.stream().forEach(ef -> ef.setEntity(entity));
			entity.setEntityFields(entityFields);
			
			entityService.save(entity);
		} else {
			Entity ent = (Entity) entityService.getEntityById(entity.getId());

			ent.setName(entity.getName());
			ent.setDescription(entity.getDescription());
			
			entityFields.stream().forEach(ef -> ef.setEntity(ent));
			ent.setEntityFields(entityFields);
			
			entityService.save(ent);
		}

		return "redirect:/virtual/entities";
	}
	
	@GetMapping("/entityFields")
	public String getVirtualEntityFields(Model model) throws IOException {

		List<IEntityField> entityFields = entityService.getEntityFields();

		model.addAttribute("entityFields", entityFields);
		return "virtual/entityFields.html";
	}

	@GetMapping(value = "/editEntityField")
	public String editVirtualEntityFields(@RequestParam(name = "id", required = true) Long id, Model model) {

		IEntityField entityField = entityService.getEntityFieldById(id);

		if (entityField == null) {
			entityField = entityService.createEntityFieldFloat(VIRTUAL_ENTITY_FIELD_NAME, VIRTUAL_ENTITY_FIELD_DESCR);
		}

		Long entityId = getItemId(entityField.getEntity());

		List<IEntity> entities = entityService.getEntities();

		model.addAttribute("entityField", entityField);

		model.addAttribute("allEntities", entities);
		model.addAttribute("entityId", entityId);

		return "virtual/entityField.html";
	}

	@PostMapping(value = "/saveEntityField")
	public String saveVirtualEntityField(@ModelAttribute("entityField") EntityField entityField,
			@RequestParam(value = "entities", required = false) long[] entityIds, Model model) {

		Long entityId = getFirstIdOrNull(entityIds);

		IEntity iEntity = entityService.getEntityById(entityId);
		Entity entity = (iEntity == null) ? null : (Entity) iEntity;

		if (entityField.getId() == null || entityField.getId().equals(0L)) {
			entityField.setEntity(entity);
			
			entityService.save(entityField);
		} else {
			EntityField ent = (EntityField) entityService.getEntityFieldById(entityField.getId());

			ent.setName(entityField.getName());
			ent.setDescription(entityField.getDescription());
			
			ent.setEntity(entity);
			
			entityService.save(ent);
		}

		return "redirect:/virtual/entityFields";
	}
	
	private Map<Long, Long> getEntitiesMap(Set<? extends IItemAbstract> items) {
		Map<Long, Long> ids = new HashMap<Long, Long>();

		if (items != null) {
			items.stream().forEach(e -> ids.put(e.getId(), e.getId()));
		}
		
		return ids;
	}
	
	private <T extends IItemAbstract> Long getItemId(T item) {
		return item != null ? item.getId() : null;
	}
	
	private Long getFirstIdOrNull(long[] ids) {
		Long result = null;
		if (ids != null && ids.length > 0) {
			result = ids[0];
		}
		return result;
	}
}
