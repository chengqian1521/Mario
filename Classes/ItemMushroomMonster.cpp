#include "ItemMushroomMonster.h"
#include "Mario.h"

ItemMushroomMonster* ItemMushroomMonster::create(ValueMap& map)
{
	ItemMushroomMonster * pRet = new ItemMushroomMonster();
	if (pRet&&pRet->init(map)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = nullptr;
	}
	return pRet;

}


bool ItemMushroomMonster::init(ValueMap& map)
{
	ItemCanMove::init();
	_type = Item::IT_MushroomMonster;
	setPositionByProperty(map);
	setDisplayFrameWithAnimationName("mushroomMoving", 0);


	_speedY = 0;
	_speedX = -40;
	_isDead = false;
	this->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("mushroomMoving"))));
	return true;
}







void ItemMushroomMonster::collisionCheck(float dt){
	if (this->isOutOfWindow()){
		removeFromParent();
		return;
	}
	if (_isDead || Mario::getInstance()->isDead()){
		return;
	}
	Rect rcMario = Mario::getInstance()->boundingBox();
	Rect rcItem = this->boundingBox();

	if (rcMario.intersectsRect(rcItem)){
		if (Mario::getInstance()->getSpeedY() <= 0 && rcMario.getMinY()>rcItem.getMaxY() - rcItem.size.height / 2){
			//�����ʤ��
			this->stopAllActions();
			this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("mushroomDead1"));
			_speedX = 0;
			this->_isDead = true;
			
			this->runAction(CCMoveBy::create(1, ccp(0, -winSize.height)));
		}
		else{
			//���������
			if (Mario::getInstance()->isGodMode())
				return;
			
			Mario::getInstance()->die();
		}
	}

}