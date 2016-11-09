#ifndef __ItemFlyFish_H__
#define __ItemFlyFish_H__
#include "Common.h"
#include "Item.h"
class ItemFlyFish :public Item
{
public:

	static ItemFlyFish* create(CCDictionary* dict);
	bool init(CCDictionary* dict);

	virtual void move(float dt);
	virtual void collisionCheck(float dt);
	void updateStatus();

	

	bool _alreadlyFly;

	int m_duration;
	int m_offsetH;
	int m_offsetV;

	
	
};
#endif
