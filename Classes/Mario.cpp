#include "Mario.h"
#include "Item.h"
#include "SceneGame.h"
#include "ItemMushroom.h"
#include <hash_set>
Mario* Mario::sm_mario = NULL;
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
			sm_mario = nullptr;
		}

		return sm_mario;
	}
}

bool Mario::init(){

	Texture2D* texture = TextureCache::getInstance()->addImage("smallWalkRight.png");
	SpriteFrame* frame = SpriteFrame::createWithTexture(texture, Rect(0, 0, texture->getContentSize().width / 11, texture->getContentSize().height));
	Sprite::initWithSpriteFrame(frame);

	m_faceDir = Common::RIGHT;//静止时脸的朝向
	_speedX = 0;	//速度,规定向右为正, 单位:每秒移动多少像素
	_speedY = 0;	//向上的速度


	_speed_const = 100;
	_isFly = false;
	_life = 3;
	_canFire = false;
	_isBig = false;
	_isGodMode = false;
	_isDead = false;
	_isAutoRunning = false;
	_isOnLadder = false;
	{


		//加载到缓存中
		CCAnimationCache::sharedAnimationCache()->addAnimation(Common::createAnimation("smallWalkLeft.png",
			0, 10, 14, 0.05f), "smallMoveLeftAnimation");
		CCAnimationCache::sharedAnimationCache()->addAnimation(Common::createAnimation("smallWalkRight.png",
			0, 10, 14, 0.05f), "smallMoveRightAnimation");

		//马里奥跳的时候要用的帧


		CCSpriteFrameCache::sharedSpriteFrameCache()
			->addSpriteFrame(Common::getSpriteFrame("smallWalkLeft.png", 10, 14),
			"smallJumpLeft");



		CCSpriteFrameCache::sharedSpriteFrameCache()->
			addSpriteFrame(Common::getSpriteFrame("smallWalkRight.png", 10, 14)
			, "smallJumpRight");
	}
	{

		//加载到缓存中
		CCAnimationCache::sharedAnimationCache()
			->addAnimation(Common::createAnimation("walkRight.png",
			0, 10, 18, 0.05f), "bigMoveRightAnimation");

		CCAnimationCache::sharedAnimationCache()
			->addAnimation(Common::createAnimation("walkLeft.png",
			0, 10, 18, 0.05f)
			, "bigMoveLeftAnimation");

		//马里奥跳的时候要用的帧


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
			0, 7, 16, 0.05f)
			, "smallDieAnimation");

	}
	return true;
}

