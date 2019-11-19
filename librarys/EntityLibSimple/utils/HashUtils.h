/*
 * HashUtils.h
 *
 *  Created on: Nov 18, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_UTILS_HASHUTILS_H_
#define LIBRARIES_ENTITYLIBSIMPLE_UTILS_HASHUTILS_H_

#include <Arduino.h>

#ifdef ESP8266
#include "Hash.h"
#endif
#ifdef ESP32
#include "rom/sha.h"
#include "esp_types.h"
#endif

class HashUtils {
public:
	static void encode_sha1(uint8_t * data, uint32_t size, uint8_t hash[20]) {

#ifdef ESP8266
		sha1(data, size, hash);
#endif
#ifdef ESP32
		SHA_CTX ctx;

	    ets_sha_init(&ctx);
	    ets_sha_update(&ctx, SHA1, data, size);
	    ets_sha_finish(&ctx, SHA1, hash);
#endif
	}
/*
	void encode_sha1(char * data, uint32_t size, uint8_t hash[20]) {
		encode_sha1((uint8_t *) data, size, hash);
	}

	void encode_sha1(const uint8_t * data, uint32_t size, uint8_t hash[20]) {
		encode_sha1((uint8_t *) data, size, hash);
	}

	void encode_sha1(const char * data, uint32_t size, uint8_t hash[20]) {
		encode_sha1((uint8_t *) data, size, hash);
	}

	void encode_sha1(String data, uint8_t hash[20]) {
		encode_sha1(data.c_str(), data.length(), hash);
	}
*/
	static String encode_sha1(uint8_t* data, uint32_t size) {
	    uint8_t hash[20];
	    String hashStr = "";

	    encode_sha1(&data[0], size, &hash[0]);

	    for(uint16_t i = 0; i < 20; i++) {
	        String hex = String(hash[i], HEX);
	        if(hex.length() < 2) {
	            hex = "0" + hex;
	        }
	        hashStr += hex;
	    }

	    return hashStr;
	}
	/*
	String encode_sha1(char* data, uint32_t size) {
	    return encode_sha1((uint8_t*) data, size);
	}

	String encode_sha1(const uint8_t* data, uint32_t size) {
	    return encode_sha1((uint8_t*) data, size);
	}
*/
	static String encode_sha1(const char* data, uint32_t size) {
	    return encode_sha1((uint8_t*) data, size);
	}

	static String encode_sha1(String data) {
	    return encode_sha1(data.c_str(), data.length());
	}
};


#endif /* LIBRARIES_ENTITYLIBSIMPLE_UTILS_HASHUTILS_H_ */
