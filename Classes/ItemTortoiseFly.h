#ifndef __ItemTortoiseFly_H__
#define __ItemTortoiseFly_H__
#include "Common.h"
#include "Item.h"
class ItemTortoiseFly :public Item
{
public:
	//״̬
	enum 
	{
		NORMAL,
		DROPPING,
		ONLAND,
		SLEEP,
		CRAZY
	} m_status;
	static ItemTortoiseFly* create(CCDictionary* dict);
	bool init(CCDictionary* dict);

	void updateStatus();

	virtual void move(float dt);
	virtual void collisionCheck(float dt);
	void reLiveCallback(float);
	
	int m_flyDis;
	
};
#endif
