package com.balabas.smarthouse.server.entity.service;

import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_STATUS;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_SWG;
import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_FIELD_DEVICE;
import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_FIELD_DEVICE_INFO;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_DEVICE_DEVICE_DESCRIPTION;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_DEVICE_DEVICE_FIRMWARE;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_COUNT;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_C;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_DESCRIPTION;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_FIELDS_ARRAY;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_ITEM_CLASS;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_KEYS_ARRAY;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_SENSOR_ITEMS;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_ID;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_MESSAGE;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_MQ;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_CLASS;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_CLASS_VIEW;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_ENTITY_FIELDS;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_ACTION_DESCR;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_DESCRIPTION;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_DESCR_FIELD;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_EMOJI;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_ENABLED_VALUES;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_NAME;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_MEASURE;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_ID;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_READ_ONLY;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EMPTY_STR;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.FALSE;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.HIGH;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.TRUE;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_COUNT_DESCR;

import java.util.Arrays;
import java.util.Collections;
import java.util.Date;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Optional;
import java.util.Set;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

import org.json.JSONArray;
import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;

import com.balabas.smarthouse.server.entity.model.descriptor.EntityFieldClassType;
import com.balabas.smarthouse.server.entity.model.descriptor.EntityFieldClassView;
import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;
import com.balabas.smarthouse.server.entity.model.descriptor.State;
import com.balabas.smarthouse.server.entity.model.enabledvalue.EntityFieldEnabledValue;
import com.balabas.smarthouse.server.entity.model.enabledvalue.EntityFieldEnabledValueBoolean;
import com.balabas.smarthouse.server.entity.model.enabledvalue.EntityFieldEnabledValueFloat;
import com.balabas.smarthouse.server.entity.model.enabledvalue.EntityFieldEnabledValueInteger;
import com.balabas.smarthouse.server.entity.model.enabledvalue.EntityFieldEnabledValueIp;
import com.balabas.smarthouse.server.entity.model.enabledvalue.EntityFieldEnabledValueLong;
import com.balabas.smarthouse.server.entity.model.enabledvalue.EntityFieldEnabledValuePassword;
import com.balabas.smarthouse.server.entity.model.enabledvalue.EntityFieldEnabledValueString;
import com.balabas.smarthouse.server.entity.model.enabledvalue.IEntityFieldEnabledValue;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityField;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldBoolean;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldFloat;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldInteger;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldIp;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldLong;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldPassword;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldString;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldValue;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldValueBoolean;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldValueNumber;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.repository.IDeviceRepository;
import com.balabas.smarthouse.server.entity.repository.IEntityFieldIncorrectValueRepository;
import com.balabas.smarthouse.server.util.MathUtil;
import com.balabas.smarthouse.server.entity.behaviour.IEntityBehaviourService;
import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.EntityStatus;
import com.balabas.smarthouse.server.entity.model.Group;
import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;

import lombok.extern.log4j.Log4j2;

@SuppressWarnings({ "rawtypes", "unchecked" })
@Log4j2
@Service
public class SmartHouseItemBuildService {

	private static List<String> notUpdateableEntityFields = Arrays.asList(ENTITY_FIELD_ID, ENTITY_FIELD_STATUS,
			ENTITY_FIELD_SENSOR_ITEMS, ENTITY_FIELD_SWG, ENTITY_FIELD_ITEM_CLASS, ENTITY_FIELD_MESSAGE);

	private static List<String> notParsedAsEntityNames = Arrays.asList(ENTITY_FIELD_DESCRIPTION, EDC_FIELD_EMOJI);

	@Value("${smarthouse.server.fields.data.changed.if.diff:0.001}")
	private Float minDiff;
	
	@Value("${smarthouse.server.fields.data.save.unchanged.if.older.sec:600}")
	private Long saveDataIfOlderSec;
	
	@Autowired
	IDeviceRepository deviceRepository;
	
	@Autowired
	IEntityMessageProcessor entityMessageProcessor;
	
