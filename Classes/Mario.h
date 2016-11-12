#ifndef __MARIO_H__
#define __MARIO_H__
#include "Common.h"
#include "ItemCanMove.h"

//��mario���ɵ���
class Mario :public Sprite
{
public:
	
	static Mario* getInstance();
	bool init();

	TMXTiledMap*   getMap();

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

	
	void stop();

	bool canMoveDown(float dt);
	bool canMoveUp(float dt);
	bool canMoveHorizontally(float dt);


	//������������
	void hitSomething(CCTMXLayer* layer, int gid,Vec2 ptTile);
	

	
	bool isFly();
	bool isDead();
	void die(bool realDead = false);
	
	void eatMushroom(Item::ItemType type);
	void checkHitMushroomCallback(CCNode* node);

	void beginGodMode(float dt=2.0f);
	
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

	void setDead(bool isDead);
	bool isGodMode();
private:
	Mario(){}
protected:
	//status
	static Mario * sm_mario;
	Common::Direction  m_faceDir;//��ֹʱ���ĳ���
	int     _speed_const;	//�ȼٶ�ˮƽ����Ϊ����
	int		_speedX;		//ˮƽ�����ٶ�,��ǰ���ٶ�
	int		_speedY;		//���ϵ��ٶ�,��ǰ���ٶ�
	
	bool    _isFly;
	bool    _isGodMode;	//������ײ����õ����ݵ��޵�ģʽ
	int		_life;
	bool	_canFire;
	bool	_isBig;
	bool	_isDead;
	bool	_isAutoRunning;
	bool    _isOnLadder;


};

#endif