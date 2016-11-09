#include "ItemBoss.h"
#include "ItemBullet.h"

ItemBoss* ItemBoss::create(CCDictionary* dict){
	ItemBoss * pRet = new ItemBoss();
	if (pRet&&pRet->init(dict)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemBoss::init(CCDictionary* dict){
	Item::init();
	m_type = Item::IT_boss;
	setPositionByProperty(dict);
	
	m_left=	getPositionX()-4*16;
	m_right=getPositionX()+4*16;
	m_status = NORMAL;
	m_speedX = 40;
	m_speedY = 0;
	updateStatus();
	scheduleOnce(schedule_selector(ItemBoss::jumpCallback), 3.0f);
	return true;
}

void ItemBoss::move(float dt){
	if (m_status == NORMAL){
		setPositionX(getPositionX()+dt*m_speedX);
		if (getPositionX() > m_right||getPositionX()<m_left){
			m_speedX = -m_speedX;
			updateStatus();
		}
		if (canMoveDown(dt)){
			moveDown(dt);
			m_speedY -= sm_g;
		}
		else{
			m_speedY = 0;
		}

		//���Ŷϵ���,�л�ΪDROPPING״̬
	}
	else if (m_status == DROPPING){
		moveDown(dt);
		m_speedY -= sm_g;
	}

	
}
void ItemBoss::collisionCheck(float dt){

}
void ItemBoss::updateStatus(){
	stopAllActions();
	runAnimation(m_speedX > 0 ? "bossRight" : "bossLeft");
	
}

void ItemBoss::jumpCallback(float dt){

	if (m_status != NORMAL)
		return;
	m_speedY = 200;
	//�����������ٴε���
	int randS = ::rand() % 5 + 2;

	if (randS == 4){
		//�����ӵ�
		ItemBullet* bullet;
		getMap()->addChild(bullet=ItemBullet::create(NULL));
		//����bulletλ��

		//���ӵ���

	}

	scheduleOnce(schedule_selector(ItemBoss::jumpCallback), randS);
}