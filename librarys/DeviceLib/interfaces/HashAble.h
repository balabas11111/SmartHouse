/*
 * HashAble.h
 *
 *  Created on: Feb 14, 2019
 *      Author: Vitaliy_Vlasov
 *
 *      Interface provides item hash functionality
 *
 *      +can generate hash using parent item params
 */

#ifndef LIBRARIES_DEVICELIB_INTERFACES_HASHABLE_H_
#define LIBRARIES_DEVICELIB_INTERFACES_HASHABLE_H_

#include <Arduino.h>
#include <Hash.h>

class HashAble {
public:
	virtual ~HashAble(){};

	virtual String getHashableString()=0;

	boolean checkHash(String hash){
		return sha1(hash)==getHash();
	}

	virtual String getHash(){
		//if(hash==NULL || hash==nullptr || hash.length()==0){setHash();}
		return this->hash;
	}

	virtual void setHash(){
		this->hash = sha1(getHashableString());
	}
	virtual void setHash(String& parent){
		this->hash = hash;
	}
	virtual void setHash(HashAble* parent){
		String data=parent->getHashableString()+"_"+getHashableString();
		this->hash = sha1(data);
	}

	static String constructHash(String hashStr){
		return sha1(hashStr);
	}
protected:
	String hash;
};

#endif /* LIBRARIES_DEVICELIB_INTERFACES_HASHABLE_H_ */
