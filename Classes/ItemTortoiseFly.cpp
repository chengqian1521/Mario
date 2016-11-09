#include "ItemTortoiseFly.h"
#include "Mario.h"
ItemTortoiseFly* ItemTortoiseFly::create(CCDictionary* dict){
	ItemTortoiseFly * pRet = new ItemTortoiseFly();
	if (pRet&&pRet->init(dict)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemTortoiseFly::init(CCDictionary* dict){
	Item::init();
	m_type = Item::IT_tortoise_fly;
	setPositionByProperty(dict);
	
	m_flyDis = dict->valueForKey("flyDis")->intValue();
	m_status = NORMAL;
	m_speedX = -50;
	m_bIsGodMode = false;
	return true;
}

void ItemTortoiseFly::updateStatus(){
	
	if (m_status == NORMAL){
		stopAllActions();


		
		CCAnimation* animation1 = CCAnimationCache::sharedAnimationCache()->
			animationByName(m_speedX > 0 ? "tortoiseFlyRight" : "tortoiseFlyLeft");

		runAction(CCRepeatForever::create(CCAnimate::create(animation1)));

	}

}

void ItemTortoiseFly::move(float dt){
	if (m_status == NORMAL){
		if (canMoveHorizontal(dt)){
			moveHorizontal(dt);
		}

		
	}
	else if (m_status == DROPPING){
		if (canMoveDown(dt)){
			moveDown(dt);
		}
		else{
			m_speedY = 0;
		}
	}
	else if (m_status == ONLAND||m_status==CRAZY){
		if (canMoveHorizontal(dt)){
			moveHorizontal(dt);
		}

		if (canMoveDown(dt)){
			moveDown(dt);
		}
		else{
			m_speedY = 0;
		}
	
	}
	
}
void ItemTortoiseFly::collisionCheck(float dt){

	CCRect rcItem = boundingBox();
	CCRect rcMario = sm_mario->boundingBox();
	if (rcItem.intersectsRect(rcMario)){
		if (m_status == NORMAL){
				if (Item::sm_mario->getSpeedY() <= 0 && rcMario.getMinY() > rcItem.getMaxY() - rcItem.size.height / 2){
					
					m_speedX = 0;
					m_status = DROPPING;
					updateStatus();
					//�޵�һ��ʱ��
					beginGodMode(0.2f);

					//������µ���ȥ
					sm_mario->jump(100);
				}
				else{
					//���������
					sm_mario->die(false);
				}
			}
			else if (m_status == DROPPING){
				//�������µ��½����ٶ�С��   �ڹ���½��ٶ�
			}
			else if (m_status == ONLAND){
				if (Item::sm_mario->getSpeedY() <= 0 && rcMario.getMinY() > rcItem.getMaxY() - rcItem.size.height / 2){
					
					m_speedX = 0;
					m_status = SLEEP;
					updateStatus();
					//�޵�һ��ʱ��
					beginGodMode(0.2f);

					//��һ�Ḵ��
					//��һ��ʱ�临��
					scheduleOnce(schedule_selector(ItemTortoiseFly::reLiveCallback), 5.0f);

					//������µ���ȥ
					sm_mario->jump(100);

				}
				else{
					//���������
					sm_mario->die(false);
				}
			}

			else if (m_status == SLEEP){
				m_speedX = sm_mario->getPositionX() < getPositionX() ?
					150 :- 150;
				m_status= CRAZY;


			}
			else if (m_status == CRAZY){

			}
	
	}
}

void ItemTortoiseFly::reLiveCallback(float){

	m_status = NORMAL;
	m_speedX = 50;
	updateStatus();
}