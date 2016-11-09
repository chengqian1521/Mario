#ifndef __ItemTortoiseRound_H__
#define __ItemTortoiseRound_H__
#include "Common.h"
#include "Item.h"
class ItemTortoiseRound :public Item
{
public:
	enum State
	{
		NORMAL,
		SLEEP,
		CRAZY
	};
	static ItemTortoiseRound* create(CCDictionary* dict);
	bool init(CCDictionary* dict);

	virtual void move(float dt);
	virtual void collisionCheck(float dt);
	void updateStatus();

	void autoReLiveCallback(float dt);
	void cancelGodModeCallback(float dt);
	int m_initXV;
	int m_roundDis;
	
	int m_state;
	bool m_bIsGodMode;
};
#endif
