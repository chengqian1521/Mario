#ifndef __MARIO_H__
#define __MARIO_H__
#include "Common.h"
#include "Item.h"
//��mario���ɵ���
class Mario:public CCSprite
{
public:
	
	static Mario* getInstance();
	bool init();

	CCTMXTiledMap*   getMap();

	

	//���������ֱ������ٶ�
	void jump();
	void jump(int initV);
	//�������ˮƽ������ٶ�
	void moveHorizontal(Common::Direction dir);

	//��֡ѭ���д�ֱ�ٶȼ��,������ٶ�,���ƶ�
	void moveVerticalCheck(float dt);
	//��֡ѭ����ˮƽ�ٶȼ��,������ٶ�,���ƶ�
	void moveHorizontalCheck(float dt);

	

	//���������ʵ��״̬
	void updateStatus();

	//
	void stop();

	bool canMoveDown(float dt);
	bool canMoveUp(float dt);
	bool canMoveHorizontally(float dt);


	//������������
	void hitSomething(CCTMXLayer* layer, int gid,CCPoint ptTile);
	

	
	bool isFly();
	void die(bool realDead = false);
	void dieCallback();
	void eatMushroom(Item::ItemType type);
	void wakeupMushroomRewardCallback(CCNode* node);

	void beginGodMode(float dt=2.0f);
	bool isGodMode();
	void cancelGodModeCallback(float dt);

	void autoRun();
	void beginAutoMoveRightCallback(float);
	void autoMoveRightCallback(float);
	void setIsOnLadder(bool isOnLadder);
	bool isOnLadder();

	int  getSpeedY();
	int  getSpeedX();
	void setSpeedY(int v_y);
	void setSpeedX(int v_x);
	void reverseSpeedY();
	void setIsFly(bool isFly);
	void endAutoRun();
protected:
	//status
	static Mario * sm_mario;
	Common::Direction  m_faceDir;//��ֹʱ���ĳ���

	
	int     m_speed_const;		//�ȼٶ�ˮƽ����Ϊ����
	int		m_speedX;			//ˮƽ�����ٶ�,��ǰ���ٶ�
	int		m_speedY;			//���ϵ��ٶ�,��ǰ���ٶ�
	int		m_speedAcc;			//���µ��������ٶ�
	bool    m_bIsFly;
	bool    m_bIsGodMode;		//������ײ����õ����ݵ��޵�ģʽ
	int		m_life;
	bool	m_canFire;
	bool	m_bIsBig;
	bool	m_bIsDead;
	bool	m_bIsAutoRunning;

	bool    m_bIsOnLadder;

private:
	Mario(){}
};

#endif