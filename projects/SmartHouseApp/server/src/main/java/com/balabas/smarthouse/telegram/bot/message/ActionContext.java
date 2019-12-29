package com.balabas.smarthouse.telegram.bot.message;

import java.util.Optional;

import org.springframework.util.StringUtils;

import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.view.chart.IMetrics;

import lombok.Getter;

@SuppressWarnings("rawtypes")
public class ActionContext {

	@Getter
	private Emoji emoji;
	@Getter
	private String description;
	
	public ActionContext(IDevice device) {
		this.emoji = Optional.ofNullable(device.getEmoji()).orElse(Emoji.PAGER);
		this.description = device.getDescription();
		
		Emoji stateEmoji = device.getState().getStateEmoji(true);
		
		if (!Emoji.EMPTY_EMOJI.equals(stateEmoji)) {
    		emoji = stateEmoji;
    		description += " (" + device.getState().getStateDescription(true) + ")";
    	} 
	}
	
	public ActionContext(IEntity entity) {
		this.emoji = Optional.ofNullable(entity.getEmoji()).orElse(Emoji.PAGER);
		this.description = entity.getDescription();
	}
	
	public ActionContext(IEntityField entityField) {
		String description = entityField.getDescription();
		
		if(StringUtils.isEmpty(entityField.getDescription())
				&& !StringUtils.isEmpty(entityField.getDescriptionField())) {
			
			IEntityField descrField = entityField.getEntity().getEntityField(entityField.getDescriptionField());
			
			if(descrField!=null && descrField.getValue()!=null && !StringUtils.isEmpty(descrField.getValueStr())) {
				description = descrField.getValueStr();
			}
		}
		
		this.emoji = Optional.ofNullable(entityField.getEmoji()).orElse(Emoji.EMPTY_EMOJI);
		this.description = "(" + entityField.getName() + ") " + description;
	}
	
	public ActionContext(IMetrics metric) {
		this.emoji = Optional.ofNullable(metric.getEmoji()).orElse(Emoji.BAR_CHART);
		this.description = metric.getName() + " " + metric.getDescription();
	}
}
