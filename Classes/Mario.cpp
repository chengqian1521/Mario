#include "Mario.h"
#include "Item.h"
#include "LayerGame.h"

#include <hash_set>
Mario * Mario::sm_mario=NULL;
Mario* Mario::getInstance(){
	if (sm_mario){
		return sm_mario;
	}
	else{
		sm_mario = new Mario();
		if (sm_mario&&sm_mario->init()){
										
		}
		else{
			delete sm_mario;
			sm_mario = NULL;
		}

		return sm_mario;
	}
}

bool Mario::init(){

	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("smallWalkRight.png");
	CCSpriteFrame* frame = CCSpriteFrame::createWithTexture(texture, CCRectMake(0, 0, texture->getContentSize().width / 11, texture->getContentSize().height));
	CCSprite::initWithSpriteFrame(frame);

	m_faceDir = Common::RIGHT;//��ֹʱ���ĳ���

	m_speedX = 0;	//�ٶ�,�涨����Ϊ��, ��λ:ÿ���ƶ���������
	m_speedY = 0;	//���ϵ��ٶ�
	m_speedAcc = 10;//���µ��������ٶ�
	m_speed_const = 100;
	m_bIsFly = false;
	m_life = 1;
	m_canFire = false;
	m_bIsBig = false;
	m_bIsGodMode = false;
	m_bIsDead = false;
	m_bIsAutoRunning = false;
	m_bIsOnLadder = false;
	{
		

		//���ص�������
		CCAnimationCache::sharedAnimationCache()->addAnimation(Common::createAnimation("smallWalkLeft.png",
			0, 10, 14, 0.05f), "smallMoveLeftAnimation");
		CCAnimationCache::sharedAnimationCache()->addAnimation(Common::createAnimation("smallWalkRight.png",
			0, 10, 14, 0.05f), "smallMoveRightAnimation");

		//���������ʱ��Ҫ�õ�֡
		

		CCSpriteFrameCache::sharedSpriteFrameCache()
			->addSpriteFrame(Common::getSpriteFrame("smallWalkLeft.png", 10, 14),
			"smallJumpLeft");

		

		CCSpriteFrameCache::sharedSpriteFrameCache()->
			addSpriteFrame(Common::getSpriteFrame("smallWalkRight.png", 10, 14)
			, "smallJumpRight");
	}
	{
		
		//���ص�������
		CCAnimationCache::sharedAnimationCache()
			->addAnimation(Common::createAnimation("walkRight.png",
			0, 10, 18, 0.05f), "bigMoveRightAnimation");

		CCAnimationCache::sharedAnimationCache()
			->addAnimation(Common::createAnimation("walkLeft.png",
			0, 10, 18, 0.05f)
			, "bigMoveLeftAnimation");

		//���������ʱ��Ҫ�õ�֡
		

		CCSpriteFrameCache::sharedSpriteFrameCache()
			->addSpriteFrame(Common::getSpriteFrame("walkLeft.png", 10, 18),
			"bigJumpLeft");

		

		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(
			Common::getSpriteFrame("walkRight.png", 10, 18),
			"bigJumpRight");

	
	}
	{
		CCAnimationCache::sharedAnimationCache()
			->addAnimation(Common::createAnimation("small_die.png",
			0, 7,16 , 0.05f)
			, "smallDieAnimation");
	
	}
	return true;
}

