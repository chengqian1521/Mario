#ifndef __ItemLadderLR_H__
#define __ItemLadderLR_H__
#include "Common.h"
#include "Item.h"
class ItemLadderLR :public Item
{
public:

	static ItemLadderLR* create(CCDictionary* dict);
	bool init(CCDictionary* dict);

	virtual void move(float dt);
	virtual void collisionCheck(float dt);

protected:
	int m_lorR;
	float m_ladderDis;
	float m_dist;
	

	//标记该梯子上是否有马里奥
	bool m_bIsMarioOnThis;
};
#endif
