#ifndef __ITEM_H__
#define __ITEM_H__
#include "Common.h"
#include <set>
#include <string>
class ItemFlagpoint;
class ItemMushroomReward;
class Mario;
class Item:public CCSprite
{
public:
	enum ItemType
	{
		IT_MUSHROOM,
		IT_TORTOISE,
		IT_FLOWER,
		IT_MUSHROOMREWARD,
		IT_MUSHROOADDLIFE,
		IT_FLAGPOINT,
		IT_FINALPOINT,
		IT_ladderLR,
		IT_flyfish,
		IT_tortoise_round,
		IT_tortoise_fly,
		IT_fire_string,
		IT_boss,
		IT_bullet,
		IT_BridgeStartPos
	};
	Item();
	virtual ~Item();
	static Item* create(CCDictionary*);
	bool init();

	virtual void move(float dt);
	virtual void collisionCheck(float dt);
	virtual void wakeup();
	void update(float dt);

	void setPositionByProperty(CCDictionary*);

	bool isAppearInWindow();
	bool isOutOfWindow();

	CCTMXTiledMap* getMap();

	virtual void autoDropFlag();

	bool canMoveHorizontal(float dt);
	bool canMoveDown(float dt);

	void moveHorizontal(float);
	void moveDown(float dt);

	void beginGodMode(float dt);
	void cancelGodModeCallback(float dt);
	
	void runAnimation(const char* name){
		CCAnimation* animation = CCAnimationCache::sharedAnimationCache()->
			animationByName(name);
		runAction(CCRepeatForever::create(CCAnimate::create(animation)));
	}
public:
	int m_speedX;
	int m_speedY;
	bool m_bIsGodMode;
	static int		sm_g;   //重力加速度
	ItemType m_type;
	static Mario*   sm_mario;
	static Item*    sm_flag;
	static std::string str;
	static std::set<Item*>*  sm_items;
	static Item*  finalPoint;
	static Item*  sm_boss;
};

#endif