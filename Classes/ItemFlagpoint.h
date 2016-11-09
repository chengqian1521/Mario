#ifndef __ItemFlagpoint_H__
#define __ItemFlagpoint_H__
#include "Common.h"
#include "Item.h"
class ItemFlagpoint :public Item
{
public:

	static ItemFlagpoint* create(CCDictionary* dict);
	bool init(CCDictionary* dict);
	virtual void autoDropFlag();

};
#endif
