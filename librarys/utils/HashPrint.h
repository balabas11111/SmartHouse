/*
 * HashPrint.h
 *
 *  Created on: 3 мар. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_UTILS_HASHPRINT_H_
#define LIBRARIES_UTILS_HASHPRINT_H_

#include "Arduino.h"
#include <FastCRC.h>
#include <Print.h>

class HashPrint : public Print {
public:
    HashPrint() {
      _hash = _hasher.crc32(NULL, 0);
    }

    virtual ~HashPrint(){};

    virtual size_t write(uint8_t c) {
        _hash = _hasher.crc32_upd(&c, 1);
        return _hash>0;
    }

    virtual size_t write(const uint8_t *data, uint16_t len) {
		_hash = _hasher.crc32_upd(data, len);
		return _hash>0;
	}

    unsigned int hash() const {
        return _hash;
    }

private:
    FastCRC32 _hasher;
    unsigned int _hash;
};

#endif /* LIBRARIES_UTILS_HASHPRINT_H_ */
