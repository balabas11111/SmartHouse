/*
 * DisplayHelperPage.h
 *
 *  Created on: Dec 5, 2018
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_HELPERS_DisplayHelperPage_OLED_AbstractItem_H_
#define LIBRARIES_HELPERS_DisplayHelperPage_OLED_AbstractItem_H_

#include <DisplayHelper.h>
#include "Arduino.h"
#include "AbstractItem.h"
#include "DisplayHelperPage.h"


class DisplayHelperPage_OLED_AbstractItem:public DisplayHelperPage {
public:
	DisplayHelperPage_OLED_AbstractItem(AbstractItem* item){
		this->item=item;
	}
	virtual ~DisplayHelperPage_OLED_AbstractItem(){};

	String getName(){
		return item->getName();
	}

	uint8_t getItemCount(){
		return item->getItemCount();
	}
protected:
	boolean displayCurrentView(DisplayHelper* helper){
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

		return true;
	}
private:
	AbstractItem* item;
};

#endif /* LIBRARIES_HELPERS_DISPLAYHELPERPAGE_H_ */
