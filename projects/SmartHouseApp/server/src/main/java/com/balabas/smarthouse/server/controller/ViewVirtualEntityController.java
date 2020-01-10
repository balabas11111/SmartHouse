package com.balabas.smarthouse.server.controller;

import java.io.IOException;
import java.util.Collection;
import java.util.Date;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Optional;
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
import com.balabas.smarthouse.server.entity.model.ItemAbstract;
import com.balabas.smarthouse.server.entity.model.descriptor.State;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityField;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.model.virtual.CalculatedEntityField;
import com.balabas.smarthouse.server.entity.model.virtual.ICalculatedEntityField;
import com.balabas.smarthouse.server.entity.model.virtual.ICalculatedEntityFieldCalculator;
import com.balabas.smarthouse.server.entity.model.virtual.ICalculatedEntityFieldService;
import com.balabas.smarthouse.server.entity.model.virtual.IVirtualEntityService;

import static com.balabas.smarthouse.server.controller.ControllerConstants.PROP_BASE_URL;
import static com.balabas.smarthouse.server.controller.ControllerConstants.PROP_PAGE_HEADER;
import static com.balabas.smarthouse.server.entity.model.virtual.VirtualEntityService.VIRTUAL_DEVICE_DESCR;
import static com.balabas.smarthouse.server.entity.model.virtual.VirtualEntityService.VIRTUAL_DEVICE_FIRMWARE;
import static com.balabas.smarthouse.server.entity.model.virtual.VirtualEntityService.VIRTUAL_DEVICE_NAME;
import static com.balabas.smarthouse.server.entity.model.virtual.VirtualEntityService.VIRTUAL_ENTITY_DESCR;
import static com.balabas.smarthouse.server.entity.model.virtual.VirtualEntityService.VIRTUAL_ENTITY_FIELD_DESCR;
import static com.balabas.smarthouse.server.entity.model.virtual.VirtualEntityService.VIRTUAL_ENTITY_FIELD_NAME;
import static com.balabas.smarthouse.server.entity.model.virtual.VirtualEntityService.VIRTUAL_ENTITY_NAME;
import static com.balabas.smarthouse.server.entity.model.virtual.VirtualEntityService.VIRTUAL_GROUP_DESCR;
import static com.balabas.smarthouse.server.entity.model.virtual.VirtualEntityService.VIRTUAL_GROUP_NAME;

@Controller
@RequestMapping(value = "/virtual")
@SuppressWarnings({ "rawtypes", "unchecked" })
public class ViewVirtualEntityController extends BaseController {

	@Value("${smarthouse.server.name:#{null}}")
	private String serverName;

	@Autowired
	private IVirtualEntityService entityService;
	
	@Autowired
	private ICalculatedEntityFieldService calculatedEntityFieldService;
	

	@GetMapping("/devices")
	public String getVirtualDevices(Model model) throws IOException {

		List<IDevice> devices = entityService.getDevices();

		model.addAttribute("devices", devices);
		model.addAttribute(PROP_BASE_URL, getBaseUrl());
		return "virtual/devices.html";
	}

	@GetMapping(value = "/editDevice")
	public String editVirtualDevice(@RequestParam(name = "id", required = true) Long id, Model model) {

		IDevice dev = entityService.getDeviceById(id);
		String preffix = ControllerConstants.MSG_EDIT_DEVICE;
		if (dev == null) {
			preffix = ControllerConstants.MSG_EDIT_DEVICE;
			dev = entityService.createDevice(getVirtualName(VIRTUAL_DEVICE_NAME), VIRTUAL_DEVICE_DESCR,
					VIRTUAL_DEVICE_FIRMWARE);
		}

		List<IGroup> allGroups = entityService.getGroups();

		Map<Long, Long> groupIds = getEntitiesMap(dev.getGroups());

		model.addAttribute("device", dev);
		model.addAttribute("groups", allGroups);
		model.addAttribute("groupIds", groupIds);
		model.addAttribute(PROP_PAGE_HEADER, getPageHeader(preffix, dev));
		model.addAttribute(PROP_BASE_URL, getBaseUrl());

		return "virtual/device.html";
	}

