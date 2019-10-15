package com.balabas.smarthouse.server.entity.model;

import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_STATUS;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_SWG;
import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_FIELD_DEVICE;
import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_FIELD_DEVICE_INFO;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_DEVICE_DEVICE_DESCRIPTION;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_DEVICE_DEVICE_FIRMWARE;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_COUNT;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_DESCRIPTION;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_FIELDS_ARRAY;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_ITEM_CLASS;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_KEYS_ARRAY;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_SENSOR_ITEMS;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_CLASS;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_CLASS_VIEW;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_ENTITY_FIELDS;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_DESCRIPTION;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_DESCR_FIELD;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_EMOJI;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_NAME;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_ID;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_READ_ONLY;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EMPTY_STR;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.FALSE;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.HIGH;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.TRUE;

import java.util.LinkedHashSet;
import java.util.Optional;
import java.util.Set;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

import org.json.JSONArray;
import org.json.JSONObject;

import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;
import com.balabas.smarthouse.server.entity.model.descriptor.EntityFieldClassType;
import com.balabas.smarthouse.server.entity.model.descriptor.EntityFieldClassView;
import com.balabas.smarthouse.server.exception.BadValueException;
import com.balabas.smarthouse.server.model.EntityClass;

import lombok.experimental.UtilityClass;
import lombok.extern.log4j.Log4j2;

@SuppressWarnings("rawtypes")
@UtilityClass
@Log4j2
public class EntityBuilder {
	
	public static void processDeviceInfo(Device device, JSONObject deviceJson) {
		if (deviceJson.has(DEVICE_FIELD_DEVICE)) {
			JSONObject deviceDeviceJson = deviceJson.getJSONObject(DEVICE_FIELD_DEVICE);

			if (deviceDeviceJson.has(DEVICE_FIELD_DEVICE_INFO)) {
				JSONObject info = Optional.ofNullable(deviceDeviceJson.getJSONObject(DEVICE_FIELD_DEVICE_INFO)).orElse(new JSONObject());

				device.setDescription(info.optString(ENTITY_DEVICE_DEVICE_DESCRIPTION, device.getDescription()));
				device.setFirmware(info.optString(ENTITY_DEVICE_DEVICE_FIRMWARE, device.getFirmware()));
			}
		}
	}
	
	public static IEntity buildEntityFromJson(JSONObject groupJson, String groupName, String entityName) throws BadValueException {
		
		JSONObject entityJson = groupJson.optJSONObject(entityName);
		JSONObject descriptorJson = entityJson.optJSONObject(ENTITY_FIELD_SWG);
		
		String description = entityJson.optString(ENTITY_FIELD_DESCRIPTION, null);
		Emoji emoji = Emoji.getByCode(descriptorJson.optString(EDC_FIELD_EMOJI, null));
		String descriptionField = descriptorJson.optString(EDC_FIELD_DESCR_FIELD, ENTITY_FIELD_DESCRIPTION);
		int remoteId = descriptorJson.optInt(EDC_FIELD_ID, -1);
		EntityClass renderer = EntityClass.getByKey(entityJson.optString(ENTITY_FIELD_ITEM_CLASS, descriptorJson.optString(ENTITY_FIELD_ITEM_CLASS,"")));
		
		IEntity result = new Entity();
		
		result.setName(entityName);
		result.setGroupName(groupName);
		result.setDescription(description);
		result.setEmoji(emoji);
		result.setDescriptionField(descriptionField);
		result.setRemoteId(remoteId);
		result.setRenderer(renderer);
		
		Set<IEntityField> children = new LinkedHashSet<>();
		
		for(String fieldName : JSONObject.getNames(entityJson)) {
			if(ENTITY_FIELD_STATUS.equals(fieldName)){
				processEntityStatus(result, entityJson);
			}else
			if (ENTITY_FIELD_SENSOR_ITEMS.equals(fieldName)) {
				processSensorItemsValues(result, entityJson);
			} else
			if (!ENTITY_FIELD_SWG.equals(fieldName)
					&& !ENTITY_FIELD_ITEM_CLASS.equals(fieldName)) {
				children.add(buildFieldFromJson(entityJson, fieldName));
			}  
		}
		
		result.setChildren(children);
		
		return result;
	}
	
