#include "ItemMushroom.h"
#include "Mario.h"

ItemMushroom* ItemMushroom::create(ValueMap& map)
{
	ItemMushroom * pRet = new ItemMushroom();
	if (pRet&&pRet->init(map)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemMushroom::init(ValueMap& map)
{
	ItemCanMove::init();
	_type = Item::IT_MUSHROOMREWARD;
	setPositionByProperty(map);
	
	setVisible(false);
	this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("rewardMushroom"));
	setZOrder(0);
	_state = SLEEP;
	_speedY = 0;
	_speedX = 40;
	return true;
}
void ItemMushroom::wakeup(){
	_state = GROW;
	updateStatus();
}

void ItemMushroom::updateStatus(){
	if (_state == GROW){
		CCMoveBy* by = CCMoveBy::create(1.0f, ccp(0, 16));
		CCCallFunc* callfunc = CCCallFunc::create(this, callfunc_selector(ItemMushroom::afterGrowCallback));
		runAction(CCSequence::create(by,callfunc,NULL));
	}
	else if (_state == MOVE){
		
	}
}

void ItemMushroom::afterGrowCallback(){
	_state = MOVE;
	setLocalZOrder(100);
	updateStatus();
}

void ItemMushroom::collisionCheck(float dt){
	if (_state != MOVE)
		return;
	if (Mario::getInstance()->boundingBox().intersectsRect(this->boundingBox())){
		Mario::getInstance()->eatMushroom(this->_type);
		
		removeFromParent();
		
	}
}

void ItemMushroom::update(float dt){
	if (this->_state== State::MOVE)
			moveCheck(dt);
	collisionCheck(dt);
}