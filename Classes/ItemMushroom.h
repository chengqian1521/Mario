#ifndef __ITEMMUSHROOM_H__
#define __ITEMMUSHROOM_H__
#include "Common.h"
#include "Item.h"
class ItemMushroom:public Item
{
public:
	
	static ItemMushroom* create(CCDictionary* dict);
	bool init(CCDictionary* dict);
	virtual void move(float dt);
	virtual void collisionCheck(float dt);
	
	bool canMoveHorizontally(float dt);
	bool canFreeFall(float dt);

protected:
	
	
	bool m_bIsDead;
};
#endif
