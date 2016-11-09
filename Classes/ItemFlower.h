#ifndef __ItemFlower_H__
#define __ItemFlower_H__
#include "Common.h"
#include "Item.h"
class ItemFlower :public Item
{
public:

	static ItemFlower* create(CCDictionary* dict);
	bool init(CCDictionary* dict);

	void updateStatus();

	void cancelShowCallback(float);

	virtual void move(float dt);
	
	virtual void collisionCheck(float dt);
	bool m_bShow;//±íÏÖ×´Ì¬
	int  m_yTop;
	int  m_yButtom;
	
};
#endif
