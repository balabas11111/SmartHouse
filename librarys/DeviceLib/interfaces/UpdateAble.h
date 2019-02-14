/*
 * UpdateAble.h
 *
 *  Created on: Feb 14, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_INTERFACES_UPDATEABLE_H_
#define LIBRARIES_DEVICELIB_INTERFACES_UPDATEABLE_H_

class UpdateAble {
public:
	virtual boolean update(){return true;};
};

#endif /* LIBRARIES_DEVICELIB_INTERFACES_UPDATEABLE_H_ */
