#ifndef __ItemLadderLR_H__
#define __ItemLadderLR_H__
#include "Common.h"
#include "Item.h"
class ItemLadderLR :public Item
{
public:

	static ItemLadderLR* create(ValueMap& map);
	bool init(ValueMap& map);

	virtual void moveCheck(float dt);
	virtual void collisionCheck(float dt);

protected:
	int		m_lorR;
	float	m_ladderDis;
	float	m_dist;
	
	CC_SYNTHESIZE(int, _speedX, SpeedX);
	CC_SYNTHESIZE(int, _speedY, SpeedY);

	//��Ǹ��������Ƿ��������
	bool	m_bIsMarioOnThis;
};
#endif
