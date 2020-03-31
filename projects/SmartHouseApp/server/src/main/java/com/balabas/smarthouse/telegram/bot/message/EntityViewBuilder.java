package com.balabas.smarthouse.telegram.bot.message;

import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_MESSAGE;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_SENSOR_ITEMS;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_ID;

import java.io.IOException;
import java.net.URL;
import java.util.Arrays;
import java.util.Collection;
import java.util.HashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.Set;
import java.util.Map.Entry;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IGroup;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;
import com.balabas.smarthouse.server.entity.model.enabledvalue.IEntityFieldEnabledValue;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.service.IDeviceManageService;
import com.balabas.smarthouse.server.view.chart.IMetrics;
import com.google.common.base.Charsets;
import com.google.common.io.Resources;

import lombok.extern.log4j.Log4j2;

@Component
@Log4j2
@SuppressWarnings("rawtypes")
public class EntityViewBuilder {

	@Autowired
	private ItemTextHelper itemTextHelper;

	/*
	 * Builds group view header. Adds to builder
	 */
	public void buildDeviceGroupHeaderView(IDevice device, IGroup group, StringBuilder builder) {
		// build header for device group view.
		builder.append(itemTextHelper.getGroupHeader(device.getEmoji().toString(), device.getDescription(),
				group.getEmoji().toString(), group.getDescription()));
	}

	public void buildEntityView(IEntity entity, StringBuilder builder) {
		buildEntityHeader(entity, builder);
		buildEntityBody(entity, builder);
	}

	public void buildEntityView(IMetrics metrics, StringBuilder builder) {
		buildEntityHeader(metrics, builder);
		buildEntityBody(metrics.getEntityFieldsAsMap(), builder);
	}

	private void buildEntityHeader(IMetrics metric, StringBuilder builder) {
		buildEntityBodyAbstract(Emoji.BAR_CHART, metric, builder, metric.getName(), null, true, false);
	}

	private void buildEntityBody(Map<String, List<IEntityField>> map, StringBuilder builder) {
		boolean first =true;
		
		for(String key : map.keySet()) {
			List<IEntityField> list = map.get(key); 
			if(!first) {
				builder.append("\n");
			}
			builder.append(key);
			builder.append(" ---> ");
			
			if(list.size()<2) {
				builder.append(" ");
			} else {
				builder.append(" \n");
			}
			
			for(IEntityField field : list) {
				buildEntityBody(field, builder);
			}
			
			builder.append("\n");
			first = false;
		}
	}
	
	private void buildEntityBody(IEntityField entityField, StringBuilder builder) {
		buildEntityBodyAbstract(Emoji.EMPTY_EMOJI, entityField, builder,
				"      ", entityField.getValueMeasureStr()+"; ", false,
				false);
	}

	private void buildEntityBodyAbstract(Emoji emojiDefault, IItemAbstract item, StringBuilder builder, String value1,
			String value2, boolean addHorLine, boolean addNextLine) {
		builder.append("<b>");
		builder.append(item.getEmojiOrDefault(emojiDefault));
		if (value1 != null) {
			builder.append(" ");
			builder.append(value1);
		}
		builder.append(" ");
		builder.append(item.getDescriptionByDescriptionField());
		builder.append("</b>");
		if (value2 != null) {
			builder.append(" ");
			builder.append(value2);
		}
		if (addHorLine) {
			builder.append(" \n----------------------- \n");
		}
		if (addNextLine) {
			builder.append(" \n");
		}
	}

	public void buildEntityHeader(IEntity entity, StringBuilder builder) {
		builder.append(itemTextHelper.getEntityHeader(entity.getEmoji().toString(), entity.getDescription()));
		builder.append(" \n");
	}

	public void buildEntityBody(IEntity entity, StringBuilder builder) {

		if (entityValueMapToTemplate(entity.getName(), entity.getEntityFields(), builder, true, false)) {

			if (entity.getGrouppedFieldsIds() != null && !entity.getGrouppedFieldsIds().isEmpty()) {

				Set<IEntityField> siVals = new LinkedHashSet<>();
				String entName = entity.getName() + ENTITY_FIELD_SENSOR_ITEMS;

				entity.getGrouppedFieldsIds().forEach(id -> {
					siVals.clear();

					entity.getGrouppedFieldsNames().forEach(field -> {
						String fieldName = id + ":" + field;
						IEntityField entityField = entity.getEntityField(fieldName);
						if (entityField != null) {
							siVals.add(entityField);
						}
					});

					entityValueMapToTemplate(entName, siVals, /* entity.getGeneratedFields(), */ builder, true, true);
				});
			}

		} else {
			buildEntityBodyViewByFieldDescriptors(entity, builder);
		}

	}
	
