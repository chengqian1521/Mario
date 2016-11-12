#include "ItemFlagpoint.h"

ItemFlagpoint* ItemFlagpoint::create(ValueMap& map)
{
	ItemFlagpoint * pRet = new ItemFlagpoint();
	if (pRet&&pRet->init(map)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemFlagpoint::init(ValueMap& map)
{
	Item::init();
	_type = Item::IT_FLAGPOINT;
	setPositionByProperty(map);

	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("flag.png");
	setTexture(texture);
	setTextureRect(CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height));
	setPositionX(getPositionX() - getContentSize().width/2);
	return true;
}

void ItemFlagpoint::autoDropFlag(){
	
	this->runAction(CCMoveBy::create (1, ccp(0, -150)));
}
