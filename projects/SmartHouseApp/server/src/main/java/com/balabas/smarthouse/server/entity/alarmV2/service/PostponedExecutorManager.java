package com.balabas.smarthouse.server.entity.alarmV2.service;

import java.util.LinkedList;
import java.util.Queue;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.alarmV2.model.IAlarmV2;
import com.balabas.smarthouse.server.entity.model.IItemTypeable;

import lombok.extern.log4j.Log4j2;

@Service
@Log4j2
public class PostponedExecutorManager implements IPostponedExecutorManager {

	private Queue<IItemTypeable> items = new LinkedList<>();
	
	@Autowired
	private IAlarmV2Service alarmService;
	
	@Override
	public void postponeExecution(IItemTypeable item) {
		log.info("Postponed " + item);
		items.add(item);
	}

	@Override
	public void executeAllPostponed() {
		
		IItemTypeable item = items.poll();
		
		if(item == null) {
			return;
		}
		while(true) {
			execute(item);
			
			item = items.poll();
			
			if(item == null) {
				break;
			}
		}
		
		log.info("Postponed complete");
	}
	
	private void execute(IItemTypeable item) {
		if(item == null) {
			return;
		}
		
		if(IAlarmV2.class.isAssignableFrom(item.getClass())) {
			IAlarmV2 alarm = (IAlarmV2) item;
			alarmService.executePostponed(alarm);
			
			log.info("Execute postponed=" + item);
		}
	}

	
}
