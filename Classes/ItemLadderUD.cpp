#include "ItemLadderUD.h"

ItemLadderUD* ItemLadderUD::create(CCDictionary* dict){
	ItemLadderUD * pRet = new ItemLadderUD();
	if (pRet&&pRet->init(dict)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemLadderUD::init(CCDictionary* dict){
	Item::init();
	m_type = Item::IT_MUSHROOM;
	setPositionByProperty(dict);
	setDisplayFrameWithAnimationName("mushroomMoving", 0);
	return true;
}
