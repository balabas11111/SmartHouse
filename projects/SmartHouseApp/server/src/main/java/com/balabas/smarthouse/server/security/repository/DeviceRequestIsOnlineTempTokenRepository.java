package com.balabas.smarthouse.server.security.repository;

import java.nio.charset.StandardCharsets;
import java.util.Date;
import java.util.UUID;

import org.springframework.stereotype.Repository;

import com.google.common.hash.Hashing;

@Repository(value = HashedValuesRepositoryImpl.DEVICE_TOKEN_REPO)
public class DeviceRequestIsOnlineTempTokenRepository extends HashedValuesRepositoryImpl {
	
	@Override
	public String hashValue(String value) {
		return Hashing.sha1().hashString(value, StandardCharsets.UTF_8).toString();
	}
	
	@Override
	public String hashSaltedValue(String salted) {
		return Hashing.sha256().hashString(salted, StandardCharsets.UTF_8).toString();
	}

	@Override
	public String saltValue(String value) {
		return UUID.randomUUID().toString()+":"+value+":"+Long.toString((new Date()).getTime());
	}

}
