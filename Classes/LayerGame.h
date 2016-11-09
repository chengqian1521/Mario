#ifndef __LAYERGAME_H__
#define __LAYERGAME_H__
#include "Common.h"
#include "ProgressBar.h"
#include "Mario.h"
#include <set>
#include "Item.h"
class LayerGame:public CCLayer
{
public:
	static LayerGame* create(int level);
	

	

	

	void moveRightCallback(CCObject*);
	void moveLeftCallback(CCObject*);
	void moveMarioCallback(float);

	void jumpKeyCallback(CCObject*);
	void fireKeyCallback(CCObject*);
	void checkMarioTouchPole(float dt);
	void checkMarioTouchEndPointCallback(float dt);
	static void gameOver();

	//检测马里奥是否吃到金币
	void update(float delta);
public:
	CCTMXTiledMap*    m_map;
	ProgressBar*  m_bar;
	int m_level;
	int m_totalFileNum;
	int m_loadFileCount;

	Mario*  m_mario;

	CCTexture2D*  m_textureDirNone;
	CCTexture2D*  m_textureDirRight;
	CCTexture2D*  m_textureDirLeft;

	CCSprite*          m_pMenuShow;
	Common::Direction    m_menuDir;

	std::set<Item*>   m_items;

private:
	bool init(int level);
	void loadFirst();
	void loadNewLevel();
	void loadResource();
	void loadImageCallback(CCObject*);

	void startGame();
	void initItemsResource();
	void addCtrlButton();
	void addJumpFireMenuCtrl();

};

#endif