//更新状态
void Mario::updateStatus(){
	if (_isDead){
		return;
	}

	this->stopAllActions();
	if (_isAutoRunning){
		this->setDisplayFrameWithAnimationName(_isBig ? "bigMoveRightAnimation" : "smallMoveRightAnimation", 0);
		return;
	}
	
	if (_isFly){
		if (_isBig){
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

	if (_speedX < 0){

		runAction(CCRepeatForever::create(
			CCAnimate::create(CCAnimationCache::sharedAnimationCache()
			->animationByName(_isBig ? "bigMoveLeftAnimation" : "smallMoveLeftAnimation"))));
	}
	else if (_speedX>0){

		runAction(CCRepeatForever::create(
			CCAnimate::create(CCAnimationCache::sharedAnimationCache()
			->animationByName(_isBig ? "bigMoveRightAnimation" : "smallMoveRightAnimation"))));
	}
	else{

		if (m_faceDir == Common::LEFT){

			this->setDisplayFrameWithAnimationName(_isBig ? "bigMoveLeftAnimation" : "smallMoveLeftAnimation", 0);
		}
		else {

			this->setDisplayFrameWithAnimationName(_isBig ? "bigMoveRightAnimation" : "smallMoveRightAnimation", 0);
		}

	}

}

void Mario::stop(){
	if (_speedX){
		_speedX = 0;
		updateStatus();
	}
}

bool Mario::canMoveDown(float dt){
	//if (m_bIsOnLadder)
	//	CCLOG("incanMoveDown m_bISonLadder=%d", m_bIsOnLadder);
	if (_isDead || _isOnLadder)
		return false;


	CCRect rcMario(boundingBox().origin.x, boundingBox().origin.y,
				   boundingBox().size.width - 1, boundingBox().size.height - 1);
	Vec2 ptMario = getPosition();

	CCTMXTiledMap* map = getMap();
	Vec2 ptMarioWorld = map->convertToWorldSpace(ptMario);
	Vec2 pts[3];

	float minY = rcMario.getMinY() + _speedY*dt;
	if (rcMario.getMinY() >= map->getContentSize().height){
		return true;
	}
	//判断是否出界
	if (minY < 0){
		return true;
	}
	//向下
	pts[0] = ccp(rcMario.getMinX(), minY);
	pts[1] = ccp(rcMario.getMidX(), minY);
	pts[2] = ccp(rcMario.getMaxX(), minY);

	for (int i = 0; i < 3; ++i){
		Vec2 ptTile = Common::pointToMap(map, pts[i]);

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

bool Mario::canMoveUp(float dt){
	if (_isDead || _isAutoRunning)
		return false;
	CCRect rcMario(boundingBox().origin.x, boundingBox().origin.y,
				   boundingBox().size.width - 1, boundingBox().size.height - 1);
	Vec2 ptMario = getPosition();

	CCTMXTiledMap* map = getMap();
	Vec2 ptMarioWorld = map->convertToWorldSpace(ptMario);
	Vec2 pts[3];

	//向上
	//判断是否出界
	float maxY = rcMario.getMaxY() + _speedY*dt;
	if (maxY > map->getContentSize().height)
		return true;

	pts[0] = ccp(rcMario.getMinX(), maxY);
	pts[1] = ccp(rcMario.getMidX(), maxY);
	pts[2] = ccp(rcMario.getMaxX(), maxY);

	for (int i = 0; i < 3; ++i){
		Vec2 ptTile = Common::pointToMap(map, pts[i]);

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

				hitSomething(layer, gid, ptTile);

				return false;
			}
		}
	}

	return true;
}


bool Mario::canMoveHorizontally(float dt){
	if (_isDead || _isAutoRunning)
		return false;
	CCRect rcMario(boundingBox().origin.x, boundingBox().origin.y,
				   boundingBox().size.width - 1, boundingBox().size.height - 1);
	Vec2 ptMario = getPosition();

	CCTMXTiledMap* map = getMap();
	Vec2 ptMarioWorld = map->convertToWorldSpace(ptMario);
	Vec2 pts[3];

	if (ptMario.y >= map->getContentSize().height){
		return true;
	}
	if (getPositionY() < 0)
		return true;
	//判断是否出界
	if (ptMarioWorld.x + _speedX*dt <= 0){
		return false;
	}

	float midY = rcMario.getMidY() > map->getContentSize().height - 1 ? map->getContentSize().height - 1 : rcMario.getMidY();
	float maxY = rcMario.getMaxY() > map->getContentSize().height - 1 ? map->getContentSize().height - 1 : rcMario.getMaxY();
	//CCLOG("midY=%g,maxY=%g", midY, maxY);
	if (_speedX < 0){
		//向左走
		float minX = rcMario.getMinX() + _speedX*dt;
		pts[0] = ccp(minX, midY);
		pts[1] = ccp(minX, maxY);
		pts[2] = ccp(minX, rcMario.getMinY());

	}
	else{
		//向右走
		float maxX = rcMario.getMaxX() + _speedX*dt;
		pts[0] = ccp(maxX, midY);
		pts[1] = ccp(maxX, maxY);
		pts[2] = ccp(maxX, rcMario.getMinY());
	}

	for (int i = 0; i < 3; ++i){
		Vec2 ptTile = Common::pointToMap(map, pts[i]);

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

TMXTiledMap* Mario::getMap(){
	return (TMXTiledMap*)getParent();
}

//给马里奥水平方向的速度
void Mario::moveHorizontal(Common::Direction dir){
	if (_isDead || _isAutoRunning)
		return;

	if (!_speedX){

		if (dir == Common::LEFT){
			_speedX = -_speed_const;
		}
		else{
			_speedX = _speed_const;
		}

		m_faceDir = dir;

		updateStatus();
	}

}

void Mario::jump(){
	if (_isDead || _isFly || _isAutoRunning)
		return;

	_speedY = 300;
	_isFly = true;

	updateStatus();

}
void Mario::jump(int initV){
	if (_isDead)
		return;
	_speedY = initV;
	_isFly = true;

	updateStatus();
}

void Mario::moveVerticalCheck(float dt){

	//判断是否可以自由落体
	if (!_isFly){
		_speedY -= ARG_GRAVITY;
		if (canMoveDown(dt)){
			//没有东西挡住,自由下落
			this->setPositionY(getPositionY() + dt*_speedY);
			_isFly = true;

		}
		else{
			_isFly = false;
			_speedY = 0;
		}

	}

	if (_speedY > 0){
		if (canMoveUp(dt)){
			setPositionY(getPositionY() + dt*_speedY);
			_speedY -= ARG_GRAVITY;
		}
		else{
			//速度反弹
			_speedY = -_speedY;
		}
	}
	else if (_speedY < 0){
		if (canMoveDown(dt)){
			setPositionY(getPositionY() + dt*_speedY);
			_speedY -= ARG_GRAVITY;
		}
		else{
			_isFly = false;
			_speedY = 0;
			updateStatus();
		}
	}
	else{
		_speedY -= ARG_GRAVITY;

	}


}

//水平移动检测
void Mario::moveHorizontalCheck(float dt){
	if (!_speedX)
		return;
	if (!canMoveHorizontally(dt))
		return;
	this->setPositionX(getPositionX() + _speedX*dt);
	if (_speedX > 0){
		CCNode* node = getParent();
		Vec2 ptWorld = node->convertToWorldSpace(getPosition());

		if (!_isAutoRunning&&ptWorld.x > winSize.width / 2){
			node->setPositionX(node->getPositionX() - dt*abs(_speedX));
		}
	}

	//m_speed = 0;
}

void Mario::setDead(bool isDead){
	_isDead = isDead;
}


void Mario::die(bool realDead){
	if (isDead())
		return;

	if (realDead){
		//真正死亡了
		_isDead = true;
		--_life;

		this->stopAllActions();
		
		_speedX = 0;

		//进入死亡动画
		Animate* animate = Animate::create(AnimationCache::getInstance()
										   ->getAnimation("smallDieAnimation"));
		MoveBy* moveBy = MoveBy::create(1,
										Vec2(0, -winSize.height));
		CallFunc* callfunc = CallFunc::create([&](){
			//死亡动画结束了

			SceneGame* game = dynamic_cast<SceneGame*> (getMap()->getParent());
			if (game){
				if (_life > 0){
					int level = game->_level;
					this->removeFromParent();
					Director::getInstance()->replaceScene(SceneGame::create(level));
				}
				else{
					game->gameOver();
				}
			}
			else{
				CCLOG("%s:%d dynamic_cast err", __FILE__, __LINE__);
			}



		});

		runAction(Sequence::create(animate, moveBy, callfunc, nullptr));


	}
	else{
		//碰到怪先变小

		if (_isBig){
			_isBig = false;
			_canFire = false;
			this->beginGodMode();
			this->updateStatus();
		}
		else{
			die(true);
		}

	}
}

bool Mario::isFly(){
	return _isFly;
}
bool Mario::isDead(){
	return _isDead;
}

//顶到东西处理
void Mario::hitSomething(TMXLayer * layer, int gid, Vec2 ptTile){
	if (std::string(layer->getLayerName()) != "block")
		return;
	Sprite* sprite = layer->tileAt(ptTile);
	CallFuncN *callfunc = CallFuncN::create(this, CC_CALLFUNCN_SELECTOR(Mario::checkHitMushroomCallback));
	JumpBy*  by = JumpBy::create(0.3f, Vec2(0, 0), 12, 1);
	sprite->runAction(Sequence::create(by, callfunc, nullptr));
}

void Mario::checkHitMushroomCallback(CCNode* node){

	SceneGame* game = dynamic_cast<SceneGame*>(getMap()->getParent());
	if (!game){
		CCLOG_DYNAMIC_ERR;
		return;
	}
	//那块砖
	Rect rcNode = node->getBoundingBox();
	rcNode.size = rcNode.size - Size(1, 1);

	for (auto ib = game->_mushrooms.begin(); ib != game->_mushrooms.end(); ++ib){
		Item* item = *ib;
		Rect rcItem = item->getBoundingBox();
		rcItem.size = rcItem.size - Size(1, 1);

		//撞到蘑菇了
		if (rcItem.intersectsRect(rcNode)){
			item->setVisible(true);
			ItemMushroom* mushroom = dynamic_cast<ItemMushroom*>(item);
			if (!mushroom)
				CCLOG_DYNAMIC_ERR;
			mushroom->wakeup();
			game->_mushrooms.erase(ib);
			break;
		}

	}
}

void Mario::eatMushroom(Item::ItemType type){
	if (type == Item::IT_MUSHROOMREWARD)
		_isBig = true;
}

void Mario::beginGodMode(float dt){
	if (_isGodMode)
		return;

	_isGodMode = true;
	scheduleOnce(schedule_selector(Mario::cancelGodModeCallback), dt);
}
void Mario::cancelGodModeCallback(float dt){
	_isGodMode = false;
}

bool Mario::isGodMode(){
	return _isGodMode;
}

void Mario::autoRun(){
	_isAutoRunning = true;
	updateStatus();
	_speedY = 0;



	//自动向前走
	scheduleOnce(schedule_selector(Mario::beginAutoMoveRightCallback), 1.5f);

}

void Mario::beginAutoMoveRightCallback(float dt){
	//自动向前走
	schedule(schedule_selector(Mario::autoMoveRightCallback));

}
void Mario::autoMoveRightCallback(float dt){
	setPositionX(getPositionX() + dt * 100);
}

void Mario::setIsOnLadder(bool isOnLadder){
	//if (isOnLadder)
	//CCLOG("setIsOnLadder() invoke isOnLadder=%d", isOnLadder);
	_isOnLadder = isOnLadder;
}
bool Mario::isOnLadder(){
	return _isOnLadder;
}

int Mario::getSpeedY(){
	return _speedY;
}
int Mario::getSpeedX(){
	return _speedX;
}

void Mario::setSpeedY(int v_y){
	_speedY = v_y;
}
void Mario::setSpeedX(int v_x){
	_speedX = v_x;
}

void Mario::reverseSpeedY(){
	_speedY = -_speedY;
}

void  Mario::setIsFly(bool isFly){
	_isFly = isFly;
}

void Mario::endAutoRun() {

	_isAutoRunning = false;
}