/*
 * EntityService.h
 *
 *  Created on: Feb 15, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_SERVICES_ENTITYDAO_H_
#define LIBRARIES_DEVICELIB_SERVICES_ENTITYDAO_H_

#include <Arduino.h>
#include <ArduinoJson.h>

#include <utility>
#include <interfaces/Entity.h>
#include <interfaces/EntityConsts.h>

#include <list/ArrayListSearchAble.h>
/*
 * All available pages see EntityConsts
 *
enum Page: uint8_t {
	Page_list, Page_listVal, Page_save, Page_saveVal, Page_delete, Page_execute, Page_add
};

1. Entity DAO will execute page from DAO_PAGES_FINAL instead originally received

2. if POSTPROCESS is specified, result will be returned  as POSTPROCESS page

if page could be CACHED then on step 1 and no postprocess then

*/
const boolean DAO_PAGES_ALLOWED[] PROGMEM = {
	     true,         true,      true,         true,       false,        false,   false
};
const int DAO_PAGES_FINAL[] PROGMEM = {
	Page_list, Page_listVal, Page_save, Page_saveVal, Page_delete, Page_execute, Page_add
};
const boolean DAO_PAGES_CACHED[] PROGMEM = {
	     true,        false,      true,        false,       false,        false,    false
};
const int DAO_PAGES_POSTPROCESS_PAGE[] PROGMEM = {
	       -1,           -1,  Page_list,Page_listVal,   Page_list,    Page_list, Page_list
};

template <typename E, typename std::enable_if<std::is_base_of<Entity, E>::value>::type* = nullptr>
class EntityDAO {
public:
	typedef struct pageDescriptor{
	  int originId;
	  int pageId;
	  bool allowed;
	  bool found;
	  bool cached;
	  int postProcessPage;
	}EntityPageDescriptor;

	EntityDAO(ArrayListSearchAble<E>* list){
		this->list=list;
	}
	virtual ~EntityDAO(){};

	virtual int getServiceId(){
		return -1;
	}

	virtual String getServiceName(){
		if(getServiceId()<0){
			return NULL;
		}
		return FPSTR(ENTITY_NAME[getServiceId()]);
	}

	//returns overrided page Id or same value. -1 if pageId is not supported
	virtual EntityPageDescriptor getPageDescriptor(uint8_t pageId){
		  bool found = pageId>-1 && pageId<ARRAY_SIZE(getAllowedPages());

		  bool allowed = (found)?getAllowedPages()[pageId]:false;
		  int finalId = (allowed)?getFinalPages()[pageId]:false;
		  bool cached = (allowed)?getPageCached()[pageId]:false;
		  int postProcessPage = (allowed)?getPagePostprocess()[pageId]:false;

		  Serial.print(FPSTR("EntityDAO "));Serial.print(getServiceName());
		  Serial.print(FPSTR("pageId"));Serial.print(pageId);
		  Serial.print(FPSTR("finalId"));Serial.print(finalId);
		  Serial.print(FPSTR("allowed"));Serial.print(allowed);
		  Serial.print(FPSTR("found"));Serial.print(found);
		  Serial.print(FPSTR("cached"));Serial.print(cached);
		  Serial.print(FPSTR("postProcessPage"));Serial.print(postProcessPage);

		  return {pageId, finalId, allowed, found, cached, postProcessPage};
	}

	virtual String getCacheUrl(int8_t pageId){
		return "/cache/"+getServiceName()+"/"+String(pageId)+".dat";
	}

	virtual int loadFromFile(JsonObject& json);
	virtual int saveToFile(JsonObject& json);
protected:
	virtual boolean* getAllowedPages(){
		return DAO_PAGES_ALLOWED;
	}
	virtual int* getFinalPages(){
		return DAO_PAGES_FINAL;
	}
	virtual boolean* getPageCached(){
		return DAO_PAGES_CACHED;
	}
	virtual int* getPagePostprocess(){
		return DAO_PAGES_POSTPROCESS_PAGE;
	}
private:
	ArrayListSearchAble<E>* list;

};


#endif /* LIBRARIES_DEVICELIB_SERVICES_ENTITYSERVICE_H_ */