	public void buildEntityBodyViewByFieldDescriptors(IEntity entity, StringBuilder builder) {
		List<IEntityField> entityFields = IDeviceManageService.sort(entity.getEntityFields());
		
		//TODO: add sorting here

		List<String> invisibleFields = Arrays.asList(ENTITY_FIELD_SENSOR_ITEMS, ENTITY_FIELD_ID, ENTITY_FIELD_MESSAGE,
				entity.getDescriptionField());

		for (IEntityField ef : entityFields) {
			if (!invisibleFields.contains(ef.getName())) {

				builder.append((Optional.ofNullable(ef.getEmoji()).orElse(Emoji.EMPTY_EMOJI)).toString());
				builder.append(ef.getDescription());

				IEntityFieldEnabledValue enVal = ef.getEntityFieldEnabledValueByCurrentValue();

				if (enVal != null && enVal.getViewClass() != null && Boolean.class.equals(ef.getClazz())
						&& ef.getViewClass().isButton()) {

					builder.append(" ");
					builder.append(enVal.getEmoji());
					builder.append(" (");
					builder.append(enVal.getDescription());
					builder.append(")");

				} else {
					builder.append("  ");
					builder.append(ef.getValueStrByEntityFieldEnabledValueDescription());
					builder.append("  ");
					builder.append(ef.getMeasure());
				}
				builder.append("; ");
				if(ef.isVirtualized()) {
					builder.append("\n");
				}
			}
		}
		builder.append("\n");
	}

	public String buildDeviceGroupHeaderCommandsView(IDevice device, IGroup group) {
		// build header for device group command view.
		return itemTextHelper.getGroupCommandHeader(device.getEmoji().toString(), device.getDescription(),
				group.getEmoji().toString(), group.getDescription());
	}

	private boolean entityValueMapToTemplate(String entName, Set<IEntityField> entityFields,
			/* Set<IEntityField> extraFields, */ StringBuilder builder, boolean addNextLine,
			boolean renderAsTextIfFail) {
		if (entName == null || entityFields == null || entityFields.isEmpty()) {
			return true;
		}
		String result = "";

		try {
			String tmplPath = BotMessageConstants.BOT_TEMPLATES_PATH + entName + ".txt";

			try {
				URL resource = Resources.getResource(tmplPath);

				result = Resources.toString(resource, Charsets.UTF_8);

				result = fieldsToToTemplate(entityFields, result);

				// result = fieldsToToTemplate(extraFields, result);

			} catch (NullPointerException e) {
				// use standard renderer for item
				log.debug("No template for " + entName);

				if (renderAsTextIfFail) {
					result += fieldToStr(entityFields);
					// result += fieldToStr(extraFields);
					return true;
				} else {
					return false;
				}
			}

			if (addNextLine) {
				result += "\n";
			}
		} catch (IOException e) {
			log.error(e);
			result = entName + " " + fieldToStr(entityFields);
			return false;
		} catch (IllegalArgumentException e) {

			return false;
		}

		builder.append(result);
		return true;
	}

	private String fieldsToToTemplate(Collection<IEntityField> fields, String templateText) {
		if (fields != null && !fields.isEmpty()) {
			Map<String, String> keyReplacement = new HashMap<>();

			try {
				fields.stream().filter(IEntityField::isActive).forEach(ef -> {
					keyReplacement.put("_" + ef.getTemplateName() + "_", ef.getValueStr());
					keyReplacement.put("_" + ef.getTemplateName() + ".emoji_",
							(Optional.ofNullable(ef.getEmoji()).orElse(Emoji.EMPTY_EMOJI)).toString());
				});

				for (Entry<String, String> entry : keyReplacement.entrySet()) {
					templateText = templateText.replaceAll(entry.getKey(), entry.getValue());
				}
			} catch (Exception e) {
				log.debug("");
			}
		}

		return templateText;
	}

	protected String fieldToStr(Set<IEntityField> entityFields) {
		if (entityFields != null && !entityFields.isEmpty()) {
			StringBuilder buf = new StringBuilder();

			entityFields.stream().forEach(ef -> {
				buf.append(ef.getName());
				buf.append("=");
				buf.append(ef.getValueStr());
				buf.append(";");
			});

			return buf.toString();
		}

		return "";
	}
}