	@PostMapping(value = "/saveDevice")
	public String saveVirtualDevice(@ModelAttribute("device") Device device,
			@RequestParam(value = "groups", required = false) long[] groupIds, Model model) {

		Map<Long, Group> groups = new HashMap<Long, Group>();

		if (groupIds != null) {
			for (long id : groupIds) {
				Group group = (Group) entityService.getGroupById(id);
				if (group != null) {
					groups.put(id, group);
				}
			}
		}

		if (ItemAbstract.isNew(device)) {
			groups.values().stream().forEach(group -> group.setDevice(device));
			device.setGroups(new HashSet(groups.values()));
			device.setState(State.CONNECTED);
			entityService.save(device);
		} else {
			Device dev = (Device) entityService.getDeviceById(device.getId());
			Set<Group> groupSet = new HashSet(addUnexisting(groups, dev.getGroups()).values());

			dev.setNameDescriptionEmoji(device);
			setParentOrNull(groupIds, groupSet, dev, null);

			dev.setGroups(groupSet);
			dev.setState(State.CONNECTED);

			entityService.save(dev);
		}

		return "redirect:/virtual/devices";
	}

	@GetMapping("/groups")
	public String getVirtualGroups(Model model) throws IOException {

		List<IGroup> groups = entityService.getGroups();

		model.addAttribute("groups", groups);
		model.addAttribute(PROP_BASE_URL, getBaseUrl());

		return "virtual/groups.html";
	}

