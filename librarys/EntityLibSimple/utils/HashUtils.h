/*
 * HashUtils.h
 *
 *  Created on: Nov 18, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_UTILS_HASHUTILS_H_
#define LIBRARIES_ENTITYLIBSIMPLE_UTILS_HASHUTILS_H_

#include <Arduino.h>

#include "rom/sha.h"
#include "esp_types.h"

	void encode_sha1(uint8_t * data, uint32_t size, uint8_t hash[20]) {

		SHA_CTX ctx;

	#ifdef DEBUG_SHA1
	    os_printf("DATA:");
	    for(uint16_t i = 0; i < size; i++) {
	        os_printf("%02X", data[i]);
	    }
	    os_printf("\n");
	    os_printf("DATA:");
	    for(uint16_t i = 0; i < size; i++) {
	        os_printf("%c", data[i]);
	    }
	    os_printf("\n");
	#endif

	    ets_sha_init(&ctx);
	    ets_sha_update(&ctx, SHA1, data, size);
	    ets_sha_finish(&ctx, SHA1, hash);

	#ifdef DEBUG_SHA1
	    os_printf("SHA1:");
	    for(uint16_t i = 0; i < 20; i++) {
	        os_printf("%02X", hash[i]);
	    }
	    os_printf("\n\n");
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
	String encode_sha1(uint8_t* data, uint32_t size) {
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
	String encode_sha1(const char* data, uint32_t size) {
	    return encode_sha1((uint8_t*) data, size);
	}

	String encode_sha1(String data) {
	    return encode_sha1(data.c_str(), data.length());
	}

#endif /* LIBRARIES_ENTITYLIBSIMPLE_UTILS_HASHUTILS_H_ */
