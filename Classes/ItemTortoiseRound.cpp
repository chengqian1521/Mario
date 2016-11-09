#include "ItemTortoiseRound.h"
#include "Mario.h"
ItemTortoiseRound* ItemTortoiseRound::create(CCDictionary* dict){
	ItemTortoiseRound * pRet = new ItemTortoiseRound();
	if (pRet&&pRet->init(dict)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemTortoiseRound::init(CCDictionary* dict){
	Item::init();
	m_type = Item::IT_tortoise_round;
	setPositionByProperty(dict);
	
	m_roundDis = dict->valueForKey("roundDis")->intValue();
	m_speedY = 0;
	m_speedX = -50;
	m_state  = NORMAL;
	m_bIsGodMode = false;
	m_initXV = 50;
	updateStatus();
	return true;
}

void ItemTortoiseRound::move(float dt){
	if (!isAppearInWindow())
		return;
	if (m_state == NORMAL){

		static float dis = m_roundDis;
		dis += dt*m_speedX;
		setPositionX(getPositionX() + dt*m_speedX);
		if (dis >= m_roundDis || dis <= 0){
			m_speedX = -m_speedX;
			updateStatus();
		}
	}
	else if (m_state == CRAZY){
		if (canMoveHorizontal(dt)){
			moveHorizontal(dt);
		}
		if (canMoveDown(dt)){
			moveDown(dt);
			m_speedY -= sm_g;
		}
		else{
			m_speedY = 0;
		}
	}
}
void ItemTortoiseRound::collisionCheck(float dt){
	CCLOG("%g,%g",getPositionX(),getPositionY());
	if (!isAppearInWindow())
		return;
	if (m_bIsGodMode)
		return;
	Mario* mario = Mario::getInstance();
	CCRect rcMario = mario->boundingBox();
	CCRect rcItem = this->boundingBox();

	if (rcMario.intersectsRect(rcItem)){

		if (m_state == NORMAL){
			if (mario->getSpeedY() <= 0 && rcMario.getMinY() > rcItem.getMaxY() - rcItem.size.height / 2){
				m_speedX = 0;
				m_state = SLEEP;
				updateStatus();
				//无敌一段时间
				m_bIsGodMode = true;

				//让马里奥弹出去
				mario->jump(100);

				scheduleOnce(schedule_selector(ItemTortoiseRound::cancelGodModeCallback), 0.2f);

				//过一段时间复活
				scheduleOnce(schedule_selector(ItemTortoiseRound::autoReLiveCallback), 5.0f);

			}
			else{
				//马里奥死亡
				CCLOG("gama_over");

			}

			return;

		}

		if (m_state == SLEEP){
			m_speedX = sm_mario->getPositionX() < getPositionX() ?
				-m_initXV * 3 : m_initXV * 3;
			m_state = CRAZY;
			//无敌一段时间
			m_bIsGodMode = true;
			scheduleOnce(schedule_selector(ItemTortoiseRound::cancelGodModeCallback), 0.2f);
			//scheduleOnce(schedule_selector(ItemTortoiseRound::autoReLiveCallback), 5.0f);

			updateStatus();
			return;
		}
		if (m_state == CRAZY){
			if (Item::sm_mario->getSpeedY() <= 0 && rcMario.getMinY() > rcItem.getMaxY() - rcItem.size.height / 2){
				
				m_speedX = 0;
				m_state = SLEEP;
				//无敌一段时间
				m_bIsGodMode = true;
				scheduleOnce(schedule_selector(ItemTortoiseRound::cancelGodModeCallback), 0.2f);

				//过一段时间复活
				scheduleOnce(schedule_selector(ItemTortoiseRound::autoReLiveCallback), 0.2f);

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
void ItemTortoiseRound::updateStatus(){
	stopAllActions();
	if (m_state == NORMAL){
		CCAnimation* ani = CCAnimationCache::sharedAnimationCache()->
			animationByName(m_speedX>0 ? "tortoiseRightMoving" : "tortoiseLeftMoving");
		runAction(CCRepeatForever::create(CCAnimate::create(ani)));
	}
	else if (m_state == SLEEP || m_state==CRAZY){
		CCAnimation* ani = CCAnimationCache::sharedAnimationCache()->
			animationByName("tortoiseDead");
		runAction(CCRepeatForever::create(CCAnimate::create(ani)));

	}
}

void ItemTortoiseRound::autoReLiveCallback(float dt){
	m_state = NORMAL;
	m_speedX = m_initXV;
	updateStatus();
}
void ItemTortoiseRound::cancelGodModeCallback(float dt){
	m_bIsGodMode = false;
}