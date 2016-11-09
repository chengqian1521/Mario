#include "ItemMushroom.h"
#include "Mario.h"

ItemMushroom* ItemMushroom::create(CCDictionary* dict){
	ItemMushroom * pRet = new ItemMushroom();
	if (pRet&&pRet->init(dict)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemMushroom::init(CCDictionary* dict){
	Item::init();
	m_type = Item::IT_MUSHROOM;
	setPositionByProperty(dict);
	setDisplayFrameWithAnimationName("mushroomMoving", 0);

	m_speedY = 0;
	m_speedX = -40;
	m_bIsDead = false;
	this->runAction(CCRepeatForever::create(CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("mushroomMoving"))));
	return true;
}

void ItemMushroom::move(float dt){
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
bool ItemMushroom::canMoveHorizontally(float dt){
	if (!isAppearInWindow()){
		return false;
	}

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


bool ItemMushroom::canFreeFall(float dt){
	if (!isAppearInWindow()){
		return false;
	}

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

void ItemMushroom::collisionCheck(float dt){
	if (m_bIsDead)
		return;
	
	CCRect rcMario = sm_mario->boundingBox();
	CCRect rcItem = this->boundingBox();

	if (rcMario.intersectsRect(rcItem)){
		if (sm_mario->getSpeedY() <= 0 && rcMario.getMinY()>rcItem.getMaxY()-rcItem.size.height/2){
			//马里奥胜利
			this->stopAllActions();
			this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("mushroomDead1"));
			m_speedX = 0;
			this->m_bIsDead = true;
			
			this->runAction(CCMoveBy::create(1, ccp(0, -winSize.height)));
		}
		else{
			//马里奥死亡
			if (sm_mario->isGodMode())
				return;
			
			sm_mario->die();
		}
	}

}