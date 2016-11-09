#ifndef __ItemFinalpoint_H__
#define __ItemFinalpoint_H__
#include "Common.h"
#include "Item.h"
class ItemFinalpoint :public Item
{
public:

	static ItemFinalpoint* create(CCDictionary* dict);
	bool init(CCDictionary* dict);


};
#endif
