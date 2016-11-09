#include "ItemFireString.h"
#include "Mario.h"
ItemFireString* ItemFireString::create(CCDictionary* dict){
	ItemFireString * pRet = new ItemFireString();
	if (pRet&&pRet->init(dict)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemFireString::init(CCDictionary* dict){
	Item::init();
	m_type = Item::IT_fire_string;
	setPositionByProperty(dict);
	
	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->
		addImage("bossBullet.png");
	setTexture(texture);
	setTextureRect(CCRectMake(0, 0, texture->getContentSize().width,
		texture->getContentSize().height));

	setAnchorPoint(ccp(0,0.5f));
	int begAngle=dict->valueForKey("begAngle")->intValue();
	setRotation(begAngle);

	int time = dict->valueForKey("time")->intValue();

	runAction(CCRepeatForever::create(
		CCRotateBy::create(time, 360)));

	return true;
}

void ItemFireString::collisionCheck(float dt){
	if (!isAppearInWindow()){
		return;
	}

	//获取线段
	struct Line{
		CCPoint p1;
		CCPoint p2;
	};
	Line fireString;

	CCRect rcItem = boundingBox();

	int angle = getRotation();
	angle %= 360;
	if (angle > 270){
		fireString.p1 = ccp(rcItem.getMaxX(), rcItem.getMinY());
		fireString.p2 = ccp(rcItem.getMinX(),rcItem.getMaxY());
	}
	else if (angle > 180){
		fireString.p1 = ccp(rcItem.getMaxX(), rcItem.getMaxY());
		fireString.p2 = ccp(rcItem.getMinX(), rcItem.getMinY());

	}
	else if (angle > 90){
		fireString.p1 = ccp(rcItem.getMinX(), rcItem.getMaxY());
		fireString.p2 = ccp(rcItem.getMaxX(), rcItem.getMinY());

	}
	else
	{
		fireString.p1 = ccp(rcItem.getMinX(), rcItem.getMinY());
		fireString.p2 = ccp(rcItem.getMaxX(), rcItem.getMaxY());

	}
	//CCLOG("len%g", fireString.p1.getDistance(fireString.p2));
	CCRect rcMario = sm_mario->boundingBox();
	//mario的线段
	Line lineMario[2];
	lineMario[0].p1 = ccp(rcMario.getMinX(), rcMario.getMinY());
	lineMario[0].p2 = ccp(rcMario.getMaxX(), rcMario.getMaxY());
	lineMario[1].p1 = ccp(rcMario.getMinX(), rcMario.getMaxY());
	lineMario[1].p2 = ccp(rcMario.getMaxX(), rcMario.getMinY());

	for (int i = 0; i < 2; ++i){
		if (ccpSegmentIntersect(lineMario[i].p1, lineMario[i].p2, fireString.p1, fireString.p2)){
			sm_mario->die(false);
		}
	}															
}
