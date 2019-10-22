package com.balabas.smarthouse.server.entity.service;

import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_STATUS;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_SWG;
import static com.balabas.smarthouse.server.DeviceConstants.HTTP_PREFFIX;
import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_FIELD_DEVICE;
import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_FIELD_DEVICE_INFO;
import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_FIELD_URL_DATA;
import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_URL_DATA;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_DEVICE_DEVICE_DESCRIPTION;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_DEVICE_DEVICE_FIRMWARE;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_COUNT;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_C;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_DESCRIPTION;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_FIELDS_ARRAY;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_ITEM_CLASS;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_KEYS_ARRAY;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_SENSOR_ITEMS;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_CLASS;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_CLASS_VIEW;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_ENTITY_FIELDS;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_ACTION_DESCR;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_DESCRIPTION;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_DESCR_FIELD;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_EMOJI;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_ENABLED_VALUES;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_NAME;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_ID;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_READ_ONLY;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EMPTY_STR;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.FALSE;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.HIGH;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.TRUE;

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
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;

import com.balabas.smarthouse.server.entity.model.descriptor.EntityClass;
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
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.repository.IDeviceRepository;
import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.EntityStatus;
import com.balabas.smarthouse.server.entity.model.Group;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IGroup;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.descriptor.ActionTimer;
import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;
import com.balabas.smarthouse.server.exception.BadValueException;
import com.balabas.smarthouse.server.model.request.DeviceRequest;

import lombok.extern.log4j.Log4j2;

@SuppressWarnings({ "rawtypes", "unchecked" })
@Log4j2
@Service
public class SmartHouseItemBuildService {

	private static List<String> notUpdateableEntityFields = Arrays.asList(ENTITY_FIELD_STATUS,
			ENTITY_FIELD_SENSOR_ITEMS, ENTITY_FIELD_SWG, ENTITY_FIELD_ITEM_CLASS);

	private static List<String> notParsedAsEntityNames = Arrays.asList(ENTITY_FIELD_DESCRIPTION, EDC_FIELD_EMOJI);

	@Autowired
	IDeviceRepository deviceRepository;

	public Device buildDeviceFromRequest(DeviceRequest request) {

		String deviceDataUrl = getDeviceDataUrl(request);

		Device device = Optional.ofNullable(deviceRepository.findByName(request.getDeviceId())).orElse(new Device());

		Date registrationDate = Optional.ofNullable(device.getRegistrationDate()).orElse(new Date());

		device.setName(request.getDeviceId());
		device.setFirmware(request.getDeviceFirmware());
		
		device.setDescriptionIfEmpty(request.getDeviceDescr());

		device.setIp(request.getIp());
		device.setDataUrl(deviceDataUrl);
		device.setRegistrationDate(registrationDate);
		device.setState(State.CONSTRUCTED);

		device.setTimer(buildTimer(ItemType.DEVICE));

		return device;
	}

	private static String getDeviceDataUrl(DeviceRequest request) {
		String baseUrl = HTTP_PREFFIX + request.getIp();

		String deviceDataUrl = DEVICE_URL_DATA;

		if (request.hasData()) {
			String data = request.getData();

			JSONObject dataJson = new JSONObject(data);

			if (dataJson.has(DEVICE_FIELD_URL_DATA)) {
				deviceDataUrl = dataJson.optString(DEVICE_FIELD_URL_DATA, DEVICE_URL_DATA);
			}
		}

		if (!deviceDataUrl.startsWith(HTTP_PREFFIX)) {
			deviceDataUrl = baseUrl + deviceDataUrl;
		}

		return deviceDataUrl;
	}

	private static ActionTimer buildTimer(ItemType itemType) {
		Long updateInterval = itemType.getRefreshInterval();

		if (updateInterval > 0) {
			ActionTimer updateTimer = new ActionTimer(updateInterval);
			updateTimer.setActionForced(true);
			return updateTimer;
		}

		return null;
	}

