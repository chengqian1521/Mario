#ifndef __MARIO_H__
#define __MARIO_H__
#include "Common.h"
#include "Item.h"
//将mario做成单例
class Mario:public CCSprite
{
public:
	
	static Mario* getInstance();
	bool init();

	CCTMXTiledMap*   getMap();

	

	//给马里奥竖直方向的速度
	void jump();
	void jump(int initV);
	//给马里奥水平方向的速度
	void moveHorizontal(Common::Direction dir);

	//在帧循环中垂直速度检测,如果有速度,则移动
	void moveVerticalCheck(float dt);
	//在帧循环中水平速度检测,如果有速度,则移动
	void moveHorizontalCheck(float dt);

	

	//更新马里奥实际状态
	void updateStatus();

	//
	void stop();

	bool canMoveDown(float dt);
	bool canMoveUp(float dt);
	bool canMoveHorizontally(float dt);


	//顶到东西处理
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
	Common::Direction  m_faceDir;//静止时脸的朝向

	
	int     m_speed_const;		//先假定水平方向为匀速
	int		m_speedX;			//水平方向速度,当前的速度
	int		m_speedY;			//向上的速度,当前的速度
	int		m_speedAcc;			//向下的重力加速度
	bool    m_bIsFly;
	bool    m_bIsGodMode;		//被敌人撞击后得到短暂的无敌模式
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