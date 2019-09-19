package com.balabas.smarthouse.server.util;

import java.io.UnsupportedEncodingException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Formatter;

import lombok.extern.log4j.Log4j2;

@Log4j2
public class HashUtil {

	private HashUtil(){}
	
	public static boolean compareSha1(String val1, String val2 ){
		log.info("compare val1 ="+val1+" val2="+val2);
		
		return val1.equalsIgnoreCase(val2);
	}
	
	
	public static String getSha1(String str)
	{
	    String sha1 = "";
	    try
	    {
	        MessageDigest crypt = MessageDigest.getInstance("SHA-1");
	        crypt.reset();
	        crypt.update(str.getBytes("UTF-8"));
	        sha1 = byteToHex(crypt.digest());
	    }
	    catch(NoSuchAlgorithmException | UnsupportedEncodingException e)
	    {
	        log.trace(e);
	    }
	    return sha1;
	}
	
	private static String byteToHex(final byte[] hash)
	{
	    Formatter formatter = new Formatter();
	    for (byte b : hash)
	    {
	        formatter.format("%02x", b);
	    }
	    String result = formatter.toString();
	    formatter.close();
	    return result;
	}
}