//����״̬
void Mario::updateStatus(){
	this->stopAllActions();
	if (m_bIsAutoRunning){
		this->setDisplayFrameWithAnimationName(m_bIsBig ? "bigMoveRightAnimation" : "smallMoveRightAnimation", 0);


		return;
	}
	if (m_bIsDead){
		CCAnimate* animate=CCAnimate::create(CCAnimationCache::sharedAnimationCache()
						  ->animationByName("smallDieAnimation"));
		CCMoveBy* moveBy = CCMoveBy::create(winSize.height / 100,
											ccp(0, -winSize.height));
		CCCallFunc* callfunc = CCCallFunc::create(this, callfunc_selector(Mario::dieCallback));

		runAction(CCSequence::create(animate, moveBy, callfunc, NULL));
		return ;
	}
	if (m_bIsFly){
		if (m_bIsBig){
			this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()
							  ->spriteFrameByName(m_faceDir == Common::LEFT ?
							  "bigJumpLeft" : "bigJumpRight"));
		}
		else{
			this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()
								  ->spriteFrameByName(m_faceDir == Common::LEFT ?
								  "smallJumpLeft" : "smallJumpRight"));
		}
		return;
	}

	if (m_speedX < 0){

		runAction(CCRepeatForever::create(
			CCAnimate::create(CCAnimationCache::sharedAnimationCache()
			->animationByName(m_bIsBig?"bigMoveLeftAnimation":"smallMoveLeftAnimation"))));
	}
	else if (m_speedX>0){

		runAction(CCRepeatForever::create(
			CCAnimate::create(CCAnimationCache::sharedAnimationCache()
			->animationByName(m_bIsBig ? "bigMoveRightAnimation":"smallMoveRightAnimation"))));
	}
	else{

		if (m_faceDir == Common::LEFT){

			this->setDisplayFrameWithAnimationName(m_bIsBig ?"bigMoveLeftAnimation" :"smallMoveLeftAnimation", 0);
		}
		else {

			this->setDisplayFrameWithAnimationName(m_bIsBig ? "bigMoveRightAnimation" : "smallMoveRightAnimation", 0);
		}

	}

}

void Mario::stop(){
	if (m_speedX){
		m_speedX = 0;
		updateStatus();
	}
}

bool Mario::canMoveDown(float dt){
	//if (m_bIsOnLadder)
	//	CCLOG("incanMoveDown m_bISonLadder=%d", m_bIsOnLadder);
	if (m_bIsDead||m_bIsOnLadder)
		return false;
	
	
	CCRect rcMario(boundingBox().origin.x, boundingBox().origin.y,
				   boundingBox().size.width - 1, boundingBox().size.height - 1);
	CCPoint ptMario = getPosition();

	CCTMXTiledMap* map = getMap();
	CCPoint ptMarioWorld = map->convertToWorldSpace(ptMario);
	CCPoint pts[3];

	float minY = rcMario.getMinY() + m_speedY*dt;
	if (rcMario.getMinY() >= map->getContentSize().height){
		return true;
	}
	//�ж��Ƿ����
	if (minY < 0){
		return true;
	}
	//����
	pts[0] = ccp(rcMario.getMinX(), minY);
	pts[1] = ccp(rcMario.getMidX(), minY);
	pts[2] = ccp(rcMario.getMaxX(), minY);

	for (int i = 0; i < 3; ++i){
		CCPoint ptTile = Common::pointToMap(map, pts[i]);

		//ǽ,ˮ��,�ذ�
		static const char *layerName[] = {
			"block",
			"pipe",
			"land",
			
		};
		for (int j = 0; j < sizeof(layerName) / sizeof(*layerName); ++j){
			CCTMXLayer* layer = map->layerNamed(layerName[j]);
			int gid = layer->tileGIDAt(ptTile);
			if (gid){
				//�ж�����ס��
				return false;
			}
		}
	}

	return true;
}

bool Mario::canMoveUp(float dt){
	if (m_bIsDead || m_bIsAutoRunning)
		return false;
	CCRect rcMario(boundingBox().origin.x, boundingBox().origin.y,
				   boundingBox().size.width - 1, boundingBox().size.height - 1);
	CCPoint ptMario = getPosition();

	CCTMXTiledMap* map = getMap();
	CCPoint ptMarioWorld = map->convertToWorldSpace(ptMario);
	CCPoint pts[3];

	//����
	//�ж��Ƿ����
	float maxY = rcMario.getMaxY() + m_speedY*dt;
	if (maxY > map->getContentSize().height)
		return true;

	pts[0] = ccp(rcMario.getMinX(), maxY);
	pts[1] = ccp(rcMario.getMidX(), maxY);
	pts[2] = ccp(rcMario.getMaxX(), maxY);

	for (int i = 0; i < 3; ++i){
		CCPoint ptTile = Common::pointToMap(map, pts[i]);

		//ǽ,ˮ��,�ذ�
		static const char *layerName[] = {
			"block",
			"pipe",
			"land"
		};
		for (int j = 0; j < sizeof(layerName) / sizeof(*layerName); ++j){
			CCTMXLayer* layer = map->layerNamed(layerName[j]);
			int gid = layer->tileGIDAt(ptTile);
			if (gid){
				//�ж�����ס��

				hitSomething(layer, gid,ptTile);

				return false;
			}
		}
	}

	return true;
}


