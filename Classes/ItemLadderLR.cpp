#include "ItemLadderLR.h"
#include "Mario.h"
ItemLadderLR* ItemLadderLR::create(CCDictionary* dict){
	ItemLadderLR * pRet = new ItemLadderLR();
	if (pRet&&pRet->init(dict)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemLadderLR::init(CCDictionary* dict){
	Item::init();
	m_type = Item::IT_ladderLR;
	m_dist = 0;
	setPositionX(getPositionX() - 30);
	setPositionByProperty(dict);
	CCTexture2D* texture = CCTextureCache::sharedTextureCache()
		->addImage("ladder.png");
	setTexture(texture);
	setTextureRect(CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height));

	//һ��ʼ��������
	m_lorR=dict->valueForKey("LorR")->intValue();

	//�涨�����ٶ�Ϊ��
	m_speedX = m_lorR? 20:-20;
	

	//�ڶ�����
	m_ladderDis = dict->valueForKey("ladderDis")->intValue();
	m_dist = m_lorR ? 0 : m_ladderDis;
	m_bIsMarioOnThis = false;
	return true;
}

void ItemLadderLR::move(float dt){
	m_dist += dt*m_speedX;
	setPositionX(getPositionX() + dt*m_speedX);
	
	if (m_dist >= m_ladderDis || m_dist<=0){
		m_speedX = -m_speedX;
	}
}
void ItemLadderLR::collisionCheck(float dt){
	CCRect rcMario = Mario::getInstance()->boundingBox();
	CCRect rcLadder = boundingBox();
	if (rcMario.intersectsRect(rcLadder)){
		m_bIsMarioOnThis = true;
		if (Mario::getInstance()->isOnLadder()){
			//��������һ��
			Mario::getInstance()->setPositionX(Mario::getInstance()->getPositionX() + dt*m_speedX);
		}
		else{
			if (Mario::getInstance()->isFly()){
				if (Mario::getInstance()->getSpeedY() > 0){
					//�Դ�ײ����

					//������ȥ
					Mario::getInstance()->reverseSpeedY();
				}
				else if (Mario::getInstance()->getSpeedY() < 0){
					//����������
					Mario::getInstance()->setIsOnLadder(true);
					Mario::getInstance()->setSpeedY(0);
					Mario::getInstance()->setIsFly(false);
				}
				else{

				}
			}
			else{
				//����������
				Mario::getInstance()->setIsOnLadder(true);
			}
		
		}
	}
	else{
		if (m_bIsMarioOnThis){
			Mario::getInstance()->setIsOnLadder(false);
			m_bIsMarioOnThis = false;
		}
		
			
	}
	
}