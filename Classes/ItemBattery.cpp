#include "ItemBattery.h"

ItemBattery* ItemBattery::create(CCDictionary* dict){
	ItemBattery * pRet = new ItemBattery();
	if (pRet&&pRet->init(dict)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemBattery::init(CCDictionary* dict){
	Item::init();
	m_type = Item::IT_MUSHROOM;
	setPositionByProperty(dict);
	setDisplayFrameWithAnimationName("mushroomMoving", 0);
	return true;
}
