package com.balabas.smarthouse.telegram.bot.message;

import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.function.Function;
import java.util.stream.Collectors;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.telegram.telegrambots.meta.api.methods.send.SendMessage;

import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.descriptor.EntityClass;
import com.balabas.smarthouse.telegram.bot.renderers.EntityRenderer;
import com.google.common.collect.Lists;

@Component
public class ItemRendererBuilder {

	private Map<EntityClass, EntityRenderer<?>> renderers;
	
	@Autowired
	public ItemRendererBuilder(Set<EntityRenderer<?>> renderers) {
		this.renderers = renderers.stream().collect(Collectors.toMap(EntityRenderer::getType, Function.identity()));
	}
	
	@SuppressWarnings({ "rawtypes", "unchecked" })
	public List<SendMessage> build(Set<IEntity> items, Long chatId){
		List<SendMessage> result = Lists.newArrayList();
		
		items.stream().forEach(item ->{
			EntityRenderer renderer = renderers.getOrDefault(item.getRenderer(), null);
			if(renderer!=null) {
				result.add(renderer.render(item, chatId));
			}
			
		});
		 
		
		return result;
	}
}
