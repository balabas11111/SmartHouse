/*
 * Measureable.h
 *
 *  Created on: 14 окт. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_MEASURER_MEASUREABLE_H_
#define LIBRARIES_MEASURER_MEASUREABLE_H_

#include "Arduino.h"

class Measureable{

public:

	Measureable(){};
	virtual ~Measureable(){};

	virtual Measureable* measure(){return (this);};

	Measureable(String _id,String _name,String _kind,String _val,String _descr){
		id=_id;
		name=_name;
		kind=_kind;
		val=_val;
		descr=_descr;
	}

	String id;
	String name;
	String kind;
	String val;
	String descr;
};

#endif /* LIBRARIES_MEASURER_MEASUREABLE_H_ */
