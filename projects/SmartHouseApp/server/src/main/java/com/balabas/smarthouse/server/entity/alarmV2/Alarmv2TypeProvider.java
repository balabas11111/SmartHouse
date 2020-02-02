package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.annotation.PostConstruct;

import org.springframework.beans.factory.annotation.Autowired;
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
public class Alarmv2TypeProvider implements IAlarmv2TypeProvider {

	Map<String, List<Class<?>>> alarmTypes = new HashMap<>();
	
	Map<String, IAlarmStateChangeEventProcessor> stateChangeProcessors = new HashMap<String, IAlarmStateChangeEventProcessor>();
	
	@Autowired(required = false)
	Set<IAlarmStateChangeEventProcessor> stateChangedProcessors;
	
	@Override
	public List<Class<?>> getEnabledAlarms(IItemAbstract item) {
		String key = getAlarmClassKeyByClass(item);
		return alarmTypes.getOrDefault(key, Lists.newArrayList());
	}
	
	@PostConstruct
	public void loadAllowedClasses() {
		String scanPackage = this.getClass().getPackage().getName();
		int i = 0;

		ClassPathScanningCandidateComponentProvider provider = createComponentScanner(AlarmMarker.class);
		for (BeanDefinition beanDef : provider.findCandidateComponents(scanPackage)) {
			try {
				Class<?> clazz = Class.forName(beanDef.getBeanClassName());
				AlarmMarker marker = AlarmV2.getAlarmMarker(clazz);
				String key = getAlarmClassKeyByClass(marker);
				
				if(!alarmTypes.containsKey(key)) {
					alarmTypes.put(key, Lists.newArrayList());
				}
				
				alarmTypes.get(key).add(clazz);

				i++;
			} catch (ClassNotFoundException e) {
				log.error(e);
			}
		}

		log.info("total classes cached as allowed for entityField AlarmV2 =" + (i - 1));
		
		if(stateChangedProcessors!=null && stateChangedProcessors.size()>0) {
			stateChangedProcessors.stream().forEach(processor -> stateChangeProcessors.put(processor.getName(), processor));
		
		}
	}
	
	@SuppressWarnings({ "rawtypes", "unchecked" })
	private ClassPathScanningCandidateComponentProvider createComponentScanner(Class clazz) {
		ClassPathScanningCandidateComponentProvider provider = new ClassPathScanningCandidateComponentProvider(false);
		provider.addIncludeFilter(new AnnotationTypeFilter(clazz));
		return provider;
	}
	
	private static String getAlarmClassKeyByClass(AlarmMarker marker) {
		return getAlarmClassKeyByClass(marker.target(), marker.detail());
	}
	
	@SuppressWarnings("rawtypes")
	public static String getAlarmClassKeyByClass(IItemAbstract item) {
		Class<?> targetClass = item.getClass();
		Class<?> detailClass = null;
		
		if(EntityField.class.isAssignableFrom(targetClass)) {
			detailClass = ((EntityField)item).getClazz();
		}
		
		return getAlarmClassKeyByClass(targetClass, detailClass);
	}
	
	private static String getAlarmClassKeyByClass(Class<?> target, Class<?> detail) {
		String result = target.getSimpleName();
		
		if(EntityField.class.isAssignableFrom(target) && detail!=null) {
			result += "<" + detail.getSimpleName() + ">";
		}
		
		return result;
	}

	@Override
	public IAlarmStateChangeEventProcessor getAlarmStateChangedEventProcessor(IAlarmStateChangeEvent event) {
		return stateChangeProcessors.getOrDefault(event.getChangeAction().getTargetBeanName(), null);
	}

}
