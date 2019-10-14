package com.balabas.smarthouse.server.entity.model.descriptor;

import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_ENTITY_FIELDS;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_ID;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_NAME;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_EMOJI;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_DESCR_FIELD;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_DESCRIPTION;

import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Optional;

import org.json.JSONObject;

import com.google.common.base.Joiner;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.extern.log4j.Log4j2;

@Log4j2
@AllArgsConstructor
public class DeviceEntityDescriptor implements IDeviceEntityDescriptor {

	@Getter
	private int remoteId;
	@Getter
	private String name;
	@Getter
	private Emoji emoji;
	@Getter
	IEntityDescriptor descriptionDescriptor;
	@Getter
	private Map<String, IEntityDescriptor> descriptors;
	
	public static IDeviceEntityDescriptor fromJson(JSONObject json, String nameOver) {
		if(json == null || json.isEmpty()) {
			return null;
		}
		Map<String, IEntityDescriptor> descrs = new LinkedHashMap<>();
		
		Optional.ofNullable(json.optJSONObject(EDC_ENTITY_FIELDS))
		.ifPresent( fieldsJson -> descrs.putAll(EntityDescriptor.fromJson(fieldsJson)));
		
		int id = json.optInt(EDC_FIELD_ID, -1);
		String name = json.optString(EDC_FIELD_NAME, nameOver);
		
		if(id<0 || descrs.isEmpty()) {
			return null;
		}
		
		Emoji emoji = Emoji.getByCode(json.optString(EDC_FIELD_EMOJI, null));
		String descrField = json.optString(EDC_FIELD_DESCR_FIELD, EDC_FIELD_DESCRIPTION);
		
		IEntityDescriptor descr = descrs.getOrDefault(descrField, null);
		
		IDeviceEntityDescriptor result = new DeviceEntityDescriptor(id, name, emoji, descr, descrs);
		log.debug("DeviceEntity descr : " + result);
		
		return result;
	}

	@Override
	public String toString() {
		return "EntityDescriptorMap [remoteId=" + remoteId + ", name=" + name + ", descriptors= [" +
				Joiner.on(",").withKeyValueSeparator("=").join(descriptors)+ "] ]";
	}
	
	
}
