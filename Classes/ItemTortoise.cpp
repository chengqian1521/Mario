#include "ItemTortoise.h"
#include "Mario.h"

ItemTortoise* ItemTortoise::create(ValueMap& map)
{
	ItemTortoise * pRet = new ItemTortoise();
	if (pRet&&pRet->init(map)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemTortoise::init(ValueMap& map)
{
	ItemCanMove::init();
	_type = Item::IT_MushroomMonster;
	setPositionByProperty(map);
	//setDisplayFrameWithAnimationName("mushroomMoving", 0);
	
	m_initXV = -40;
	_speedY = 0;
	_speedX = m_initXV;
	m_bIsDead = false;
	_isGodMode = false;
	m_state = NORMAL;
	this->runAction(RepeatForever::create(CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("tortoiseLeftMoving"))));
	return true;
}
void ItemTortoise::moveCheck(float dt){
	if (m_state == SLEEP)
		return;
	
	if (canMoveHorizontal(dt)){
		moveHorizontal(dt);
	}
	else{
		
		_speedX = -_speedX;
		
		updateStatus();
	}

	if (canMoveDown(dt)){
		moveDown(dt);
	}
	else{
		_speedY = 0;
	}

	if (isOutOfWindow()){
		removeFromParent();
	}

}






void ItemTortoise::collisionCheck(float dt){
	if (m_bIsDead)
		return;
	if (_isGodMode)
		return;
	CCRect rcMario = Mario::getInstance()->boundingBox();
	CCRect rcItem = this->boundingBox();

	if (rcMario.intersectsRect(rcItem)){

		if (m_state==NORMAL){
			if (Mario::getInstance()->getSpeedY() <= 0 && rcMario.getMinY() > rcItem.getMaxY() - rcItem.size.height / 2){
				_speedX = 0;
				m_state = SLEEP;
				
				//无敌一段时间
				_isGodMode = true;
				
				//让马里奥弹出去
				Mario::getInstance()->jump(100);
				
				scheduleOnce(schedule_selector(ItemTortoise::cancelGodModeCallback), 0.2f);

				//过一段时间复活
				scheduleOnce(schedule_selector(ItemTortoise::reLiveCallback), 5.0f);

			}
			else{
				//马里奥死亡
				Mario::getInstance()->die(false);
			
			}
			return;
			
		}

		if (m_state==SLEEP){
			_speedX = Mario::getInstance()->getPositionX() < getPositionX() ?
				-m_initXV * 3 : m_initXV * 3;
			m_state = CRAZY;
			
			return;
		}
		if (m_state == CRAZY){
			if (Mario::getInstance()->getSpeedY() <= 0 && rcMario.getMinY() > rcItem.getMaxY() - rcItem.size.height / 2){
				this->stopAllActions();
				this->runAction(CCRepeatForever::create(CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("tortoiseDead"))));

				_speedX = 0;
				m_state = SLEEP;
				//无敌一段时间
				_isGodMode = true;
				scheduleOnce(schedule_selector(ItemTortoise::cancelGodModeCallback), 0.2f);

				//过一段时间复活
				scheduleOnce(schedule_selector(ItemTortoise::reLiveCallback), 0.2f);

			}
			else{
				if (Mario::getInstance()->isGodMode())
					return;
				Mario::getInstance()->die();

			}
			

			return;
		}
		
	}

}

void ItemTortoise::cancelGodModeCallback(float dt){
	_isGodMode = false;
}
void ItemTortoise::reLiveCallback(float dt){
	if (m_state == SLEEP){

		m_state = NORMAL;
		_speedX = m_initXV;
		stopAllActions();
		this->runAction(CCRepeatForever::create(CCAnimate::create(
			CCAnimationCache::sharedAnimationCache()
			->animationByName(_speedX > 0 ?
			"tortoiseRightMoving" : "tortoiseLeftMoving"))));

	}
}
void ItemTortoise::updateStatus(){

	if (m_state == NORMAL){
		stopAllActions();

		
	this->runAction(CCRepeatForever::create(CCAnimate::create(
				CCAnimationCache::sharedAnimationCache()
				->animationByName(_speedX > 0 ?
				"tortoiseRightMoving" : "tortoiseLeftMoving"))));

		

		
	}

}
