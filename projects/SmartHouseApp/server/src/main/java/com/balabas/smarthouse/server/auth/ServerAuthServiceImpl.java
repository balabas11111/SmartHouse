package com.balabas.smarthouse.server.auth;

import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.util.HashUtil;

import lombok.extern.log4j.Log4j2;

@Log4j2
@Service
public class ServerAuthServiceImpl implements InitializingBean, ServerAuthService {

	@Value("${smarthouse.server.key}")
    private String key;
	
	@Override
	public void afterPropertiesSet() throws Exception {
		this.key = HashUtil.getSha1(this.key);
		log.info("Key hashed ="+this.key);
	}
	
	@Override
	public boolean checkHashedKey(String keySha1) {
	    boolean result = HashUtil.compareSha1(this.key, keySha1);
	    if(!result){
	        log.error("Server key error expected = "+this.key+
                " actual"+keySha1);
	    }
		return result; 
	}

    @Override
    public String getServerKeyHashed() {
        return this.key;
    }

}
