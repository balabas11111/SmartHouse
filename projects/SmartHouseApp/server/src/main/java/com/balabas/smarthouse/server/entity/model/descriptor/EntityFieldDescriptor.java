package com.balabas.smarthouse.server.entity.model.descriptor;

import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.FALSE;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.HIGH;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.TRUE;

import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.Optional;
import java.util.stream.Collectors;

import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EMPTY_STR;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_READ_ONLY;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_TIME_TO_LIVE;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_CLASS;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_CLASS_VIEW;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_DESCRIPTION;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_NAME;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_ENABLED_VALUES;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_ID;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_EMOJI;

import org.json.JSONObject;
import org.springframework.util.StringUtils;

import com.balabas.smarthouse.server.exception.BadDataException;
import com.google.common.base.Joiner;

import lombok.Builder;
import lombok.Getter;
import lombok.extern.log4j.Log4j2;

@Log4j2
@Builder
public class EntityFieldDescriptor implements IEntityFieldDescriptor {

	@Getter
	private String name;
	@Getter
	private String description;
	@Getter
	private int remoteId;
	@Getter
	private EntityFieldClassType entityClassType;
	@Getter
	private EntityFieldClassView entityClassView;
	@Getter
	private Long timeToLive;
	@Getter
	private boolean readOnly;
	@Getter
	private Emoji emoji;
	@Getter
	private Map<String, IFieldEnabledValue> enabledValues;
	
	public static Map<String, IEntityFieldDescriptor> fromJson(JSONObject fieldsJson){
		if(fieldsJson == null || fieldsJson.isEmpty()) {
			return Collections.emptyMap();
		}
		
		log.debug("Parse fieldsJson : " + fieldsJson.toString());
		
		Map<String, IEntityFieldDescriptor> result = new HashMap<>();
		
		
		Arrays.asList(JSONObject.getNames(fieldsJson)).stream()
			.filter(fieldId -> !EDC_FIELD_ENABLED_VALUES.equals(fieldId))
			.forEach( fieldId ->	
				Optional.ofNullable(fieldsJson.optJSONObject(fieldId))
					.ifPresent( fieldJson -> Optional.ofNullable(fromJson(fieldJson, fieldId))
								.ifPresent(d -> {result.put(fieldId, d);}))
					);

		return result;
	}
	
	public static IEntityFieldDescriptor fromJson(JSONObject dJson, String nameOver) {
		EntityFieldDescriptor result = null;		
		try {
		
			if (dJson == null) {
				throw new BadDataException("Empty Entity descriptor :");
			}
			
			JSONObject fieldEnabledValues =
					Optional.ofNullable(dJson.optJSONObject(EDC_FIELD_ENABLED_VALUES)).orElse(new JSONObject());
			
			Map<String, IFieldEnabledValue> enabledVals = 
					fieldEnabledValues.toMap().entrySet().stream()
					.collect(Collectors.toMap(Map.Entry::getKey, e -> FieldEnabledValue.fromJson(e.getKey(), (Object)e.getValue())))
					.entrySet().stream().filter(e -> (e.getKey() != null && e.getValue() != null))
					.collect(Collectors.toMap(Map.Entry::getKey, e -> (IFieldEnabledValue) e.getValue()));
					
			
			result = EntityFieldDescriptor.builder()
					.remoteId(dJson.optInt(EDC_FIELD_ID, -1))
					.name(dJson.optString(EDC_FIELD_NAME, (StringUtils.isEmpty(nameOver))?EMPTY_STR:nameOver))
					.description(dJson.optString(EDC_FIELD_DESCRIPTION, EMPTY_STR))
					.entityClassType(EntityFieldClassType.from(dJson.optString(EDC_CLASS, null)))
					.entityClassView(EntityFieldClassView.from(dJson.optString(EDC_CLASS_VIEW, null)))
					.readOnly( booleanFromString(dJson.optString(EDC_READ_ONLY, FALSE)))
					.timeToLive( Long.valueOf(dJson.optLong(EDC_TIME_TO_LIVE, 60000)))
					.emoji( Emoji.getByCode(dJson.optString(EDC_FIELD_EMOJI, null)))
					.enabledValues(enabledVals)
					.build();

			return result;
		
		}catch(Throwable e) {
			log.error(e);
		}
		/*
		if (result == null || !result.validate()) {
			throw new BadDataException("Bad Entity descriptor :");
		}
		*/
		
		return result;
	}
	
	public static boolean booleanFromString(String value) {
		if (TRUE.equalsIgnoreCase(value)) {
			return true;
		}
		if (HIGH.equalsIgnoreCase(value)) {
			return true;
		}
		if (value != null && !value.isEmpty() ) {
			if(value.startsWith(HIGH)) {
				return true;
			}
		}
		return false;
	}

	@Override
	public String toString() {
		return "EntityDescriptor [remoteId=" + remoteId + ", entityClassType=" + entityClassType + ", entityClassView="
				+ entityClassView + ", timeToLive=" + timeToLive + ", readOnly=" + readOnly + ", enabledValues= ["
				+ Joiner.on(",").withKeyValueSeparator("=").join(enabledValues) + "] ]";
		
	}
	
	
	
}
