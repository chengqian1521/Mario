#ifndef __ItemMushroomAddLife_H__
#define __ItemMushroomAddLife_H__
#include "Common.h"
#include "Item.h"
class ItemMushroomAddLife :public Item
{
public:
	enum State
	{
		SLEEP,
		GROW,
		MOVE
	};
	static ItemMushroomAddLife* create(CCDictionary* dict);
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
