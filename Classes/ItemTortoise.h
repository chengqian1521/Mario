#ifndef __ItemTortoise_H__
#define __ItemTortoise_H__
#include "Common.h"
#include "Item.h"
class ItemTortoise :public Item
{
public:
	enum State
	{
		NORMAL,
		CRAZY,
		SLEEP,
	};
	static ItemTortoise* create(CCDictionary* dict);
	bool init(CCDictionary* dict);

	
	virtual void collisionCheck(float dt);
	
	
	virtual void move(float dt);
	void cancelGodModeCallback(float dt);
	void reLiveCallback(float dt);

	void updateStatus();
protected:
	
	int m_initXV;
	
	
						//被马里奥踩了一段时间
	bool m_bIsDead;
	State m_state;

};
#endif
