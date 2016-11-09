#ifndef __ItemLadderUD_H__
#define __ItemLadderUD_H__
#include "Common.h"
#include "Item.h"
class ItemLadderUD :public Item
{
public:

	static ItemLadderUD* create(CCDictionary* dict);
	bool init(CCDictionary* dict);


};
#endif
