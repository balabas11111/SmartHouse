/*
 * UpdateAble.h
 *
 *  Created on: Mar 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIB_UPDATEABLE_H_
#define LIBRARIES_ENTITYLIB_UPDATEABLE_H_

class UpdateAble {
public:
	virtual ~UpdateAble(){};

	virtual void update()=0;
};

#endif /* LIBRARIES_ENTITYLIB_UPDATEABLE_H_ */
