#include "ItemDarkCloud.h"

ItemDarkCloud* ItemDarkCloud::create(CCDictionary* dict){
	ItemDarkCloud * pRet = new ItemDarkCloud();
	if (pRet&&pRet->init(dict)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemDarkCloud::init(CCDictionary* dict){
	Item::init();
	m_type = Item::IT_MUSHROOM;
	setPositionByProperty(dict);
	setDisplayFrameWithAnimationName("mushroomMoving", 0);
	return true;
}
