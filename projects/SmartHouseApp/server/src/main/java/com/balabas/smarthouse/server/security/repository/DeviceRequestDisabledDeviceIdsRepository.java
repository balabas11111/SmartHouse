package com.balabas.smarthouse.server.security.repository;

import java.nio.charset.StandardCharsets;

import org.springframework.stereotype.Repository;

import com.google.common.hash.Hashing;

@Repository(value = HashedValuesRepositoryImpl.DISABLED_DEVICE_IDS_REPO)
public class DeviceRequestDisabledDeviceIdsRepository extends HashedValuesRepositoryImpl {
	
	@Override
	public String hashValue(String value) {
		return Hashing.sha1().hashString(value, StandardCharsets.UTF_8).toString();
	}
}