bool Mario::canMoveHorizontally(float dt){
	if (m_bIsDead || m_bIsAutoRunning)
		return false;
	CCRect rcMario(boundingBox().origin.x, boundingBox().origin.y,
				   boundingBox().size.width - 1, boundingBox().size.height - 1);
	CCPoint ptMario = getPosition();

	CCTMXTiledMap* map = getMap();
	CCPoint ptMarioWorld = map->convertToWorldSpace(ptMario);
	CCPoint pts[3];

	if (ptMario.y >= map->getContentSize().height){
		return true;
	}
	if (getPositionY() < 0)
		return true;
	//�ж��Ƿ����
	if (ptMarioWorld.x + m_speedX*dt <= 0){
		return false;
	}

	float midY = rcMario.getMidY() > map->getContentSize().height-1 ? map->getContentSize().height-1 : rcMario.getMidY();
	float maxY = rcMario.getMaxY() > map->getContentSize().height -1? map->getContentSize().height-1: rcMario.getMaxY();
	//CCLOG("midY=%g,maxY=%g", midY, maxY);
	if (m_speedX < 0){
		//������
		float minX = rcMario.getMinX() + m_speedX*dt;
		pts[0] = ccp(minX, midY);
		pts[1] = ccp(minX, maxY);
		pts[2] = ccp(minX, rcMario.getMinY());

	}
	else{
		//������
		float maxX = rcMario.getMaxX() + m_speedX*dt;
		pts[0] = ccp(maxX, midY);
		pts[1] = ccp(maxX, maxY);
		pts[2] = ccp(maxX, rcMario.getMinY());
	}

	for (int i = 0; i < 3; ++i){
		CCPoint ptTile = Common::pointToMap(map, pts[i]);

		//ǽ,ˮ��,�ذ�
		static const char *layerName[] = {
			"block",
			"pipe",
			"land"
		};
		for (int j = 0; j < sizeof(layerName) / sizeof(*layerName); ++j){
			CCTMXLayer* layer = map->layerNamed(layerName[j]);
			int gid = layer->tileGIDAt(ptTile);
			if (gid){
				//�ж�����ס��
				return false;
			}
		}
	}

	return true;
}

CCTMXTiledMap* Mario::getMap(){
	return (CCTMXTiledMap*)getParent();
}

//�������ˮƽ������ٶ�
void Mario::moveHorizontal(Common::Direction dir){
	if (m_bIsDead||m_bIsAutoRunning)
		return;

	if (!m_speedX){

		if (dir == Common::LEFT){
			m_speedX = -m_speed_const;
		}
		else{
			m_speedX = m_speed_const;
		}

		m_faceDir = dir;

		updateStatus();
	}

}

void Mario::jump(){
	if (m_bIsDead || m_bIsFly||m_bIsAutoRunning)
		return;
	
	m_speedY = 300;
	m_bIsFly = true;

	updateStatus();

}
void Mario::jump(int initV){
	if (m_bIsDead)
		return;
	m_speedY = initV;
	m_bIsFly = true;

	updateStatus();
}

void Mario::moveVerticalCheck(float dt){
	
		//�ж��Ƿ������������
		if (!m_bIsFly){
			m_speedY -= m_speedAcc;
			if (canMoveDown(dt)){
				//û�ж�����ס,��������
				this->setPositionY(getPositionY() + dt*m_speedY);
				m_bIsFly = true;

			}
			else{
				m_bIsFly = false;
				m_speedY = 0;
			}

		}

		if (m_speedY > 0){
			if (canMoveUp(dt)){
				setPositionY(getPositionY() + dt*m_speedY);
				m_speedY -= m_speedAcc;
			}
			else{
				//�ٶȷ���
				m_speedY = -m_speedY;
			}
		}
		else if (m_speedY < 0){
			if (canMoveDown(dt)){
				setPositionY(getPositionY() + dt*m_speedY);
				m_speedY -= m_speedAcc;
			}
			else{
				m_bIsFly = false;
				m_speedY = 0;
				updateStatus();
			}
		}
		else{
			m_speedY -= m_speedAcc;

		}

	
}

