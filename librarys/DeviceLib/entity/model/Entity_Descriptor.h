/*
 * Entity_Descriptor.h
 *
 *  Created on: Feb 25, 2019
 *      Author: Vitaliy_Vlasov
 *
 *      contains all fields configuration for entity
 */

#ifndef LIBRARIES_DEVICELIB_ENTITY_MODEL_ENTITY_DESCRIPTOR_H_
#define LIBRARIES_DEVICELIB_ENTITY_MODEL_ENTITY_DESCRIPTOR_H_

#include <list>

class Entity_Descriptor {
public:
	Entity_Descriptor(){};
	virtual ~Entity_Descriptor(){};

	std::list<int> getLoadFields() const {
		return loadFields;
	}

	std::list<int> getSaveFields() const {
		return saveFields;
	}

	std::list<int> getSetFields() const {
		return setFields;
	}

	std::list<int> getStatFields() const {
		return statFields;
	}

	std::list<int> getTemplFields() const {
		return templFields;
	}

	std::list<int> getVarFields() const {
		return varFields;
	}

	std::list<int> getHiddenFields() const {
		return hiddenFields;
	}

	bool existsInList(std::list<int> lst,int val){
		for (std::list<int>::iterator it = lst.begin(); it != lst.end(); it++){
			if(val == (*it)){
				return true;
			}
		}
		return false;
	}

private:
	std::list<int> statFields;
	std::list<int> varFields;
	std::list<int> templFields;
	std::list<int> loadFields;
	std::list<int> saveFields;
	std::list<int> setFields;
	std::list<int> hiddenFields;
};

#endif /* LIBRARIES_DEVICELIB_ENTITY_MODEL_ENTITY_DESCRIPTOR_H_ */