	public boolean updateDeviceEntityValuesFromJson(IDevice device, JSONObject deviceJson) {

		boolean isOk = true;

		for (String groupName : JSONObject.getNames(deviceJson)) {

			IGroup group = device.getGroup(groupName);
			if (group != null) {
				JSONObject groupJson = deviceJson.optJSONObject(groupName);

				boolean groupOk = updateGroupEntityValuesFromJson(group, groupJson);

				isOk = isOk && groupOk;
			}
		}
		if (isOk) {
			device.getTimer().setActionSuccess();
		}
		return isOk;
	}

	private boolean updateGroupEntityValuesFromJson(IGroup group, JSONObject groupJson) {
		boolean isOk = true;
		for (String entityName : JSONObject.getNames(groupJson)) {

			Entity entity = group.getEntity(entityName);
			JSONObject entityJson = groupJson.optJSONObject(entityName);

			if (entity != null && entityJson != null && !entityJson.isEmpty()) {
				boolean entityOk = updateEntityValuesFromJson(entity, entityJson);

				if (entityOk) {
					entity.setState(State.OK);
				} else {
					entity.setState(State.BAD_DATA);
				}

				isOk = isOk && entityOk;
			}
		}
		if (isOk) {
			group.getTimer().setActionSuccess();
		}
		return isOk;
	}

	private boolean updateEntityValuesFromJson(Entity entity, JSONObject entityJson) {
		boolean setOk = true;

		for (String entityFieldName : JSONObject.getNames(entityJson)) {

			if (!notUpdateableEntityFields.contains(entityFieldName)) {
				try {
					IEntityField entityField = entity.getEntityField(entityFieldName);
					Object valueObj = entityJson.get(entityFieldName);

					if (entity.getDescriptionField().equals(entityFieldName)) {
						entity.setDescriptionIfEmpty(valueObj.toString());
					}

					if (entityField != null) {
						setOk = setEntityFieldValueWithNoCheck(entityField, valueObj, setOk);
					} else {
						createEntityFieldValue(entity, entityJson, entityFieldName);

					}
				} catch (Exception e) {
					log.error("field " + entityFieldName + " : ", e);
				}
			}
		}

		return setOk;
	}

	private static boolean setEntityFieldValueWithNoCheck(IEntityField entityField, Object value, boolean setOk) {
		try {
			entityField.setValueWithNoCheck(value);
			return setOk;
		} catch (BadValueException e) {
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
		} catch (BadValueException e) {
			log.error(e);
		}
	}

