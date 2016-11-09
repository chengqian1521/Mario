#include "ItemBridgeStartPos.h"
#include "Mario.h"
#include "ItemBoss.h"
ItemBridgeStartPos* ItemBridgeStartPos::create(CCDictionary* dict){
	ItemBridgeStartPos * pRet = new ItemBridgeStartPos();
	if (pRet&&pRet->init(dict)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemBridgeStartPos::init(CCDictionary* dict){
	Item::init();
	m_type = Item::IT_BridgeStartPos;
	setPositionByProperty(dict);

	setContentSize(CCSizeMake(16, 16));

	return true;
}

void ItemBridgeStartPos::collisionCheck(float dt){
	
	//if (boundingBox().intersectsRect(sm_mario->boundingBox())){
	//	//ฒ๐วล
	//	ItemBoss* boss = (ItemBoss*)sm_boss;
	//	boss->m_status = ItemBoss::DROPPING;
	//	CCPoint ptTile = Common::pointToMap(getMap(), getPosition());
	//	CCTMXLayer* layer = getMap()->layerNamed("land");
	//	for (int i = 0; i < 13; ++i){
	//		layer->setTileGID(0, ptTile);
	//		++ptTile.x;
	//	}
	//}
}
