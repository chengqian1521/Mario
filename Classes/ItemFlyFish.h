#ifndef __ItemFlyFish_H__
#define __ItemFlyFish_H__
#include "Common.h"
#include "Item.h"
class ItemFlyFish :public Item
{
public:

	static ItemFlyFish* create(ValueMap& map);
	bool init(ValueMap& map);

	virtual void moveCheck(float dt);
	virtual void collisionCheck(float dt);
	void updateStatus();
	void update(float dt)override;

	

	bool _alreadlyFly;

	int m_duration;
	int m_offsetH;
	int m_offsetV;

protected:
	
	CC_SYNTHESIZE(int, _speedX, SpeedX);
	CC_SYNTHESIZE(int, _speedY, SpeedY);
	
};
#endif