	@GetMapping(value = "/editGroup")
	public String editVirtualGroup(@RequestParam(name = "id", required = true) Long id, Model model) {

		IGroup group = entityService.getGroupById(id);
		String preffix = ControllerConstants.MSG_EDIT_GROUP;
		if (group == null) {
			preffix = ControllerConstants.MSG_NEW_GROUP;
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
		model.addAttribute(PROP_PAGE_HEADER, getPageHeader(preffix, group));
		model.addAttribute(PROP_BASE_URL, getBaseUrl());

		return "virtual/group.html";
	}

	@PostMapping(value = "/saveGroup")
	public String saveVirtualGroup(@ModelAttribute("group") Group group,
			@RequestParam(value = "entities", required = false) long[] entityIds,
			@RequestParam(value = "devices", required = false) long[] devicesIds, Model model) {

		Map<Long, Entity> entities = new HashMap<Long, Entity>();

		if (entityIds != null) {
			for (long id : entityIds) {
				Entity entity = (Entity) entityService.getEntityById(id);
				if (entity != null) {
					entities.put(id, entity);
				}
			}
		}

		Long deviceId = getFirstIdOrNull(devicesIds);

		IDevice iDevice = entityService.getDeviceById(deviceId);
		Device device = (iDevice == null) ? null : (Device) iDevice;

		if (ItemAbstract.isNew(group)) {
			group.setDevice(device);
			entities.values().stream().forEach(entity -> entity.setGroup(group));
			group.setEntities(new HashSet(entities.values()));
			entityService.save(group);
		} else {
			Group gr = (Group) entityService.getGroupById(group.getId());
			Set<Entity> entitiesSet = new HashSet(addUnexisting(entities, gr.getEntities()).values());
			gr.setNameDescriptionEmoji(group);
			setParentOrNull(entityIds, entitiesSet, gr, device);

			gr.setEntities(entitiesSet);

			entityService.save(gr);
		}

		return "redirect:/virtual/groups";
	}

	@GetMapping("/entities")
	public String getVirtualEntities(Model model) throws IOException {

		List<IEntity> entities = entityService.getEntities();

		model.addAttribute("entities", entities);
		model.addAttribute(PROP_BASE_URL, getBaseUrl());
		return "virtual/entities.html";
	}

	@GetMapping(value = "/editEntity")
	public String editVirtualEntities(@RequestParam(name = "id", required = true) Long id, Model model) {

		IEntity entity = entityService.getEntityById(id);
		String preffix = ControllerConstants.MSG_EDIT_ENTITY;
		if (entity == null) {
			preffix = ControllerConstants.MSG_NEW_ENTITY;
			entity = entityService.createEntity(getVirtualName(VIRTUAL_ENTITY_NAME), VIRTUAL_ENTITY_DESCR);
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
		model.addAttribute(PROP_PAGE_HEADER, getPageHeader(preffix, entity));
		model.addAttribute(PROP_BASE_URL, getBaseUrl());

		return "virtual/entity.html";
	}

	@PostMapping(value = "/saveEntity")
	public String saveVirtualEntities(@ModelAttribute("entity") Entity entity,
			@RequestParam(value = "entityFields", required = false) long[] entityFieldIds,
			@RequestParam(value = "groups", required = false) long[] groupIds, Model model) {

		Map<Long, IEntityField> entityFields = new HashMap<Long, IEntityField>();

		if (entityFieldIds != null) {
			for (long id : entityFieldIds) {
				EntityField ef = (EntityField) entityService.getEntityFieldById(id);
				if (ef != null) {
					entityFields.put(id, ef);
				}
			}
		}

		Long groupId = getFirstIdOrNull(groupIds);

		IGroup iGroup = entityService.getGroupById(groupId);
		Group group = (iGroup == null) ? null : (Group) iGroup;

		if (ItemAbstract.isNew(entity)) {
			entity.setGroup(group);
			entityFields.values().stream().forEach(ef -> ef.setEntity(entity));
			entity.setEntityFields(new HashSet(entityFields.values()));

			entityService.save(entity);
		} else {
			Entity ent = (Entity) entityService.getEntityById(entity.getId());

			Set<IEntityField> entityfieldsSet = new HashSet(
					addUnexisting(entityFields, ent.getEntityFields()).values());

			ent.setNameDescriptionEmoji(entity);
			setParentOrNull(entityFieldIds, entityfieldsSet, ent, group);

			ent.setEntityFields(entityfieldsSet);
			entityService.save(ent);
		}

		return "redirect:/virtual/entities";
	}

	@GetMapping("/entityFields")
	public String getVirtualEntityFields(Model model) throws IOException {

		List<IEntityField> entityFields = entityService.getEntityFields();

		model.addAttribute("entityFields", entityFields);
		model.addAttribute(PROP_BASE_URL, getBaseUrl());

		return "virtual/entityFields.html";
	}

	@GetMapping(value = "/editEntityField")
	public String editVirtualEntityFields(@RequestParam(name = "id", required = true) Long id, Model model) {

		IEntityField entityField = entityService.getEntityFieldById(id);
		String preffix = ControllerConstants.MSG_EDIT_ENTITY_FIELD;
		if (entityField == null) {
			preffix = ControllerConstants.MSG_NEW_ENTITY_FIELD;
			entityField = entityService.createEntityFieldFloat(getVirtualName(VIRTUAL_ENTITY_FIELD_NAME),
					VIRTUAL_ENTITY_FIELD_DESCR);
		}

		Long entityId = getItemId(entityField.getEntity());

		List<IEntity> entities = entityService.getEntities();

		model.addAttribute("entityField", entityField);

		model.addAttribute("allEntities", entities);
		model.addAttribute("entityId", entityId);
		model.addAttribute(PROP_PAGE_HEADER, getPageHeader(preffix, entityField));
		model.addAttribute(PROP_BASE_URL, getBaseUrl());

		return "virtual/entityField.html";
	}

	@PostMapping(value = "/saveEntityField")
	public String saveVirtualEntityField(@ModelAttribute("entityField") EntityField entityField,
			@RequestParam(value = "entities", required = false) long[] entityIds, Model model) {

		Long entityId = getFirstIdOrNull(entityIds);

		IEntity iEntity = entityService.getEntityById(entityId);
		Entity entity = (iEntity == null) ? null : (Entity) iEntity;

		if (ItemAbstract.isNew(entityField)) {
			entityField.setEntity(entity);

			entityService.save(entityField);
		} else {
			EntityField ef = (EntityField) entityService.getEntityFieldById(entityField.getId());
			ef.setNameDescriptionEmoji(entityField);

			ef.setEntity(entity);

			entityService.save(ef);
		}

		return "redirect:/virtual/entityFields";
	}

	@GetMapping("/calculatedEntityFields")
	public String getCalculatedEntityFields(Model model) {

		model.addAttribute("fields", calculatedEntityFieldService.getCalculatedEntityFields());
		
		return "virtual/calculatedEntityFields.html";
	}

	@GetMapping("/editCalculatedEntityField")
	public String getCalculatedEntityField(@RequestParam(name = "id", required = true) Long id, Model model) {
		
		ICalculatedEntityField calculatedField = Optional.ofNullable(calculatedEntityFieldService.getCalculatedEntityField(id))
				.orElse(calculatedEntityFieldService.createNewCalculatedEntityField(getVirtualName(VIRTUAL_ENTITY_FIELD_NAME), VIRTUAL_ENTITY_FIELD_DESCR));
		
		String preffix = ItemAbstract.isNew(calculatedField)?
			preffix = ControllerConstants.MSG_NEW_CALC_ENTITY_FIELD:ControllerConstants.MSG_EDIT_CALC_ENTITY_FIELD;
		
		Set<IEntityField> targetFields = calculatedEntityFieldService.getAllTargetFields();
		Set<IEntityField> sourceFields = calculatedEntityFieldService.getAllSourceFields();
		
		Map<Long, Long> selectedSourceFields = getEntitiesMap(calculatedField.getSourceEntityFields()); 
		
		model.addAttribute("field", calculatedField);
		model.addAttribute("targetFieldId", calculatedField.getTargetEntityField()!=null?calculatedField.getTargetEntityField():null);
		model.addAttribute("targetFields", targetFields);
		
		model.addAttribute("sourceFields", sourceFields);
		model.addAttribute("selectedSourceFields", selectedSourceFields);
		
		model.addAttribute("calculators", calculatedEntityFieldService.getCalculators());
		model.addAttribute(PROP_PAGE_HEADER, getPageHeader(preffix, calculatedField));
		
		return "virtual/calculatedEntityField.html";
	}
	
	@PostMapping(value = "/saveCalculatedEntityField")
	public String saveCalculatedEntityField(@ModelAttribute("field") CalculatedEntityField calcEntityField,
			@RequestParam(value = "calculators", required = false) String[] calculators,
			@RequestParam(value = "targetFields", required = false) long[] targetFieldIds,
			@RequestParam(value = "sourceFields", required = false) long[] sourceFieldIds, 
			Model model) {
		
		String calculatorName = getFirstIdOrNull(calculators);
		Long targetFieldId = getFirstIdOrNull(targetFieldIds);
		
		Map<Long, IEntityField> sourceEntityFields = new HashMap<Long, IEntityField>();

		if (sourceFieldIds != null) {
			for (long id : sourceFieldIds) {
				IEntityField ef = entityService.getEntityFieldById(id);
				if (ef != null) {
					sourceEntityFields.put(id, ef);
				}
			}
		}

		ICalculatedEntityFieldCalculator calculator = calculatedEntityFieldService.getCalculator(calculatorName);
		IEntityField targetfield = entityService.getEntityFieldById(targetFieldId);
		
		if (ItemAbstract.isNew(calcEntityField)) {
			calcEntityField.setCalculator(calculator);
			calcEntityField.setTargetEntityField(targetfield);
			calcEntityField.setSourceEntityFields(new HashSet(sourceEntityFields.values()));

			calculatedEntityFieldService.save(calcEntityField);
		} else {
			ICalculatedEntityField ent = calculatedEntityFieldService.getCalculatedEntityFieldById(calcEntityField.getId());

			ent.setNameDescriptionEmoji(calcEntityField);
			
			ent.setCalculator(calculator);
			ent.setTargetEntityField(targetfield);
			ent.setSourceEntityFields(new HashSet(sourceEntityFields.values()));
			
			calculatedEntityFieldService.save(ent);
		}
		
		
		return "redirect:/virtual/calculatedEntityFields";
	}

	private void setParentOrNull(long[] expected, Set<? extends IItemAbstract> children, IItemAbstract parent,
			IItemAbstract parentOfParent) {
		Map<Long, Long> toBeRemovedMap = getChildrenToBeRemovedMap(expected, children);

		parent.setParent(parentOfParent);

		children.stream().forEach(child -> {
			if (toBeRemovedMap.containsKey(child.getId())) {
				child.setParent(null);
			} else {
				child.setParent(parent);
			}
		});
	}

	private <T extends IItemAbstract> Map<Long, T> addUnexisting(Map<Long, T> current, Collection<T> proposal) {

		proposal.stream().filter(a -> a != null && !current.containsKey(a.getId()))
				.forEach(a -> current.put(a.getId(), a));

		return current;
	}

	private Map<Long, Long> getChildrenToBeRemovedMap(long[] expected, Set<? extends IItemAbstract> actual) {
		Map<Long, Long> idsExpected = getEntitiesMap(expected);
		Map<Long, Long> result = new HashMap<Long, Long>();

		actual.stream().filter(a -> !idsExpected.containsKey(a.getId())).forEach(a -> result.put(a.getId(), a.getId()));

		return result;
	}

	private Map<Long, Long> getEntitiesMap(long[] items) {
		Map<Long, Long> ids = new HashMap<Long, Long>();

		if (items != null) {
			for (Long id : items) {
				ids.put(id, id);
			}
		}

		return ids;
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
	
	private <T> T getFirstIdOrNull(T[] ids) {
		T result = null;
		if (ids != null && ids.length > 0) {
			result = ids[0];
		}
		return result;
	}

	private String getVirtualName(String baseName) {
		return baseName + (new Date()).getTime();
	}
}
