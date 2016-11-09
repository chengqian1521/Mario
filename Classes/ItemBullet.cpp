#include "ItemBullet.h"

ItemBullet* ItemBullet::create(CCDictionary* dict){
	ItemBullet * pRet = new ItemBullet();
	if (pRet&&pRet->init(dict)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemBullet::init(CCDictionary* dict){
	Item::init();
	m_type = Item::IT_bullet;
	//setPositionByProperty(dict);
	


	return true;
}
