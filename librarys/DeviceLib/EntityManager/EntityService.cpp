/*
 * EntityService.cpp
 *
 *  Created on: Feb 4, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <EntityManager/EntityService.h>

EntityService::~EntityService() {
	// TODO Auto-generated destructor stub
}

 String EntityService::loadEntityCache(String parent,uint8_t eId){
	String fileName=constructEntityPath(parent, eId);
	return loadStringFromFile(fileName);
}
 String EntityService::loadEntityValueCache(String parent,uint8_t eId,uint8_t eBid,uint8_t fId){
	String fileName=constructEntityValuePath(parent, eId,eBid,fId);
	return loadStringFromFile(fileName);
}
 boolean EntityService::saveEntityCache(String parent,uint16_t eId,String data){
	String fileName=constructEntityPath(parent, eId);
	return saveStringToFile(fileName, data);
}
 boolean EntityService::saveEntityCacheByNames(String parent,String entityName,String data){
	String fileName=constructEntityPathByNames(parent, entityName);
	return saveStringToFile(fileName, data);
}
 boolean EntityService::saveEntityValueCache(String parent,uint8_t eId,uint8_t eBid,uint8_t fId,String data){
	String fileName=constructEntityValuePath(parent, eId,eBid,fId);
	return saveStringToFile(fileName, data);
}
 boolean EntityService::saveEntityValueCacheByNames(String parent,String entityName,String baseEntityName,String fieldName,String data){
	String fileName=constructEntityValuePathByNames(parent,entityName,baseEntityName,fieldName);
	return saveStringToFile(fileName, data);
}

 String EntityService::constructEntityPath(String parent,uint8_t eId){
	return parent+FPSTR(DIV)+entities[eId]->getName()+FPSTR(TXT);
}
 String EntityService::constructEntityPathByNames(String parent,String entityName){
	return parent+FPSTR(DIV)+entityName+FPSTR(TXT);
}

 String EntityService::constructEntityValuePath(String parent,uint8_t eId,uint8_t eBid,uint8_t fId){
	return parent+FPSTR(DIV)
			+entities[eId]->getName()+FPSTR(DIV)
			+entityFieldsAll[eBid]->getName()+FPSTR(DIV)
			+String(entityFieldsAll[eBid]->getKeys()[fId])+FPSTR(TXT);
}
 String EntityService::constructEntityValuePathByNames(String parent,String entityName,String baseEntityName,String fieldName){
	return parent+FPSTR(DIV)
			+entityName+FPSTR(DIV)
			+baseEntityName+FPSTR(DIV)
			+fieldName+FPSTR(TXT);
}

