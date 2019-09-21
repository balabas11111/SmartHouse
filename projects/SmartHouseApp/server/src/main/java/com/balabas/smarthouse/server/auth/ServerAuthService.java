package com.balabas.smarthouse.server.auth;

public interface ServerAuthService {

	boolean checkHashedKey(String keySha1);
	
	String getServerKeyHashed();
}
