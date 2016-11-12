#include "ItemTortoiseFly.h"
#include "Mario.h"
ItemTortoiseFly* ItemTortoiseFly::create(ValueMap& map)
{
	ItemTortoiseFly * pRet = new ItemTortoiseFly();
	if (pRet&&pRet->init(map)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemTortoiseFly::init(ValueMap& map)
{
	ItemCanMove::init();
	_type = Item::IT_tortoise_fly;
	setPositionByProperty(map);
	
	m_flyDis = map.at("flyDis").asInt();
	m_status = NORMAL;
	_speedX = -50;
	_isGodMode = false;
	return true;
}

void ItemTortoiseFly::updateStatus(){
	
	if (m_status == NORMAL){
		stopAllActions();


		
		CCAnimation* animation1 = CCAnimationCache::sharedAnimationCache()->
			animationByName(_speedX > 0 ? "tortoiseFlyRight" : "tortoiseFlyLeft");

		runAction(CCRepeatForever::create(CCAnimate::create(animation1)));

	}

}

void ItemTortoiseFly::moveCheck(float dt){
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
			_speedY = 0;
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
			_speedY = 0;
		}
	
	}
	
}
void ItemTortoiseFly::collisionCheck(float dt){

	CCRect rcItem = boundingBox();
	CCRect rcMario = Mario::getInstance()->boundingBox();
	if (rcItem.intersectsRect(rcMario)){
		if (m_status == NORMAL){
				if (Mario::getInstance()->getSpeedY() <= 0 && rcMario.getMinY() > rcItem.getMaxY() - rcItem.size.height / 2){
					
					_speedX = 0;
					m_status = DROPPING;
					updateStatus();
					//�޵�һ��ʱ��
					beginGodMode(0.2f);

					//������µ���ȥ
					Mario::getInstance()->jump(100);
				}
				else{
					//���������
					Mario::getInstance()->die(false);
				}
			}
			else if (m_status == DROPPING){
				//�������µ��½����ٶ�С��   �ڹ���½��ٶ�
			}
			else if (m_status == ONLAND){
				if (Mario::getInstance()->getSpeedY() <= 0 && rcMario.getMinY() > rcItem.getMaxY() - rcItem.size.height / 2){
					
					_speedX = 0;
					m_status = SLEEP;
					updateStatus();
					//�޵�һ��ʱ��
					beginGodMode(0.2f);

					//��һ�Ḵ��
					//��һ��ʱ�临��
					scheduleOnce(schedule_selector(ItemTortoiseFly::reLiveCallback), 5.0f);

					//������µ���ȥ
					Mario::getInstance()->jump(100);

				}
				else{
					//���������
					Mario::getInstance()->die(false);
				}
			}

			else if (m_status == SLEEP){
				_speedX = Mario::getInstance()->getPositionX() < getPositionX() ?
					150 :- 150;
				m_status= CRAZY;


			}
			else if (m_status == CRAZY){

			}
	
	}
}

void ItemTortoiseFly::reLiveCallback(float){

	m_status = NORMAL;
	_speedX = 50;
	updateStatus();
}