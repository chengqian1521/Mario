#ifndef __ItemBoss_H__
#define __ItemBoss_H__
#include "Common.h"
#include "Item.h"
class ItemBoss :public Item
{
public:
	enum State
	{
	NORMAL,
	DROPPING
	} m_status;
	static ItemBoss* create(CCDictionary* dict);
	bool init(CCDictionary* dict);
	virtual void move(float dt);
	virtual void collisionCheck(float dt);
	void updateStatus();
	void jumpCallback(float dt);

	float m_left;
	float m_right;
};
#endif
