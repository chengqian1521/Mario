#include "Item.h"
#include "ItemMushroom.h"
#include "ItemTortoise.h"
#include "ItemFlower.h"
#include "ItemMushroomReward.h"
#include "ItemMushroomAddLife.h"
#include "ItemFlagpoint.h"
#include "ItemFinalpoint.h"
#include "ItemLadderLR.h"
#include "ItemFlyFish.h"
#include "ItemTortoiseRound.h"
#include "ItemTortoiseFly.h"
#include "ItemLadderUD.h"
#include "ItemFireString.h"
#include "ItemBoss.h"
#include "ItemBridgeStartPos.h"
#include "Mario.h"
std::set<Item*>*  Item::sm_items = new std::set<Item*>();
std::string Item::str("heh");
Mario* Item::sm_mario = NULL;
Item*  Item::sm_flag=NULL;
Item*  Item::finalPoint = NULL;
Item*  Item::sm_boss=NULL;
int Item::sm_g = 10;
Item::Item(){
	
}
Item::~Item(){
}
Item* Item::create(CCDictionary* dict){
	
	std::set<int>  hah;
	std::string str("hehe");

	const CCString* type = dict->valueForKey("type");
	if (type->m_sString == "mushroom"){
		return ItemMushroom::create(dict);
	}
	else  if (type->m_sString == "tortoise"){
		return ItemTortoise::create(dict);
	}
	else if (type->m_sString == "flower"){
		return ItemFlower::create(dict);
	}
	else if (type->m_sString == "MushroomReward"){
		ItemMushroomReward* mushroomReward = ItemMushroomReward::create(dict);
		sm_items->insert(mushroomReward);
			
		return mushroomReward;
	}
	else if (type->m_sString == "MushroomAddLife"){
		ItemMushroomAddLife* itemMushroomAddLife = ItemMushroomAddLife::create(dict);
		sm_items->insert(itemMushroomAddLife);

		return itemMushroomAddLife;
	}
	else if (type->m_sString == "flagpoint"){
		return	Item::sm_flag=ItemFlagpoint::create(dict);
	}
	else if (type->m_sString == "finalpoint"){
		finalPoint = ItemFinalpoint::create(dict);
	}
	else if (type->m_sString == "ladderLR"){
		return ItemLadderLR::create(dict);
	}
	else if (type->m_sString == "flyfish"){
		return ItemFlyFish::create(dict);
	}
	else if (type->m_sString == "tortoise_round"){
		return ItemTortoiseRound::create(dict);
	}
	else if (type->m_sString == "tortoise_fly"){
		return ItemTortoiseFly::create(dict);
	}
	else if (type->m_sString == "ladderUD"){
		return ItemLadderUD::create(dict);
	}
	else if (type->m_sString == "fire_string"){
		return ItemFireString::create(dict);
	}
	else if (type->m_sString == "boss"){
		return ItemBoss::create(dict);
	}
	else if (type->m_sString == "bridgestartpos"){
		return ItemBridgeStartPos::create(dict);
	}


	return NULL;
}

void Item::setPositionByProperty(CCDictionary* dict){

	const CCString* x = dict->valueForKey("x");
	const CCString* y = dict->valueForKey("y");
	setPosition(ccp(x->intValue(), y->intValue()-16));
	ignoreAnchorPointForPosition(true);
}
bool Item::init(){
	CCSprite::init();
	
	sm_mario = Mario::getInstance();
	m_bIsGodMode = false;
	setZOrder(Common::ZO_MUSHROOM);
	scheduleUpdate();
	return true;
}
void Item::move(float dt){

}

void Item::update(float dt){
	move(dt);
	collisionCheck(dt);
}

bool Item::isAppearInWindow(){
	CCPoint ptInMap = getPosition();
	CCTMXTiledMap* map = getMap();
	CCPoint ptInWorld = map->convertToWorldSpace(ptInMap);
	return ( ptInWorld.x < winSize.width);
	
	
}

CCTMXTiledMap* Item::getMap(){
	return (CCTMXTiledMap*)getParent();
}
bool Item::isOutOfWindow(){
	CCPoint	ptWorld=getMap()->convertToWorldSpace(getPosition());
	return (ptWorld.x < -winSize.width) || (getPositionY() <= -boundingBox().size.height);
		
	
}

 void Item::wakeup(){

}

