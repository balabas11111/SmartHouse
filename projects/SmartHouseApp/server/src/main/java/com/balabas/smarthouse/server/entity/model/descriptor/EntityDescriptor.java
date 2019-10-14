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

import org.json.JSONObject;
import org.springframework.util.StringUtils;

import com.balabas.smarthouse.server.entity.model.EntityAbstract;
import com.balabas.smarthouse.server.exception.BadDataException;
import com.google.common.base.Joiner;

import lombok.Builder;
import lombok.Getter;
import lombok.extern.log4j.Log4j2;

@Log4j2
@Builder
public class EntityDescriptor extends EntityAbstract implements IEntityDescriptor {

	/*@Getter
	private EntityClassEditors entityClassEditor;
	@Getter
	private EntityClassSubstance entityClassSubstance;
	@Getter
	private EntityClassLevel entityClassLevel;
	@Getter
	private EntityClassPersistor entityClassPersistor;
	*/
	@Getter
	private int remoteId;
	@Getter
	private EntityClassType entityClassType;
	@Getter
	private EntityClassView entityClassView;
	@Getter
	private Long timeToLive;
	@Getter
	private boolean readOnly;
	@Getter
	private Map<String, String> enabledValues;
	
	protected boolean validate() {
		
		return true;
	}
	
	public static Map<String, IEntityDescriptor> fromJson(JSONObject fieldsJson){
		if(fieldsJson == null || fieldsJson.isEmpty()) {
			return Collections.emptyMap();
		}
		
		log.info("Parse fieldsJson : " + fieldsJson.toString());
		
		Map<String, IEntityDescriptor> result = new HashMap<>();
		
		
		Arrays.asList(JSONObject.getNames(fieldsJson)).stream()
			.filter(fieldId -> !EDC_FIELD_ENABLED_VALUES.equals(fieldId))
			.forEach( fieldId ->	
				Optional.ofNullable(fieldsJson.optJSONObject(fieldId))
					.ifPresent( fieldJson -> Optional.ofNullable(fromJson(fieldJson, fieldId))
								.ifPresent(d -> {result.put(fieldId, d);}))
					);

		return result;
	}
	
	public static IEntityDescriptor fromJson(JSONObject dJson, String nameOver) {
		EntityDescriptor result = null;		
		try {
		
			if (dJson == null) {
				throw new BadDataException("Empty Entity descriptor :");
			}
			
			JSONObject fieldEnabledValues =
					Optional.ofNullable(dJson.optJSONObject(EDC_FIELD_ENABLED_VALUES)).orElse(new JSONObject());
			
			Map<String, String> enabledVals = 
					Optional.ofNullable(fieldEnabledValues.optJSONObject(nameOver)).orElse(new JSONObject())
					.toMap().entrySet().stream()
					.collect(Collectors.toMap(Map.Entry::getKey, e -> (String)e.getValue()));
			
			result = EntityDescriptor.builder()
					/*.entityClassEditor(EntityClassEditors.from(dJson.optString(ENTITY_CLASS_EDITORS, null)))
					.entityClassLevel(EntityClassLevel.from(dJson.optString(ENTITY_CLASS_LEVEL, null)))
					.entityClassPersistor(EntityClassPersistor.from(dJson.optString(ENTITY_CLASS_PERSISTOR, null)))
					.entityClassSubstance(EntityClassSubstance.from(dJson.optString(ENTITY_CLASS_SUBSTANCE, null)))
					*/
					.remoteId(dJson.optInt(EDC_FIELD_ID, -1))
					.entityClassType(EntityClassType.from(dJson.optString(EDC_CLASS, null)))
					.entityClassView(EntityClassView.from(dJson.optString(EDC_CLASS_VIEW, null)))
					.readOnly( booleanFromString(dJson.optString(EDC_READ_ONLY, FALSE)))
					.timeToLive( Long.valueOf(dJson.optLong(EDC_TIME_TO_LIVE, 60000)))
					.enabledValues(enabledVals)
					.build();

			result.setDescription(dJson.optString(EDC_FIELD_DESCRIPTION, EMPTY_STR));
			result.setName(dJson.optString(EDC_FIELD_NAME, (StringUtils.isEmpty(nameOver))?EMPTY_STR:nameOver));
			
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
