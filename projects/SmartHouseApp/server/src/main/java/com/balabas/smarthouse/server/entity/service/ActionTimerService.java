package com.balabas.smarthouse.server.entity.service;

import java.util.HashMap;
import java.util.Map;

import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.alarmV2.model.IAlarmV2;
import com.balabas.smarthouse.server.entity.model.ActionTimer;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IGroup;
import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;

import lombok.extern.log4j.Log4j2;

@Service
@Log4j2
public class ActionTimerService implements IActionTimerService {

	private Map<ItemType, Map<Long, ActionTimer>> timers = new HashMap<>();
	
	
	@Override
	public ActionTimer getActionTimer(ItemType itemType, Long itemId) {
		return getActionTimer(itemType, itemId, null);
	}
	
	@Override
	public ActionTimer getActionTimer(ItemType itemType, Long itemId, Long updateInterval) {
		if(itemId==null || itemId==0) {
			//log.error("Cant  get ActionTimer: " + itemType.name() + " id=" + itemId);
			return null;
		}
		
		if(!timers.containsKey(itemType)) {
			Map<Long, ActionTimer> itemTypeTimers = new HashMap<>();
			timers.put(itemType, itemTypeTimers);
		}
		
		Map<Long, ActionTimer> itemTypeTimers = timers.get(itemType);
		
		if(!itemTypeTimers.containsKey(itemId)) {
			ActionTimer timer = buildTimer(itemType, updateInterval);
			if(timer==null) {
				return null;
			}
			
			itemTypeTimers.put(itemId, timer);
			log.info("New ActionTimer created for: " + itemType.name() + " id=" + itemId);
		}
		
		return itemTypeTimers.get(itemId);
	}

	@Override
	public ActionTimer getActionTimer(IDevice device) {
		return getActionTimer(ItemType.DEVICE, device.getId());
	}

	@Override
	public ActionTimer getActionTimer(IGroup group) {
		return getActionTimer(group.getType(), group.getId());
	}

	@Override
	public ActionTimer getActionTimer(IEntity entity) {
		return getActionTimer(ItemType.ENTITY, entity.getId());
	}
	
	@Override
	public ActionTimer getActionTimer(IAlarmV2 entity) {
		return getActionTimer(ItemType.ALARM, entity.getId(), entity.getMessageInterval().longValue()*1000);
	}

	@Override
	public void setActionSuccess(IDevice device) {
		setActionSuccess(getActionTimer(device));
	}

	@Override
	public void setActionSuccess(IGroup group) {
		setActionSuccess(getActionTimer(group));
	}

	@Override
	public void setActionSuccess(IEntity entity) {
		setActionSuccess(getActionTimer(entity));
	}

	@Override
	public void setActionSuccess(ActionTimer timer) {
		if(timer==null) {
			return;
		}
		
		timer.setActionSuccess();
	}

	private static ActionTimer buildTimer(ItemType itemType, Long updateInterval) {
		Long interval = updateInterval==null || updateInterval < 1 ? itemType.getRefreshInterval() : updateInterval;

		if (interval > 0) {
			ActionTimer updateTimer = new ActionTimer(interval);
			updateTimer.setActionForced(true);
			return updateTimer;
		}

		return null;
	}

	@Override
	public void setActionForced(IDevice device) {
		ActionTimer timer = getActionTimer(device);
		timer.setActionForced(true);
	}
	
	@Override
	public void setActionForced(IAlarmV2 alarm) {
		ActionTimer timer = getActionTimer(alarm);
		timer.setActionForced(true);
	}
	
	@Override
	public void initAlarmScheduling(IAlarmV2 alarm) {
		if(alarm.isRepeatable()) {
			getActionTimer(alarm).update(false);
		}
	}
	
	@Override
	public void nextAlarmScheduling(IAlarmV2 alarm) {
		if(alarm.isRepeatable()) {
			getActionTimer(alarm).update(false);
		}
	}
	
	@Override
	public boolean requireScheduledCheck(IAlarmV2 alarm) {
		if(!alarm.isRepeatable() || alarm.getItem() == null) {
			return false;
		}
		ActionTimer timer = getActionTimer(alarm);
		if(timer==null) {
			return false;
		}
		boolean result = timer.isTimeToExecuteAction();
		if(result) {
			log.info("Alarm check scheduled " + alarm.getId());
		}
		return result;
	}

}