void Item::collisionCheck(float dt){
	
}
 void Item::autoDropFlag(){

}

 bool Item::canMoveHorizontal(float dt){
 
	
	 CCRect rc(boundingBox().origin.x, boundingBox().origin.y,
					boundingBox().size.width - 1, boundingBox().size.height - 1);
	
	 
	 CCTMXTiledMap* map = getMap();
	 CCPoint ptInWorld = map->convertToWorldSpace(getPosition());
	 CCPoint pts[3];

	 //判断水平是否出界
	 if (ptInWorld.x + m_speedX*dt <= 0){
		 return false;
	 }

	 //竖直方向如果出界 ,则可以移动
	 if (getPositionY() < 0||getPositionY() >= map->getContentSize().height){
		 return true;
	 }
	
	 

	 float midY = rc.getMidY() > map->getContentSize().height - 1 ? map->getContentSize().height - 1 : rc.getMidY();
	 float maxY = rc.getMaxY() > map->getContentSize().height - 1 ? map->getContentSize().height - 1 : rc.getMaxY();
	 
	 if (m_speedX < 0){
		 //向左走
		 float minX = rc.getMinX() + m_speedX*dt;
		 pts[0] = ccp(minX, midY);
		 pts[1] = ccp(minX, maxY);
		 pts[2] = ccp(minX, rc.getMinY());

	 }
	 else{
		 //向右走
		 float maxX = rc.getMaxX() + m_speedX*dt;
		 pts[0] = ccp(maxX, midY);
		 pts[1] = ccp(maxX, maxY);
		 pts[2] = ccp(maxX, rc.getMinY());
	 }

	 for (int i = 0; i < 3; ++i){
		 CCPoint ptTile = Common::pointToMap(map, pts[i]);

		 //墙,水管,地板
		 static const char *layerName[] = {
			 "block",
			 "pipe",
			 "land"
		 };
		 for (int j = 0; j < sizeof(layerName) / sizeof(*layerName); ++j){
			 CCTMXLayer* layer = map->layerNamed(layerName[j]);
			 int gid = layer->tileGIDAt(ptTile);
			 if (gid){
				 //有东西挡住了
				 return false;
			 }
		 }
	 }


	 return true;
 }
 bool Item::canMoveDown(float dt){
	 if (m_speedY > 0)
		 return true;
	 CCRect rcItem(boundingBox().origin.x, boundingBox().origin.y,
					boundingBox().size.width - 1, boundingBox().size.height - 1);
	 CCPoint ptItem = getPosition();

	 CCTMXTiledMap* map = getMap();
	 CCPoint ptItemInWorld = map->convertToWorldSpace(ptItem);
	 CCPoint pts[3];

	 if (!m_speedY)
		 m_speedY -= sm_g;

	 float minY = rcItem.getMinY() + m_speedY*dt;
	 if (rcItem.getMinY() >= map->getContentSize().height){
		 return true;
	 }
	 //判断是否出界
	 if (minY < 0){
		 return true;
	 }
	 //向下
	 pts[0] = ccp(rcItem.getMinX(), minY);
	 pts[1] = ccp(rcItem.getMidX(), minY);
	 pts[2] = ccp(rcItem.getMaxX(), minY);

	 for (int i = 0; i < 3; ++i){
		 CCPoint ptTile = Common::pointToMap(map, pts[i]);

		 //墙,水管,地板
		 static const char *layerName[] = {
			 "block",
			 "pipe",
			 "land",

		 };
		 for (int j = 0; j < sizeof(layerName) / sizeof(*layerName); ++j){
			 CCTMXLayer* layer = map->layerNamed(layerName[j]);
			 int gid = layer->tileGIDAt(ptTile);
			 if (gid){
				 //有东西挡住了
				 return false;
			 }
		 }
	 }


	 return true;
 }
	 
 void Item::moveHorizontal(float dt){
	 setPositionX(getPositionX() + dt*m_speedX);
 }
 void Item::moveDown(float dt){
	 
	 setPositionY(getPositionY() + dt*m_speedY);
 }

 void Item::beginGodMode(float dt){
	 m_bIsGodMode = true;
	 scheduleOnce(schedule_selector(Item::cancelGodModeCallback),dt);
 }
 void Item::cancelGodModeCallback(float dt){
	 m_bIsGodMode = false;
 }