	public boolean processDeviceInfo(IDevice device, JSONObject deviceJson) {
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
					group.setTimer(buildTimer(type));

					groups.add(group);
				}
			}
		}

		return groups;
	}

	public Set<Entity> buildEntitiesForGroupNoEx(Group group, JSONObject deviceJson) {
		try {
			return buildEntitiesForGroup(group, deviceJson);
		} catch (BadValueException e) {
			log.error(e);
			return Collections.emptySet();
		}
	}

	public Set<Entity> buildEntitiesForGroup(Group group, JSONObject deviceJson) throws BadValueException {
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

	public Entity buildEntityFromJson(Group group, JSONObject groupJson, String entityName) throws BadValueException {

		JSONObject entityJson = groupJson.optJSONObject(entityName);
		JSONObject descriptorJson = Optional.ofNullable(entityJson.optJSONObject(ENTITY_FIELD_SWG))
				.orElse(new JSONObject());

		String description = entityJson.optString(ENTITY_FIELD_DESCRIPTION, null);
		Emoji emoji = Emoji.getByCode(descriptorJson.optString(EDC_FIELD_EMOJI, null));
		String descriptionField = descriptorJson.optString(EDC_FIELD_DESCR_FIELD, ENTITY_FIELD_DESCRIPTION);
		int remoteId = descriptorJson.optInt(EDC_FIELD_ID, -1);
		EntityClass renderer = EntityClass.getByKey(
				entityJson.optString(ENTITY_FIELD_ITEM_CLASS, descriptorJson.optString(ENTITY_FIELD_ITEM_CLASS, "")));

		Entity entity = new Entity();

		entity.setName(entityName);
		entity.setGroup(group);
		entity.setDescriptionIfEmpty(description);
		entity.setEmoji(emoji);
		entity.setDescriptionField(descriptionField);
		entity.setRemoteId(remoteId);
		entity.setRenderer(renderer);

		Set<IEntityField> entityFields = new LinkedHashSet<>();

		for (String fieldName : JSONObject.getNames(entityJson)) {
			if (ENTITY_FIELD_STATUS.equals(fieldName)) {
				processEntityStatus(entity, entityJson);
			} else if (ENTITY_FIELD_SENSOR_ITEMS.equals(fieldName)) {
				processGrouppedValues(entity, entityJson);
			} else if (!ENTITY_FIELD_SWG.equals(fieldName) && !ENTITY_FIELD_ITEM_CLASS.equals(fieldName)) {
				EntityField field = buildEntityFieldFromJson(entity, entityJson, fieldName);
				entityFields.add(field);
			}
		}

		entity.setEntityFields(entityFields);

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

	private static void processGrouppedValues(Entity entity, JSONObject entityJson) {
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
			entity.setGrouppedFieldsIds(grouppedFieldsIds);
			entity.setGrouppedFieldsNames(grouppedFieldsNames);

			try {
				IEntityField countField = new EntityFieldInteger();
				countField.setName(ENTITY_FIELD_COUNT);
				countField.setReadOnly(true);
				countField.setValueWithNoCheck(Integer.valueOf(count));

				entity.addGeneratedField(countField);
			} catch (BadValueException e) {
				log.error(e);
			}

			log.debug("Sensor items added");
		}

	}

	private static EntityField buildEntityFieldFromJson(Entity entity, JSONObject entityJson, String fieldName)
			throws BadValueException {
		JSONObject descriptorJson = entityJson.optJSONObject(ENTITY_FIELD_SWG);
		JSONObject allFieldsDecriptor = descriptorJson.optJSONObject(EDC_ENTITY_FIELDS);

		JSONObject fieldDecriptor = Optional.ofNullable(allFieldsDecriptor.optJSONObject(fieldName))
				.orElse(new JSONObject());

		String value = entityJson.optString(fieldName);

		EntityFieldClassType fieldClassType = Optional
				.ofNullable(EntityFieldClassType.from(fieldDecriptor.optString(EDC_CLASS, null)))
				.orElse(EntityFieldClassType.STRING);

		String name = fieldDecriptor.optString(EDC_FIELD_NAME, fieldName);
		String description = fieldDecriptor.optString(EDC_FIELD_DESCRIPTION, EMPTY_STR);
		EntityFieldClassView viewClass = EntityFieldClassView.from(fieldDecriptor.optString(EDC_CLASS_VIEW, null));
		boolean readOnly = booleanFromString(fieldDecriptor.optString(EDC_READ_ONLY, FALSE));
		Emoji emoji = Emoji.getByCode(fieldDecriptor.optString(EDC_FIELD_EMOJI, null));

		EntityField entityField = createEntityFieldByClass(fieldClassType);

		entityField.setEntity(entity);
		entityField.setName(name);
		entityField.setDescriptionIfEmpty(description);
		entityField.setViewClass(viewClass);
		entityField.setReadOnly(readOnly);
		entityField.setEmoji(emoji);
		entityField.setValueStr(value);

		Set<IEntityFieldEnabledValue> enabledValues = getEnabledFieldValues(fieldDecriptor, name, fieldClassType,
				entityField);
		entityField.setEnabledValues(enabledValues);

		return entityField;
	}

	private static Set<IEntityFieldEnabledValue> getEnabledFieldValues(JSONObject fieldDecriptor, String fieldName,
			EntityFieldClassType fieldClassType, EntityField entityField) throws BadValueException {
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

				IEntityFieldEnabledValue enabledValue = createEntityFieldEnabledValueByClass(fieldClassType);
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
