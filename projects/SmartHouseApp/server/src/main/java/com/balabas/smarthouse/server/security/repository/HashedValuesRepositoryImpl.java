package com.balabas.smarthouse.server.security.repository;

import java.util.Map;

import com.google.common.collect.Maps;

import lombok.Getter;

public abstract class HashedValuesRepositoryImpl implements HashedValuesRepository {

	public static final String DISABLED_DEVICE_IDS_REPO = "disabledDeviceIdsRepo";
	
	public static final String DEVICE_TOKEN_REPO = "deviceTokenRepo";
	
	public static final String SERVER_TOKEN_REPO = "serverTokenRepo";
	
	@Getter
	private Map<String, String> valueHashMap;
	
	@Getter
	private Map<String, String> hashValueMap;
	
	public abstract String hashValue(String value);
	
	protected void initMaps() {
		this.valueHashMap = Maps.newHashMap();
		this.hashValueMap = Maps.newHashMap();
	}
	
	@Override
	public void afterPropertiesSet() throws Exception {
		initMaps();
	}

}
