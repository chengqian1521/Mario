#ifndef __ItemDarkCloud_H__
#define __ItemDarkCloud_H__
#include "Common.h"
#include "Item.h"
class ItemDarkCloud :public Item
{
public:

	static ItemDarkCloud* create(CCDictionary* dict);
	bool init(CCDictionary* dict);


};
#endif