//ˮƽ�ƶ����
void Mario::moveHorizontalCheck(float dt){
	if (!m_speedX)
		return;
	if (!canMoveHorizontally(dt))
		return;
	this->setPositionX(getPositionX() + m_speedX*dt);
	if (m_speedX > 0){
		CCNode* node = getParent();
		CCPoint ptWorld = node->convertToWorldSpace(getPosition());

		if (!m_bIsAutoRunning&&ptWorld.x > winSize.width / 2){
			node->setPositionX(node->getPositionX() - dt*abs(m_speedX));
		}
	}

	//m_speed = 0;
}




void Mario::die(bool realDead){
	CCLOG("maiio die");
	return;
	if (realDead){
		//����������
		m_bIsDead = true;
		updateStatus();
	}
	else{
		//�������ȱ�С
		
		if (m_bIsBig){
			m_bIsBig = false;
			m_canFire = false;
			this->beginGodMode();
			this->updateStatus();
		}
		else{
			die(true);
		}
	
	}
}
void Mario::dieCallback(){
	LayerGame::gameOver();
}
bool Mario::isFly(){
	return m_bIsFly;
}

//������������
void Mario::hitSomething(CCTMXLayer * layer, int gid, CCPoint ptTile){
	if (std::string(layer->getLayerName()) != "block")
		return;
	CCSprite* sprite = layer->tileAt(ptTile);
	CCCallFuncN *callfunc = CCCallFuncN::create(this, callfuncN_selector(Mario::wakeupMushroomRewardCallback));
	CCJumpBy*  by = CCJumpBy::create(0.3f, ccp(0, 0), 12, 1);
	sprite->runAction(CCSequence::create(by,callfunc,NULL));

	
	
}

void Mario::wakeupMushroomRewardCallback(CCNode* node){
	//using MapIt = std::set<Item*>::iterator;

	typedef std::set<Item*>::iterator MapIt;
	
	for (MapIt ib = Item::sm_items->begin(); ib != Item::sm_items->end(); ){
		Item* item = *ib;
		CCRect rcItem = item->boundingBox();

		//�ǿ�ש
		CCRect rcNode = node->boundingBox();
		rcItem.size.width -= 1;
		rcNode.size.width -= 1;
		rcItem.size.height -= 1;
		rcNode.size.height -= 1;
		if (rcItem.intersectsRect(rcNode)){
			item->setVisible(true);
			item->wakeup();
			//MapIt ib2=Item::sm_items->erase(ib);
			
			break;
		}
		else{
			++ib;
		}
	}
}

void Mario::eatMushroom(Item::ItemType type){
	if (type==Item::IT_MUSHROOMREWARD)
		m_bIsBig = true;
}

void Mario::beginGodMode(float dt){
	if (m_bIsGodMode)
		return;

	m_bIsGodMode = true;
	scheduleOnce(schedule_selector(Mario::cancelGodModeCallback), dt);
}
void Mario::cancelGodModeCallback(float dt){
	m_bIsGodMode = false;
}

bool Mario::isGodMode(){
	return m_bIsGodMode;
}

void Mario::autoRun(){
	m_bIsAutoRunning = true;
	updateStatus();
	m_speedY = 0;

	//������Զ��»�
	Item::sm_flag->autoDropFlag();

	//�Զ���ǰ��
	scheduleOnce(schedule_selector(Mario::beginAutoMoveRightCallback), 1.5f);

}

void Mario::beginAutoMoveRightCallback(float dt){
	//�Զ���ǰ��
	schedule(schedule_selector(Mario::autoMoveRightCallback));

	
}
void Mario::autoMoveRightCallback(float dt){
	setPositionX(getPositionX() + dt * 100);
}

void Mario::setIsOnLadder(bool isOnLadder){
	//if (isOnLadder)
	//CCLOG("setIsOnLadder() invoke isOnLadder=%d", isOnLadder);
	m_bIsOnLadder = isOnLadder;
}
bool Mario::isOnLadder(){
	return m_bIsOnLadder;
}

int Mario::getSpeedY(){
	return m_speedY;
}
int Mario::getSpeedX(){
	return m_speedX;
}

void Mario::setSpeedY(int v_y){
	m_speedY = v_y;
}
void Mario::setSpeedX(int v_x){
	m_speedX = v_x;
}

void Mario::reverseSpeedY(){
	m_speedY = -m_speedY;
}

void  Mario::setIsFly(bool isFly){
	m_bIsFly = isFly;
}

void Mario::endAutoRun() {

	m_bIsAutoRunning = false;
}