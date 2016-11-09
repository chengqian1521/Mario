#ifndef __ItemMushroomReward_H__
#define __ItemMushroomReward_H__
#include "Common.h"
#include "Item.h"
class ItemMushroomReward :public Item
{
public:
	enum State
	{
		SLEEP,
		GROW,
		MOVE
	};
	static ItemMushroomReward* create(CCDictionary* dict);
	bool init(CCDictionary* dict);
	virtual void wakeup();

	bool canMoveHorizontally(float dt);
	bool canFreeFall(float dt);
	virtual void move(float dt);
	void updateStatus();
	void afterGrowCallback();

	void collisionCheck(float dt);
public:
	State m_state;
	
};
#endif