	@Autowired
	IActionTimerService actionTimerService; 
	
	@Autowired
	IEntityBehaviourService entityBehaviourService;
	
	@Autowired
	IEntityFieldIncorrectValueRepository entityFieldIncorrectValueRepository;

	public boolean updateDeviceEntityValuesFromJson(Device device, JSONObject deviceJson, boolean updateDeviceTimer,
			boolean updateGroupTimer, List<EntityFieldValue> changedValues) {

		boolean isOk = true;

		for (String groupName : JSONObject.getNames(deviceJson)) {

			Group group = device.getGroup(groupName);
			if (group != null) {
				JSONObject groupJson = deviceJson.optJSONObject(groupName);

				boolean groupOk = updateGroupEntityValuesFromJson(group, groupJson, updateGroupTimer, changedValues);

				isOk = isOk && groupOk;
			}
		}
		if (isOk && updateDeviceTimer) {
			actionTimerService.setActionSuccess(device);
		}
		return isOk;
	}

	private boolean updateGroupEntityValuesFromJson(Group group, JSONObject groupJson, boolean updateGroupTimer,
			List<EntityFieldValue> changedValues) {
		boolean isOk = true;
		for (String entityName : JSONObject.getNames(groupJson)) {

			Entity entity = group.getEntity(entityName);
			JSONObject entityJson = groupJson.optJSONObject(entityName);

			if (entity != null && entityJson != null && !entityJson.isEmpty()) {
				boolean entityOk = updateEntityValuesFromJson(entity, entityJson, changedValues);

				if (entityOk) {
					entity.setState(State.OK);
				} else {
					entity.setState(State.BAD_DATA);
				}

				isOk = isOk && entityOk;
			}
		}
		if (isOk && updateGroupTimer) {
			actionTimerService.setActionSuccess(group);
		}
		return isOk;
	}

	public static boolean isFieldValueSaveAble(IEntityField entityField) {
		return entityField != null && !StringUtils.isEmpty(entityField.getName())
				&& !notUpdateableEntityFields.contains(entityField.getName())
				&& !entityField.getEntity().getDescriptionField().equals(entityField.getTemplateName())
				&& entityField.isActive()
				&& !String.class.isAssignableFrom(entityField.getClazz());
	}

	private boolean updateEntityValuesFromJson(Entity entity, JSONObject entityJson,
			List<EntityFieldValue> changedValues) {
		boolean setOk = true;

		for (String entityFieldName : JSONObject.getNames(entityJson)) {

			if (!notUpdateableEntityFields.contains(entityFieldName)) {
				try {
					IEntityField entityField = entity.getEntityField(entityFieldName);
					Object valueObj = entityJson.get(entityFieldName);

					String valStr = valueObj.toString();

					if (entity.getDescriptionField().equals(entityFieldName)) {
						entity.setDescriptionIfEmpty(valStr);
					}

					if (entityField != null) {
						if (!entity.getDescriptionField().equals(entityField.getTemplateName())) {

							if (entityBehaviourService.isValueCorrect(entity, valStr)) {
								String oldValue = entityField.getValueStr();
	
								setOk = setEntityFieldValueWithNoCheck(entityField, valStr, setOk);
	
								if (setOk) {
									processValueChange(entityField, oldValue, changedValues);
								}
							} else {
								
								if(entityField !=null) {
									entityFieldIncorrectValueRepository.insertEntityFieldIncorrectValue(entityField.getId(), valStr, new Date());
								} 
								log.error(entity.getEntityKey() + " " +entityField.getName() + " INCORRECT value " +valStr);
							}
							
						} else {
							setOk = true;
						}
					} else {
						createEntityFieldValue(entity, entityJson, entityFieldName);
						log.info("new field added" + entityFieldName);
					}
				} catch (Exception e) {
					log.error("field " + entityFieldName + " : ", e);
				}
			} else {
				if(ENTITY_FIELD_MESSAGE.equals(entityFieldName)) {
					entityMessageProcessor.processMessage(entity, entityJson.optJSONObject(ENTITY_FIELD_MESSAGE));
				}
			}
		}

		return setOk;
	}
	
