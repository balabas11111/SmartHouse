package com.balabas.smarthouse.server.entity.service;

import java.nio.charset.StandardCharsets;

import javax.annotation.PostConstruct;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import com.google.common.hash.Hashing;

import lombok.Getter;
import lombok.extern.log4j.Log4j2;

@Log4j2
@Service
public class DeviceSecurityServiceImpl implements DeviceSecurityService {

	@Value("${smarthouse.server.key}")
	private String serverKeyUnhashed;

	@Getter
	private String serverKey;

	@PostConstruct
	public void afterPropertiesSet() throws Exception {
		this.serverKey = Hashing.sha1().hashString(serverKeyUnhashed, StandardCharsets.UTF_8).toString();
		log.debug("Key hashed =" + this.serverKey);
	}

}
