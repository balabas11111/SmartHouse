/*
 * HashableItem.h
 *
 *  Created on: 20 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_INTERFACES_HASHABLEITEM_H_
#define LIBRARIES_DEVICELIB_INTERFACES_HASHABLEITEM_H_

#include <Arduino.h>
#include <Hash.h>

class HashableItem {
public:
	virtual ~HashableItem(){};

	virtual const char* buildHash(const char* hashStr){
			this->hash = sha1(hashStr,sizeof(hashStr)).c_str();
			return getHash();
		}
	virtual const char* getHash(){
		return this->hash;
	}
	virtual void setHash(const char* hash){
		this->hash=hash;
	}
private:
	const char* hash;

};

#endif /* LIBRARIES_DEVICELIB_INTERFACES_HASHABLEITEM_H_ */
