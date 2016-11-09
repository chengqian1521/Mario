#include "ItemFinalpoint.h"

ItemFinalpoint* ItemFinalpoint::create(CCDictionary* dict){
	ItemFinalpoint * pRet = new ItemFinalpoint();
	if (pRet&&pRet->init(dict)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemFinalpoint::init(CCDictionary* dict){
	Item::init();
	m_type = Item::IT_FINALPOINT;
	setPositionByProperty(dict);
	//setDisplayFrameWithAnimationName("mushroomMoving", 0);
	return true;
}