	public void processValueChange(IEntityField entityField, String oldValueStr,
			List<EntityFieldValue> changedValues) {

		boolean doSave = false;
		boolean dataIsTooOld = entityField.getLastDate()==null || (new Date()).getTime() - entityField.getLastDate().getTime() > saveDataIfOlderSec;

		if (entityField.isActive() 
				&& ItemType.SENSORS.getCode().equals(entityField.getEntity().getGroup().getName())) {

			entityField.setLastDate(new Date());
			
			if (Number.class.isAssignableFrom(entityField.getClazz())) {

				Float value = null;

				String newValueStr = entityField.getValueStr();

				if (oldValueStr == null && newValueStr != null) {
					value = Float.valueOf(newValueStr);
					doSave = true;
				} else if (oldValueStr != null && newValueStr != null) {
					value = Float.valueOf(newValueStr);
					Float oldValue = MathUtil.precise(Float.valueOf(oldValueStr));

					doSave = Math.abs(oldValue - value) > minDiff || dataIsTooOld;
				}

				if (doSave) {
					changedValues.add(new EntityFieldValueNumber(entityField, value));
				}
			} else if (Boolean.class.isAssignableFrom(entityField.getClazz())) {

				Boolean value = null;

				String newValueStr = entityField.getValueStr();

				if (oldValueStr == null && newValueStr != null) {
					value = Boolean.valueOf(newValueStr);
					doSave = true;
				} else if (oldValueStr != null && newValueStr != null) {
					value = Boolean.valueOf(newValueStr);
					Boolean oldValue = Boolean.valueOf(oldValueStr);

					doSave = !value.equals(oldValue) || dataIsTooOld;
				}

				if (doSave) {
					changedValues.add(new EntityFieldValueBoolean(entityField, value));
				}
			}

		}
	}

	private static boolean setEntityFieldValueWithNoCheck(IEntityField entityField, String value, boolean setOk) {
		try {
			entityField.setValueWithNoCheckStr(value);
			return setOk;
		} catch (IllegalArgumentException e) {
			log.error(e);
			return false;
		}
	}

	private static void createEntityFieldValue(Entity entity, JSONObject entityJson, String entityFieldName) {
		log.warn("New field " + entityFieldName + " entity = " + entity.getName() + " device = "
				+ entity.getGroup().getDevice().getName());
		try {
			EntityField field = buildEntityFieldFromJson(entity, entityJson, entityFieldName);
			entity.getEntityFields().add(field);
		} catch (IllegalArgumentException e) {
			log.error(e);
		}
	}

	public boolean processDeviceInfo(Device device, JSONObject deviceJson) {
		if (deviceJson.has(DEVICE_FIELD_DEVICE)) {
			JSONObject deviceDeviceJson = deviceJson.getJSONObject(DEVICE_FIELD_DEVICE);

			if (deviceDeviceJson.has(DEVICE_FIELD_DEVICE_INFO)) {
				JSONObject info = Optional.ofNullable(deviceDeviceJson.getJSONObject(DEVICE_FIELD_DEVICE_INFO))
						.orElse(new JSONObject());

				String description = info.optString(ENTITY_DEVICE_DEVICE_DESCRIPTION, device.getDescription());
				String firmware = info.optString(ENTITY_DEVICE_DEVICE_FIRMWARE, device.getFirmware());
				Emoji emoji = Optional.ofNullable(Emoji.getByCode(info.optString(EDC_FIELD_EMOJI, null)))
						.orElse(ItemType.DEVICE.getEmoji());

				boolean changed = !description.equals(device.getDescription()) || !firmware.equals(device.getFirmware())
						|| !emoji.equals(device.getEmoji());

				device.setDescriptionIfEmpty(description);
				device.setFirmware(firmware);
				device.setEmoji(emoji);

				return changed;
			}
		}

		return false;
	}

