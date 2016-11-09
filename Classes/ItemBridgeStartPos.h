#ifndef __ItemBridgeStartPos_H__
#define __ItemBridgeStartPos_H__
#include "Common.h"
#include "Item.h"
class ItemBridgeStartPos :public Item
{
public:

	static ItemBridgeStartPos* create(CCDictionary* dict);
	bool init(CCDictionary* dict);
	virtual void collisionCheck(float dt);

};
#endif
