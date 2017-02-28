#include "ItemBoss.h"
#include "ItemBullet.h"

ItemBoss* ItemBoss::create(ValueMap& map)
{
	ItemBoss * pRet = new ItemBoss();
	if (pRet&&pRet->init(map)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemBoss::init(ValueMap& map)
{
	ItemCanMove::init();
	_type = Item::IT_boss;
	setPositionByProperty(map);
	
	m_left=	getPositionX()-4*16;
	m_right=getPositionX()+4*16;
	m_status = NORMAL;
	_speedX = 40;
	_speedY = 0;

	

	updateStatus();
	scheduleOnce(schedule_selector(ItemBoss::jumpCallback), 3.0f);
	return true;
}

void ItemBoss::moveCheck(float dt){
	if (m_status == NORMAL){
		setPositionX(getPositionX()+dt*_speedX);
		if (getPositionX() > m_right||getPositionX()<m_left){
			_speedX = -_speedX;
			updateStatus();
		}
		if (canMoveDown(dt)){
			moveDown(dt);
			_speedY -= ARG_GRAVITY;
		}
		else{
			_speedY = 0;
		}

		//���Ŷϵ���,�л�ΪDROPPING״̬
	}
	else if (m_status == DROPPING){
		moveDown(dt);
		_speedY -= ARG_GRAVITY;
	}

	
}
void ItemBoss::update(float dt){
	collisionCheck(dt);
	moveCheck(dt);
}
void ItemBoss::collisionCheck(float dt){

}
void ItemBoss::updateStatus(){
	stopAllActions();
	runAnimation(_speedX > 0 ? "bossRight" : "bossLeft");
	
}

void ItemBoss::jumpCallback(float dt){

	if (m_status != NORMAL)
		return;
	_speedY = 200;
	//�����������ٴε���
	int randS = ::rand() % 5 + 2;

	if (randS == 4){
		//�����ӵ�
		ItemBullet* bullet;
		getMap()->addChild(bullet=ItemBullet::create());
		//����bulletλ��

		//���ӵ���

	}

	scheduleOnce(schedule_selector(ItemBoss::jumpCallback), randS);
}