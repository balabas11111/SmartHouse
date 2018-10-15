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

	Measureable(String _id,String _name,String _kind,String _val){
		id=_id;
		name=_name;
		kind=_kind;
		val=_val;
	}
	String id;
	String name;
	String kind;
	String val;
};

#endif /* LIBRARIES_MEASURER_MEASUREABLE_H_ */
