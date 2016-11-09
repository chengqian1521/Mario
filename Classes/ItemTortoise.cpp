#include "ItemTortoise.h"
#include "Mario.h"

ItemTortoise* ItemTortoise::create(CCDictionary* dict){
	ItemTortoise * pRet = new ItemTortoise();
	if (pRet&&pRet->init(dict)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemTortoise::init(CCDictionary* dict){
	Item::init();
	m_type = Item::IT_MUSHROOM;
	setPositionByProperty(dict);
	//setDisplayFrameWithAnimationName("mushroomMoving", 0);
	
	m_initXV = -40;
	m_speedY = 0;
	m_speedX = m_initXV;
	m_bIsDead = false;
	m_bIsGodMode = false;
	m_state = NORMAL;
	this->runAction(CCRepeatForever::create(CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("tortoiseLeftMoving"))));
	return true;
}
void ItemTortoise::move(float dt){
	if (m_state == SLEEP)
		return;
	
	if (canMoveHorizontal(dt)){
		moveHorizontal(dt);
	}
	else{
		
		m_speedX = -m_speedX;
		
		updateStatus();
	}

	if (canMoveDown(dt)){
		moveDown(dt);
	}
	else{
		m_speedY = 0;
	}

	if (isOutOfWindow()){
		removeFromParent();
	}

}






void ItemTortoise::collisionCheck(float dt){
	if (m_bIsDead)
		return;
	if (m_bIsGodMode)
		return;
	CCRect rcMario = Item::sm_mario->boundingBox();
	CCRect rcItem = this->boundingBox();

	if (rcMario.intersectsRect(rcItem)){

		if (m_state==NORMAL){
			if (Item::sm_mario->getSpeedY() <= 0 && rcMario.getMinY() > rcItem.getMaxY() - rcItem.size.height / 2){
				m_speedX = 0;
				m_state = SLEEP;
				
				//无敌一段时间
				m_bIsGodMode = true;
				
				//让马里奥弹出去
				sm_mario->jump(100);
				
				scheduleOnce(schedule_selector(ItemTortoise::cancelGodModeCallback), 0.2f);

				//过一段时间复活
				scheduleOnce(schedule_selector(ItemTortoise::reLiveCallback), 5.0f);

			}
			else{
				//马里奥死亡
				sm_mario->die(false);
			
			}
			return;
			
		}

		if (m_state==SLEEP){
			m_speedX = sm_mario->getPositionX() < getPositionX() ?
				-m_initXV * 3 : m_initXV * 3;
			m_state = CRAZY;
			
			return;
		}
		if (m_state == CRAZY){
			if (Item::sm_mario->getSpeedY() <= 0 && rcMario.getMinY() > rcItem.getMaxY() - rcItem.size.height / 2){
				this->stopAllActions();
				this->runAction(CCRepeatForever::create(CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("tortoiseDead"))));

				m_speedX = 0;
				m_state = SLEEP;
				//无敌一段时间
				m_bIsGodMode = true;
				scheduleOnce(schedule_selector(ItemTortoise::cancelGodModeCallback), 0.2f);

				//过一段时间复活
				scheduleOnce(schedule_selector(ItemTortoise::reLiveCallback), 0.2f);

			}
			else{
				if (sm_mario->isGodMode())
					return;
				sm_mario->die();

			}
			

			return;
		}
		
	}

}

void ItemTortoise::cancelGodModeCallback(float dt){
	m_bIsGodMode = false;
}
void ItemTortoise::reLiveCallback(float dt){
	if (m_state == SLEEP){

		m_state = NORMAL;
		m_speedX = m_initXV;
		stopAllActions();
		this->runAction(CCRepeatForever::create(CCAnimate::create(
			CCAnimationCache::sharedAnimationCache()
			->animationByName(m_speedX > 0 ?
			"tortoiseRightMoving" : "tortoiseLeftMoving"))));

	}
}
void ItemTortoise::updateStatus(){

	if (m_state == NORMAL){
		stopAllActions();

		
	this->runAction(CCRepeatForever::create(CCAnimate::create(
				CCAnimationCache::sharedAnimationCache()
				->animationByName(m_speedX > 0 ?
				"tortoiseRightMoving" : "tortoiseLeftMoving"))));

		

		
	}

}