	public boolean buildDeviceFromJson(Device device, JSONObject deviceJson) {
		Set<Group> groups = buildGroupsFromJson(device, deviceJson);
		boolean initOk = !groups.isEmpty();

		if (initOk) {
			for (Group group : groups) {
				Set<Entity> entities = buildEntitiesForGroupNoEx(group, deviceJson);

				group.setEntities(entities);
				initOk = initOk && !entities.isEmpty();
			}
		}

		device.setGroups(groups);

		return initOk;
	}

	public Set<Group> buildGroupsFromJson(Device device, JSONObject deviceJson) {
		Set<Group> groups = new LinkedHashSet<>();

		for (String groupName : JSONObject.getNames(deviceJson)) {
			if (!DEVICE_FIELD_DEVICE.equals(groupName)) {
				ItemType type = ItemType.getItemTypeByName(groupName);

				if (!ItemType.CUSTOM.equals(type)) {
					String description = deviceJson.getJSONObject(groupName).optString(ENTITY_FIELD_DESCRIPTION,
							type.getDescription());
					String emojiDescr = deviceJson.getJSONObject(groupName).optString(EDC_FIELD_EMOJI, null);
					Emoji groupEmoji = (emojiDescr != null) ? Emoji.getByCode(emojiDescr) : type.getEmoji();

					Group group = Optional.ofNullable(device.getGroup(groupName)).orElse(new Group());

					group.setEmoji(groupEmoji);
					group.setDevice(device);
					group.setDescriptionIfEmpty(description);
					group.setName(groupName);
					group.setType(type);

					groups.add(group);
				}
			}
		}

		return groups;
	}

	public Set<Entity> buildEntitiesForGroupNoEx(Group group, JSONObject deviceJson) {
		try {
			return buildEntitiesForGroup(group, deviceJson);
		} catch (IllegalArgumentException e) {
			log.error(e);
			return Collections.emptySet();
		}
	}

	public Set<Entity> buildEntitiesForGroup(Group group, JSONObject deviceJson) {
		Set<Entity> entities = new LinkedHashSet<>();

		JSONObject groupJson = deviceJson.optJSONObject(group.getName());

		for (String entityName : JSONObject.getNames(groupJson)) {
			if (!notParsedAsEntityNames.contains(entityName)) {
				Entity entity = buildEntityFromJson(group, groupJson, entityName);
				entities.add(entity);
			}
		}

		return entities;
	}

	public Entity buildEntityFromJson(Group group, JSONObject groupJson, String entityName) {

		JSONObject entityJson = groupJson.optJSONObject(entityName);
		JSONObject descriptorJson = Optional.ofNullable(entityJson.optJSONObject(ENTITY_FIELD_SWG))
				.orElse(new JSONObject());

		String description = entityJson.optString(ENTITY_FIELD_DESCRIPTION, null);
		Emoji emoji = Emoji.getByCode(descriptorJson.optString(EDC_FIELD_EMOJI, null));
		String descriptionField = descriptorJson.optString(EDC_FIELD_DESCR_FIELD, ENTITY_FIELD_DESCRIPTION);
		//int remoteId = descriptorJson.optInt(EDC_FIELD_ID, -1);
		boolean hasMq = descriptorJson.has(ENTITY_FIELD_MQ);
		/*EntityClass renderer = EntityClass.getByKey(
				entityJson.optString(ENTITY_FIELD_ITEM_CLASS, descriptorJson.optString(ENTITY_FIELD_ITEM_CLASS, "")));
*/
		Entity entity = Optional.ofNullable(group.getEntity(entityName)).orElse(new Entity());

		entity.setName(entityName);
		entity.setGroup(group);
		entity.setDescriptionIfEmpty(description);
		entity.setEmoji(emoji);
		entity.setDescriptionField(descriptionField);
		entity.setHasMq(hasMq);
		//entity.setRenderer(renderer);
		
		entityBehaviourService.cacheEntityBehaviourIfFound(entity);

		Set<IEntityField> entityFields = new LinkedHashSet<>();

		Map<String, Object> entityJsonObj = entityJson.toMap();

		for (Entry<String, Object> entry : entityJsonObj.entrySet()) {
			String fieldName = entry.getKey();

			if (ENTITY_FIELD_STATUS.equals(fieldName)) {
				processEntityStatus(entity, entityJson);
			} else if (ENTITY_FIELD_SENSOR_ITEMS.equals(fieldName)) {
				EntityField countField = processGrouppedValues(entity, entityJson);
				if (countField != null) {
					entityFields.add(countField);
				}
			} else if (!ENTITY_FIELD_SWG.equals(fieldName) && !ENTITY_FIELD_ITEM_CLASS.equals(fieldName)) {
				EntityField field = buildEntityFieldFromJson(entity, entityJson, fieldName);
				entityFields.add(field);
			}
		}

		entity.setEntityFields(entityFields);

		entity.getEntityFields().forEach(entField -> {
			if (!entityJsonObj.containsKey(entField.getName()) && !entField.isCalculated()) {
				entField.setActive(false);
			}
		});

		return entity;
	}

