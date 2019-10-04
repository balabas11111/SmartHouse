package com.balabas.smarthouse.server.alarm;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Repository;

import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.Entity;
import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;

import lombok.Getter;
import lombok.extern.log4j.Log4j2;

@Log4j2
@Repository
public class EntityMinMaxValueAlarmRepository implements AlarmRepositoryBaseValueContainer<Entity, EntityMinMaxValueAlarm>, InitializingBean  {

	@Getter
	private List<EntityMinMaxValueAlarm> alarms = new ArrayList<>();
	
	@Getter
	@Value("${smarthouse.server.repository.path}")
	private String storagePath = "repository/alarm/";
	
	private String getStorageFilePath(){
		return storagePath + this.getClass().getSimpleName()+".json";
	}
	
	@Override
	public void afterPropertiesSet() throws Exception {
		init();
	}
	
	@Override
	public void init() throws IOException {
		File file = new File(getStorageFilePath());
		if(file.exists()){
			 ObjectMapper mapper = new ObjectMapper();
			
			List<EntityMinMaxValueAlarm> loaded = mapper.readValue(file, new TypeReference<List<EntityMinMaxValueAlarm>>(){});
			
			alarms = loaded;
		}
	}
	
	@Override
	public void saveAlarms(List<EntityMinMaxValueAlarm> alarms) throws IOException {
		File file = new File(getStorageFilePath());
		
		ObjectMapper mapper = new ObjectMapper();
		mapper.writeValue(file, alarms);
		
		log.info("Alarms saved");
	}
	
	@Override
	public void activateAlarmsForDevice(Device device) {
		alarms.stream().filter(a->device.getDeviceId().equals(a.getDeviceId()))
		.forEach(alarm->
					device.getGroups().stream().flatMap(g->g.getEntities().stream())
					.filter(e->e.getName().equals(alarm.getItemName()))
					.forEach(e->{
						alarm.setItem(e);
						alarm.setActive(true);
						log.info("Alarm activated " + alarm.getItemName());
					})
				);
	}
	
	@Override
	public void activateAlarmsForItem(Entity entity) {
		alarms.stream()
			.filter(a-> !a.isActive() && a.getDeviceId().equals(entity.getDeviceId()) && a.getItemName().equals(entity.getName()))
			.forEach(alarm->{
						alarm.setItem(entity);
						alarm.setActive(true);
						log.info("Alarm activated " + alarm.getItemName());
					});
	}

	@Override
	public void putAlarm(EntityMinMaxValueAlarm alarm) {
		/*if(alarm.getItem() == null){
			throw new IllegalArgumentException("bad alarm");
		}
		*/
		int index = IntStream.range(0, alarms.size())
				.filter(i -> alarms.get(i).getItem()!=null && alarms.get(i).getItem().equals(alarm.getItem()))
				.findFirst().orElse(-1);
		
		if(index>-1){
			alarms.set(index, alarm);
			log.debug("Alarm refreshed");
		}else{
			alarms.add(alarm);
			log.debug("new alarm added");
		}
	}

	@Override
	public List<EntityMinMaxValueAlarm> getActiveAlarms() {
		return alarms.stream().filter(Alarm::isActive).collect(Collectors.toList());
	}

	@Override
	public List<EntityMinMaxValueAlarm> getActiveAlarms(String deviceId) {
		return alarms.stream().filter(a-> a.isActive() && a.getDeviceId().equals(deviceId)).collect(Collectors.toList());
	}

	@Override
	public List<EntityMinMaxValueAlarm> getActiveAlarms(String deviceId, String itemName) {
		return alarms.stream().filter(a-> a.isActive() && a.getDeviceId().equals(deviceId) && a.getItemName().equals(itemName)).collect(Collectors.toList());
	}
	
	@Override
	public List<EntityMinMaxValueAlarm> getAlarmsForItem(Entity entity) {
		return alarms.stream()
				.filter(a-> a.getDeviceId().equals(entity.getDeviceId()) && a.getItemName().equals(entity.getName()))
				.collect(Collectors.toList());
	}

}
