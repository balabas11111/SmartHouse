package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.annotation.PostConstruct;

import org.springframework.beans.factory.config.BeanDefinition;
import org.springframework.context.annotation.ClassPathScanningCandidateComponentProvider;
import org.springframework.core.type.filter.AnnotationTypeFilter;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityField;
import com.google.common.collect.Lists;

import lombok.extern.log4j.Log4j2;

@Service
@Log4j2
@SuppressWarnings("rawtypes")
public class AlarmTypeProvider implements IAlarmTypeProvider {

	Map<String, List<Class<?>>> alarmTypes = new HashMap<>();
	
	@PostConstruct
	public void loadAllowedClasses() {
		String scanPackage = this.getClass().getPackage().getName();
		int i = 0;

		ClassPathScanningCandidateComponentProvider provider = createComponentScanner(AlarmMarker.class);
		for (BeanDefinition beanDef : provider.findCandidateComponents(scanPackage)) {
			try {
				Class clazz = Class.forName(beanDef.getBeanClassName());
				AlarmMarker marker = AlarmV2.getAlarmMarker(clazz);
				String itemKey = getItemClassKey(marker);
				
				if(!alarmTypes.containsKey(itemKey)) {
					alarmTypes.put(itemKey, Lists.newArrayList());
				}
				
				alarmTypes.get(itemKey).add(clazz);

				i++;
			} catch (ClassNotFoundException e) {
				log.error(e);
			}
		}

		log.info("total classes cached as allowed for entityField Alarm =" + (i - 1));
	}
	
	@SuppressWarnings("unchecked")
	private ClassPathScanningCandidateComponentProvider createComponentScanner(Class clazz) {
		ClassPathScanningCandidateComponentProvider provider = new ClassPathScanningCandidateComponentProvider(false);
		provider.addIncludeFilter(new AnnotationTypeFilter(clazz));
		return provider;
	}
	
	private String getItemClassKey(AlarmMarker marker) {
		StringBuilder builder = new StringBuilder();
		
		Class<?> target = marker.target();
		
		Class<?> detail = marker.detail();
		
		builder.append(target.getSimpleName());
		
		if(EntityField.class.isAssignableFrom(target)) {
			builder.append(detail.getSimpleName());
		}
		
		return builder.toString();
	}
	
	@Override
	public List<Class<?>> getEnabledAlarms(IItemAbstract item) {
		return alarmTypes.get(item.getItemClassId());
	}
}