	private static void processEntityStatus(Entity entity, JSONObject entityJson) {
		JSONObject statusJson = entityJson.optJSONObject(ENTITY_FIELD_STATUS);
		if (statusJson != null) {
			int stat = entityJson.optInt(EDC_FIELD_ID, 1);
			String descr = entityJson.optString(EDC_FIELD_DESCRIPTION, "OK");

			entity.setStatus(new EntityStatus(stat, descr));
		}
	}

	private static EntityField processGrouppedValues(Entity entity, JSONObject entityJson) {
		JSONObject sensorItemsJson = entityJson.getJSONObject(ENTITY_FIELD_SENSOR_ITEMS);

		log.debug("sensorItemsJson : " + sensorItemsJson);

		int count = sensorItemsJson.optInt(ENTITY_FIELD_COUNT, sensorItemsJson.optInt(ENTITY_FIELD_C, 0));

		Set<String> grouppedFieldsNames = Optional.ofNullable(sensorItemsJson.optJSONArray(ENTITY_FIELD_FIELDS_ARRAY))
				.orElse(new JSONArray()).toList().stream().map(Object::toString).collect(Collectors.toSet());

		Set<String> grouppedFieldsIds = Optional.ofNullable(sensorItemsJson.optJSONArray(ENTITY_FIELD_KEYS_ARRAY))
				.orElse(new JSONArray()).toList().stream().map(Object::toString).collect(Collectors.toSet());

		if (count == 0 && !grouppedFieldsIds.isEmpty()) {
			count = grouppedFieldsIds.size();
		}

		if (grouppedFieldsIds.isEmpty() && count > 0) {
			IntStream.range(0, count).forEach(i -> grouppedFieldsIds.add(Integer.toString(i)));
		}

		if (count == 0 || grouppedFieldsNames.isEmpty() || grouppedFieldsIds.size() != count) {
			log.error("Bad SensorItems count=" + count + " fields=" + grouppedFieldsNames + " grouppedFieldsIds = "
					+ grouppedFieldsIds);
		} else {
			if (entity.getGrouppedFieldsIds() == null) {
				entity.setGrouppedFieldsIds(grouppedFieldsIds);
			} else {
				grouppedFieldsIds.stream().forEach(id -> {
					if (!entity.getGrouppedFieldsIds().contains(id)) {
						entity.getGrouppedFieldsIds().add(id);
					}
				});
			}

			if (entity.getGrouppedFieldsNames() == null) {
				entity.setGrouppedFieldsNames(grouppedFieldsNames);
			} else {
				grouppedFieldsNames.stream().forEach(id -> {
					if (!entity.getGrouppedFieldsNames().contains(id)) {
						entity.getGrouppedFieldsNames().add(id);
					}
				});
			}

			try {

				IEntityField countField = Optional.ofNullable(entity.getEntityField(ENTITY_FIELD_COUNT))
						.orElse(new EntityFieldInteger());
				countField.setEntity(entity);
				countField.setName(ENTITY_FIELD_COUNT);
				countField.setDescriptionIfEmpty(EDC_FIELD_COUNT_DESCR);
				countField.setViewClass(EntityFieldClassView.EDC_CLASS_VIEW_LABEL);
				countField.setReadOnly(true);
				countField.setValueWithNoCheck(Integer.valueOf(count));
				countField.setActive(true);
				countField.setCalculated(true);

				// entity.addGeneratedField(countField);
				return (EntityField) countField;
			} catch (IllegalArgumentException e) {
				log.error(e);
			}

			log.debug("Sensor items added");
		}

		return null;

	}

