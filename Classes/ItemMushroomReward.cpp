#include "ItemMushroomReward.h"
#include "Mario.h"

ItemMushroomReward* ItemMushroomReward::create(CCDictionary* dict){
	ItemMushroomReward * pRet = new ItemMushroomReward();
	if (pRet&&pRet->init(dict)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemMushroomReward::init(CCDictionary* dict){
	Item::init();
	m_type = Item::IT_MUSHROOMREWARD;
	setPositionByProperty(dict);
	
	setVisible(false);
	this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("rewardMushroom"));
	setZOrder(0);
	m_state = SLEEP;
	m_speedY = 0;
	m_speedX = 40;
	return true;
}
void ItemMushroomReward::wakeup(){
	m_state = GROW;
	updateStatus();
}

void ItemMushroomReward::updateStatus(){
	if (m_state == GROW){
		CCMoveBy* by = CCMoveBy::create(1.0f, ccp(0, 16));
		CCCallFunc* callfunc = CCCallFunc::create(this, callfunc_selector(ItemMushroomReward::afterGrowCallback));
		runAction(CCSequence::create(by,callfunc,NULL));
	}
	else if (m_state == MOVE){
		
	}
}

void ItemMushroomReward::afterGrowCallback(){
	m_state = MOVE;
	setZOrder(100);
	updateStatus();
}

void ItemMushroomReward::move(float dt){
	if (m_state != MOVE)return;

	if (canMoveHorizontally(dt)){
		setPositionX(getPositionX() + m_speedX*dt);
	}
	else{
		m_speedX = -m_speedX;
	}

	if (canFreeFall(dt)){
		setPositionY(getPositionY() + m_speedY*dt);
	}

	if (isOutOfWindow()){
		removeFromParent();
	}
	
}

bool ItemMushroomReward::canMoveHorizontally(float dt){
	

	CCRect rc(boundingBox().origin.x, boundingBox().origin.y,
			  boundingBox().size.width - 1, boundingBox().size.height - 1);

	CCTMXTiledMap* map = getMap();
	CCPoint ptMarioWorld = map->convertToWorldSpace(getPosition());
	CCPoint pts[3];

	if (rc.getMinY() <= 0)
		return true;

	if (ptMarioWorld.x + m_speedX*dt <= 0){
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


bool ItemMushroomReward::canFreeFall(float dt){


	CCRect rc = boundingBox();


	CCTMXTiledMap* map = getMap();

	CCPoint ptWorld = map->convertToWorldSpace(getPosition());
	if (ptWorld.x <= 0)
		return true;
	CCPoint pts[3];
	m_speedY -= sm_g;
	float minY = rc.getMinY() + m_speedY*dt;

	if (minY <= 0)
		return true;
	pts[0] = ccp(rc.getMinX(), minY);
	pts[1] = ccp(rc.getMidX(), minY);
	pts[2] = ccp(rc.getMaxX(), minY);

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
				m_speedY = 0;
				return false;
			}
		}
	}
	return true;
}

void ItemMushroomReward::collisionCheck(float dt){
	if (m_state != MOVE)
		return;
	if (sm_mario->boundingBox().intersectsRect(this->boundingBox())){
		sm_mario->eatMushroom(this->m_type);
		
		removeFromParent();
		
	}
}