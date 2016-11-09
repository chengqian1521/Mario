#include "ItemFlower.h"
#include "Mario.h"
ItemFlower* ItemFlower::create(CCDictionary* dict){
	ItemFlower * pRet = new ItemFlower();
	if (pRet&&pRet->init(dict)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemFlower::init(CCDictionary* dict){
	Item::init();
	m_type = Item::IT_FLOWER;
	setPositionByProperty(dict);
	//setDisplayFrameWithAnimationName("mushroomMoving", 0);

	
	m_bShow=true;//表现状态
	m_yTop=getPositionY();
	m_yButtom = m_yTop - 64;
//	m_yButtom=Common::mapPointToG_LBPoint(this->getMap(),
//										  ccp(0,11)).y;
	m_speedY = 50;

	setZOrder(0);
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
	m_speedY = -m_speedY;
	
}

void ItemFlower::move(float dt){
	if (m_bShow)
		return;

	float targetY = getPositionY() + dt*m_speedY;
	if (m_speedY > 0){
		//正在上升
		CCRect rc =this->boundingBox();
		CCRect rcWill = rc;
		rcWill.origin.y += dt*m_speedY;
		CCPoint ptMario = sm_mario->getPosition();
		//CCLOG("ptMario.y=%g,m_yTop=%g", ptMario.y, m_yTop);
		if (!sm_mario->isFly()&&rcWill.intersectsRect(sm_mario->boundingBox())){
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
		
		m_speedY = -m_speedY;
	}

	setPositionY(targetY);
}

void ItemFlower::collisionCheck(float dt){
	
	if (sm_mario->isGodMode())
		return;
	//碰撞检测
	CCRect rcItem = this->boundingBox();
	CCRect rcMario = sm_mario->boundingBox();
	if (rcItem.intersectsRect(rcMario)){
		//CCLOG("2");
		sm_mario->die();
	}

}