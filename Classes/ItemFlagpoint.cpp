#include "ItemFlagpoint.h"

ItemFlagpoint* ItemFlagpoint::create(CCDictionary* dict){
	ItemFlagpoint * pRet = new ItemFlagpoint();
	if (pRet&&pRet->init(dict)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemFlagpoint::init(CCDictionary* dict){
	Item::init();
	m_type = Item::IT_FLAGPOINT;
	setPositionByProperty(dict);

	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("flag.png");
	setTexture(texture);
	setTextureRect(CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height));
	setPositionX(getPositionX() - getContentSize().width/2);
	return true;
}

void ItemFlagpoint::autoDropFlag(){
	
	this->runAction(CCMoveBy::create (1, ccp(0, -150)));
}
