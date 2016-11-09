#include "ItemFlyFish.h"
#include "Mario.h"
ItemFlyFish* ItemFlyFish::create(CCDictionary* dict){
	ItemFlyFish * pRet = new ItemFlyFish();
	if (pRet&&pRet->init(dict)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemFlyFish::init(CCDictionary* dict){
	Item::init();
	m_type = Item::IT_flyfish;
	setPositionByProperty(dict);

	m_duration = dict->valueForKey("duration")->intValue();
	m_offsetH = dict->valueForKey("offsetH")->intValue();
	m_offsetV = dict->valueForKey("offsetV")->intValue();
	//setVisible(false);
	//m_speedY = m_offsetV *4;
	m_speedX = m_offsetH;
	_alreadlyFly = false;
	
	return true;
}

void ItemFlyFish::move(float dt){
	if (Mario::getInstance()->getPositionX() < getPositionX())
		return;
	if (!this->isAppearInWindow()||_alreadlyFly){
		return;
	}
	updateStatus();
	//贝塞尔曲线运动
	ccBezierConfig config;
	config.controlPoint_1.x = CCRANDOM_0_1()*m_offsetH;
	config.controlPoint_1.y = CCRANDOM_0_1()*m_offsetV;
	config.controlPoint_2.x = CCRANDOM_0_1()*m_offsetH;
	config.controlPoint_2.y = CCRANDOM_0_1()*m_offsetV;
	config.endPosition = ccp(m_offsetH, m_offsetV);
	CCBezierBy* bezier = CCBezierBy::create(m_duration, config);

	ccBezierConfig config1;
	config1.controlPoint_1.x = CCRANDOM_0_1()*m_offsetH;
	config1.controlPoint_1.y = CCRANDOM_0_1()*m_offsetV;
	config1.controlPoint_2.x = CCRANDOM_0_1()*m_offsetH;
	config1.controlPoint_2.y = CCRANDOM_0_1()*m_offsetV;
	config1.endPosition = ccp(m_offsetH, -m_offsetV);
	CCBezierBy* bezier1 = CCBezierBy::create(m_duration, config1);

	CCCallFunc* callfunc = CCCallFunc::create(this, callfunc_selector(ItemFlyFish::removeFromParent));

	runAction(CCSequence::create(bezier, bezier1, callfunc,NULL));
	_alreadlyFly = true;
	////水平移动
	//setPositionX(getPositionX() + dt*m_speedX);

	////竖直方向移动
	//setPositionY(getPositionY() + dt*m_speedY);
}
void ItemFlyFish::collisionCheck(float dt){
	if (!_alreadlyFly)
		return;
	Mario* mario = Mario::getInstance();
	if (mario->boundingBox().intersectsRect(boundingBox())){
		mario->die(false);
	}
}

void ItemFlyFish::updateStatus(){
	
	
	CCAnimation* ani = CCAnimationCache::sharedAnimationCache()
		->animationByName(m_speedX>0 ? "flyFishRight" : "flyFishLeft");
	this->runAction(CCRepeatForever::create(CCAnimate::create(ani)));
}