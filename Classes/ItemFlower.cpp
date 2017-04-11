#include "ItemFlower.h"
#include "Mario.h"
ItemFlower* ItemFlower::create(ValueMap& map)
{
	ItemFlower * pRet = new ItemFlower();
	if (pRet&&pRet->init(map)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemFlower::init(ValueMap& map)
{
	Item::init();
	_type = Item::IT_FLOWER;
	setPositionByProperty(map);
	//setDisplayFrameWithAnimationName("mushroomMoving", 0);

	
	m_bShow=true;//表现状态
	m_yTop=getPositionY();
	m_yButtom = m_yTop - 64;
//	m_yButtom=Common::mapPointToG_LBPoint(this->getMap(),
//										  ccp(0,11)).y;
	_speedY = 50;

	setLocalZOrder(0);
	setPositionX(getPositionX() - 8);

	this->runAction(CCRepeatForever::create(
		CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("flowerShow"))));


	//更新显示
	updateStatus();

	return true;
}
void ItemFlower::updateStatus(){

	if (m_bShow){
		scheduleOnce(schedule_selector(ItemFlower::cancelShowCallback),2.0f);

	}
}

void ItemFlower::cancelShowCallback(float){
	m_bShow = false;
	_speedY = -_speedY;
	
}
void ItemFlower::update(float dt){
	moveCheck(dt);
	collisionCheck(dt);
}
void ItemFlower::moveCheck(float dt){
	if (m_bShow)
		return;
	if (Mario::getInstance()->isDead())
		return;
	float targetY = getPositionY() + dt*_speedY;
	if (_speedY > 0){
		//正在上升
		CCRect rc =this->boundingBox();
		CCRect rcWill = rc;
		rcWill.origin.y += dt*_speedY;
		CCPoint ptMario = Mario::getInstance()->getPosition();
		//CCLOG("ptMario.y=%g,m_yTop=%g", ptMario.y, m_yTop);
		if (!Mario::getInstance()->isFly()&&rcWill.intersectsRect(Mario::getInstance()->boundingBox())){
			targetY = m_yButtom;
		}

	}


	if (targetY > m_yTop){
		m_bShow = true;
		targetY = m_yTop;
		
		updateStatus();
	}
	if (targetY < m_yButtom){
		targetY = m_yButtom;
		
		_speedY = -_speedY;
	}

	setPositionY(targetY);
}

void ItemFlower::collisionCheck(float dt){
	
	auto mario=Mario::getInstance();
	if (mario->isGodMode())
		return;
	if (mario->isDead())
		return;

	//碰撞检测
	CCRect rcItem = this->boundingBox();
	CCRect rcMario = mario->boundingBox();
	if (rcItem.intersectsRect(rcMario)){
		//CCLOG("2");
		mario->die();
	}

}