	private static EntityField buildEntityFieldFromJson(Entity entity, JSONObject entityJson, String fieldName) {
		JSONObject descriptorJson = entityJson.optJSONObject(ENTITY_FIELD_SWG);
		JSONObject allFieldsDecriptor = descriptorJson.optJSONObject(EDC_ENTITY_FIELDS);

		JSONObject fieldDecriptor = Optional.ofNullable(allFieldsDecriptor.optJSONObject(fieldName))
				.orElse(new JSONObject());

		String value = entityJson.optString(fieldName);

		String entityFieldName = fieldDecriptor.optString(EDC_FIELD_NAME, fieldName);

		EntityFieldClassType fieldClassType = Optional
				.ofNullable(EntityFieldClassType.from(fieldDecriptor.optString(EDC_CLASS, null)))
				.orElse(ENTITY_FIELD_ID.equals(entityFieldName) ? EntityFieldClassType.INTEGER
						: EntityFieldClassType.STRING);

		String descriptionField = fieldDecriptor.optString(EDC_FIELD_DESCR_FIELD, null);
		String description = fieldDecriptor.optString(EDC_FIELD_DESCRIPTION, EMPTY_STR);
		EntityFieldClassView viewClass = EntityFieldClassView.from(fieldDecriptor.optString(EDC_CLASS_VIEW, null));
		boolean readOnly = booleanFromString(
				fieldDecriptor.optString(EDC_READ_ONLY, ENTITY_FIELD_ID.equals(entityFieldName) ? TRUE : FALSE));
		Emoji emoji = Emoji.getByCode(fieldDecriptor.optString(EDC_FIELD_EMOJI, null));
		String measure = fieldDecriptor.optString(EDC_FIELD_MEASURE, null);

		EntityField entityField = Optional.ofNullable((EntityField) entity.getEntityField(entityFieldName))
				.orElse(createEntityFieldByClass(fieldClassType));

		if (!entityFieldName.contains(":")) {
			descriptionField = null;
		} else {
			log.debug(entityFieldName);
		}

		entityField.setEntity(entity);
		entityField.setName(entityFieldName);
		entityField.setDescriptionIfEmpty(description);
		entityField.setDescriptionField(descriptionField);
		entityField.setViewClass(viewClass);
		entityField.setReadOnly(readOnly);
		entityField.setEmoji(emoji);
		entityField.setActive(true);
		entityField.setMeasure(measure);
		entityField.setLastDate(new Date());

		try {
			if (!(ENTITY_FIELD_DESCRIPTION.equals(entityField.getTemplateName())
					&& !StringUtils.isEmpty(entityField.getValueStr())
					&& EntityFieldString.class.equals(entityField.getClass()))) {
				entityField.setValueWithNoCheckStr(value);
			}

		} catch (IllegalArgumentException e) {
			log.error(e);
		}

		Set<IEntityFieldEnabledValue> enabledValues = getEnabledFieldValues(fieldDecriptor, entityFieldName,
				fieldClassType, entityField);
		entityField.setEnabledValues(enabledValues);

		return entityField;
	}