	private void processEntityStatus(IEntity entity, JSONObject entityJson) {
		JSONObject statusJson = entityJson.optJSONObject(ENTITY_FIELD_STATUS);
		if (statusJson != null){
			int stat = entityJson.optInt(EDC_FIELD_ID, 1);
			String descr = entityJson.optString(EDC_FIELD_DESCRIPTION, "OK");
			
			entity.setStatus(new EntityStatus(stat, descr));
		}
	}
	
	private void processSensorItemsValues(IEntity entity, JSONObject entityJson) {
		JSONObject sensorItemsJson = entityJson.getJSONObject(ENTITY_FIELD_SENSOR_ITEMS);
		
		log.debug("sensorItemsJson : " + sensorItemsJson);
		
		int count = sensorItemsJson.optInt(ENTITY_FIELD_COUNT, 0);

		Set<String> fields = Optional.ofNullable(sensorItemsJson.optJSONArray(ENTITY_FIELD_FIELDS_ARRAY))
				.orElse(new JSONArray()).toList().stream().map(Object::toString).collect(Collectors.toSet());

		Set<String> keys = Optional.ofNullable(sensorItemsJson.optJSONArray(ENTITY_FIELD_KEYS_ARRAY))
				.orElse(new JSONArray()).toList().stream().map(Object::toString).collect(Collectors.toSet());

		if (count == 0 && !keys.isEmpty()) {
			count = keys.size();
		}

		if (keys.isEmpty() && count > 0) {
			IntStream.range(0, count).forEach(i -> keys.add(Integer.toString(i)));
		}

		if (count == 0 || fields.isEmpty() || keys.size() != count) {
			log.error("Bad SensorItems count=" + count + " fields=" + fields + " keys = " + keys);
		} else {
			entity.setSensorItemFields(fields);
			entity.setSensorItemIds(keys);

			log.debug("Sensor items added");
		}

	}

	public static IEntityField buildFieldFromJson(JSONObject entityJson, String fieldName) throws BadValueException {
		JSONObject descriptorJson = entityJson.optJSONObject(ENTITY_FIELD_SWG);
		JSONObject fieldsDecriptor = descriptorJson.optJSONObject(EDC_ENTITY_FIELDS);
		
		JSONObject fieldDecriptor = fieldsDecriptor.optJSONObject(fieldName);
		
		String value = entityJson.optString(fieldName);

		Class<?> clazz = Optional.ofNullable( EntityFieldClassType.from(fieldDecriptor.optString(EDC_CLASS, null))).orElse(EntityFieldClassType.STRING).getClazz();
		String name = fieldDecriptor.optString(EDC_FIELD_NAME, fieldName);
		String description = fieldDecriptor.optString(EDC_FIELD_DESCRIPTION, EMPTY_STR);
		EntityFieldClassView viewClass = EntityFieldClassView.from(fieldDecriptor.optString(EDC_CLASS_VIEW, null));
		boolean readOnly = booleanFromString( fieldDecriptor.optString(EDC_READ_ONLY, FALSE));
		Emoji emoji = Emoji.getByCode(fieldDecriptor.optString(EDC_FIELD_EMOJI, null));
		
		IEntityField result = createEntityFieldByClass(clazz);
		
		result.setName(name);
		result.setDescription(description);
		result.setDescription(description);
		result.setViewClass(viewClass);
		result.setReadOnly(readOnly);
		result.setEmoji(emoji);
		result.setValueStr(value);
		
		return result;
	}
	
	private static Set<IEntityField> getEnabledValues(JSONObject fieldDecriptor, String fieldName) {
		
		return null;
	}
	
	private static IEntityField createEntityFieldByClass(Class<?> clazz){
		if(clazz.equals(Boolean.class)){
			return new EntityFieldBoolean();
		}else if (clazz.equals(Integer.class)){
			return new EntityFieldInteger();
		}else if (clazz.equals(Float.class)){
			return new EntityFieldFloat();
		}else if (clazz.equals(Long.class)){
			return new EntityFieldLong();
		}else {
			return new EntityFieldString();
		}
	}
	
	private static boolean booleanFromString(String value) {
		if (TRUE.equalsIgnoreCase(value)) {
			return true;
		}
		if (HIGH.equalsIgnoreCase(value)) {
			return true;
		}
		if (value != null && !value.isEmpty() && value.startsWith(HIGH)) {
				return true;
		}
		return false;
	}
}

