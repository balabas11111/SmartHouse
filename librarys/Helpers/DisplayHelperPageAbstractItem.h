/*
 * DisplayHelperPage.h
 *
 *  Created on: Dec 5, 2018
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_HELPERS_DISPLAYHELPERAbstractItemPAGE_H_
#define LIBRARIES_HELPERS_DISPLAYHELPERAbstractItemPAGE_H_

#include <DisplayHelper.h>
#include "Arduino.h"
#include "AbstractItem.h"
#include "DisplayHelperPage.h"


class DisplayHelperPageAbstractItem:public DisplayHelperPage {
public:
	DisplayHelperPageAbstractItem(AbstractItem* item){
		this->item=item;
	}
	virtual ~DisplayHelperPageAbstractItem(){};

	String getName(){
		return item->getName();
	}

	uint8_t getItemCount(){
		return item->getItemCount();
	}

	void printPage(DisplayHelper* helper){
		String str=item->getName();
		helper->displayLine(str,0,0);

		helper->displayLine("----------------",1,0);

		int row=helper->getStartRow();
		uint8_t dc=item->getItemCount();

		for(uint8_t i=0;i<dc;i++){
			str=item->getName(i);
			helper->displayLine(str,row,0);

			helper->displayLine(" :",row,8);

			str=item->getValStr(i);
			helper->displayLine(str,row,11);

			row+=helper->getRowIncrement();
		}

		helper->displayLine("----------------",row,0);
		row+=helper->getRowIncrement();

		if((dc)!=0){
			str=" (";
			str+=String(dc);
			str+=" sensors) ";
		}else{
			str=" (NO SENSORS)";
		}
		helper->displayLine(str,row,0);
	}
private:
	AbstractItem* item;
};

#endif /* LIBRARIES_HELPERS_DISPLAYHELPERPAGE_H_ */