	private static Set<IEntityFieldEnabledValue> getEnabledFieldValues(JSONObject fieldDecriptor, String fieldName,
			EntityFieldClassType fieldClassType, EntityField entityField) {
		if (fieldDecriptor.has(EDC_FIELD_ENABLED_VALUES)) {
			JSONObject enabledFieldJson = fieldDecriptor.getJSONObject(EDC_FIELD_ENABLED_VALUES);

			Set<IEntityFieldEnabledValue> enabledValues = new LinkedHashSet<>();

			for (Entry<String, Object> entry : enabledFieldJson.toMap().entrySet()) {
				String enabledValueStr = entry.getKey();

				Map<String, String> enabledValueBody = (Map<String, String>) entry.getValue();

				String description = enabledValueBody.getOrDefault(EDC_FIELD_DESCRIPTION, null);
				String actionDescription = enabledValueBody.getOrDefault(EDC_FIELD_ACTION_DESCR, null);
				Emoji emoji = Emoji.getByCode(enabledValueBody.getOrDefault(EDC_FIELD_EMOJI, null));
				EntityFieldClassView viewClass = EntityFieldClassView
						.from(enabledValueBody.getOrDefault(EDC_CLASS_VIEW, entityField.getViewClass().getKey()));

				IEntityFieldEnabledValue enabledValue = Optional
						.ofNullable(entityField.getEntityFieldEnabledValueByValueStr(enabledValueStr))
						.orElse(createEntityFieldEnabledValueByClass(fieldClassType));
				enabledValue.setEntityField(entityField);
				enabledValue.setDescription(description);
				enabledValue.setActionDescription(actionDescription);
				enabledValue.setEmoji(emoji);
				enabledValue.setValueStr(enabledValueStr);
				enabledValue.setViewClass(viewClass);

				enabledValues.add(enabledValue);

			}

			return enabledValues;
		}

		return Collections.emptySet();

	}

	private static EntityField createEntityFieldByClass(EntityFieldClassType fieldClassType) {
		if (EntityFieldClassType.BOOLEAN.equals(fieldClassType)) {
			return new EntityFieldBoolean();
		} else if (EntityFieldClassType.INTEGER.equals(fieldClassType)) {
			return new EntityFieldInteger();
		} else if (EntityFieldClassType.FLOAT.equals(fieldClassType)) {
			return new EntityFieldFloat();
		} else if (EntityFieldClassType.LONG.equals(fieldClassType)) {
			return new EntityFieldLong();
		} else if (EntityFieldClassType.IP.equals(fieldClassType)) {
			return new EntityFieldIp();
		} else if (EntityFieldClassType.PASSWORD.equals(fieldClassType)) {
			return new EntityFieldPassword();
		} else {
			return new EntityFieldString();
		}
	}

	private static EntityFieldEnabledValue createEntityFieldEnabledValueByClass(EntityFieldClassType fieldClassType) {
		if (EntityFieldClassType.BOOLEAN.equals(fieldClassType)) {
			return new EntityFieldEnabledValueBoolean();
		} else if (EntityFieldClassType.INTEGER.equals(fieldClassType)) {
			return new EntityFieldEnabledValueInteger();
		} else if (EntityFieldClassType.FLOAT.equals(fieldClassType)) {
			return new EntityFieldEnabledValueFloat();
		} else if (EntityFieldClassType.LONG.equals(fieldClassType)) {
			return new EntityFieldEnabledValueLong();
		} else if (EntityFieldClassType.IP.equals(fieldClassType)) {
			return new EntityFieldEnabledValueIp();
		} else if (EntityFieldClassType.PASSWORD.equals(fieldClassType)) {
			return new EntityFieldEnabledValuePassword();
		} else {
			return new EntityFieldEnabledValueString();
		}
	}

	private static boolean booleanFromString(String value) {
		return (TRUE.equalsIgnoreCase(value) || HIGH.equalsIgnoreCase(value)
				|| value != null && !value.isEmpty() && value.startsWith(HIGH));
	}

}
