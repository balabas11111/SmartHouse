package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.annotation.PostConstruct;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.config.BeanDefinition;
import org.springframework.context.annotation.ClassPathScanningCandidateComponentProvider;
import org.springframework.core.type.filter.AnnotationTypeFilter;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.alarm.EntityFieldAlarmMarker;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.google.common.collect.Lists;

import lombok.extern.log4j.Log4j2;

@Service
@Log4j2
public class AlarmV2Service implements IAlarmV2Service {

	@Autowired
	IAlarmV2Repository alarmRepository;
	
	Map<AlarmMarker, List<Class<?>>> alarmTypes = new HashMap<>();
	
	@PostConstruct
	public void loadAllowedClasses() {
		String scanPackage = this.getClass().getPackage().getName();
		int i = 0;

		ClassPathScanningCandidateComponentProvider provider = createComponentScanner(AlarmMarker.class);
		for (BeanDefinition beanDef : provider.findCandidateComponents(scanPackage)) {
			try {
				Class clazz = Class.forName(beanDef.getBeanClassName());
				AlarmMarker marker = AlarmV2.getAlarmMarker(clazz);
				
				if(!alarmTypes.containsKey(marker)) {
					alarmTypes.put(marker, Lists.newArrayList());
				}
				
				alarmTypes.get(marker).add(clazz);

				i++;
			} catch (ClassNotFoundException e) {
				log.error(e);
			}
		}

		log.info("total classes cached as allowed for entityField Alarm =" + (i - 1));
	}
	
	private ClassPathScanningCandidateComponentProvider createComponentScanner(Class clazz) {
		ClassPathScanningCandidateComponentProvider provider = new ClassPathScanningCandidateComponentProvider(false);
		provider.addIncludeFilter(new AnnotationTypeFilter(clazz));
		return provider;
	}
}
