#ifndef __MARIO_H__
#define __MARIO_H__
#include "Common.h"
#include "ItemCanMove.h"

//将mario做成单例
class Mario :public Sprite
{
public:
	
	static Mario* getInstance();
	bool init();

	TMXTiledMap*   getMap();

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

	
	void stop();

	bool canMoveDown(float dt);
	bool canMoveUp(float dt);
	bool canMoveHorizontally(float dt);


	//顶到东西处理
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
	Common::Direction  m_faceDir;//静止时脸的朝向
	int     _speed_const;	//先假定水平方向为匀速
	int		_speedX;		//水平方向速度,当前的速度
	int		_speedY;		//向上的速度,当前的速度
	
	bool    _isFly;
	bool    _isGodMode;	//被敌人撞击后得到短暂的无敌模式
	int		_life;
	bool	_canFire;
	bool	_isBig;
	bool	_isDead;
	bool	_isAutoRunning;
	bool    _isOnLadder;


};

#endif