
#include "SceneGame.h"
#include "SimpleAudioEngine.h"
#include "MenuCtrl.h"
#include "SceneStart.h"
#include "Mario.h"
#include "Item.h"
SceneGame* SceneGame::create(int level){
	 SceneGame* pRet = new SceneGame();
	 if (pRet&&pRet->init(level)){
		 pRet->autorelease();
	 }
	 else{
		 delete pRet;
		 pRet = nullptr;
	 }

	 return pRet;
}
bool SceneGame::init(int level){
	Scene::init();
	_menuDir = Common::NONE;
	_level = level;
	_finalPoint = nullptr;
	
	addMap();
	addCtrlButton();
	addAnimationToCache();
	addMapObjectGroup();
	
	return true;
}

void SceneGame::addMap(){
	char mapName[32];
	Common::format(mapName, "MarioMap", _level, ".tmx");
	_map = TMXTiledMap::create(mapName);
	addChild(_map);
	_map->setPosition(_map->getPosition() + Vec2(0, winSize.height - _map->getContentSize().height));

}
void SceneGame::addCtrlButton(){



	//增加控制面板背景
	Sprite* crtlBG = Sprite::createWithTexture(TextureCache::getInstance()->addImage("controlUI.png"));
	crtlBG->setPosition(Vec2(0, 0));
	crtlBG->setAnchorPoint(Vec2(0, 0));
	addChild(crtlBG);

	//自定义菜单，实现连续点击功能
	MenuCtrl * dirMenu = MenuCtrl::create();
	addChild(dirMenu);

	_textureDirNone = TextureCache::getInstance()->addImage("backKeyImage.png");
	_textureDirLeft = TextureCache::getInstance()->addImage("backKeyLeft.png");
	_textureDirRight = TextureCache::getInstance()->addImage("backKeyRight.png");

	//方向控制键效果
	_menuShow = Sprite::createWithTexture(_textureDirNone);
	Vec2 menuBGPoint(80, 50);
	_menuShow->setPosition(menuBGPoint);
	addChild(_menuShow);



	auto left1 = Sprite::create();
	auto left2 = Sprite::create();

	left1->setContentSize(Size(_textureDirNone->getContentSize().width / 2, _textureDirNone->getContentSize().height));
	left2->setContentSize(Size(_textureDirNone->getContentSize().width / 2, _textureDirNone->getContentSize().height));
	MenuItemSprite* menuItemLeft = MenuItemSprite::create(left1, left2, std::bind(&SceneGame::moveLeftCallback, this, std::placeholders::_1));




	auto right1 = Sprite::create();
	auto right2 = Sprite::create();
	right1->setContentSize(Size(_textureDirNone->getContentSize().width / 2, _textureDirNone->getContentSize().height));
	right2->setContentSize(Size(_textureDirNone->getContentSize().width / 2, _textureDirNone->getContentSize().height));
	MenuItemSprite* menuItemRight = MenuItemSprite::create(right1, right2, std::bind(&SceneGame::moveRightCallback, this, std::placeholders::_1));



	Vec2  menuItemLeftPoint(menuBGPoint.x - winSize.width / 2 - right1->getContentSize().width / 2, menuBGPoint.y - winSize.height / 2);
	Vec2  menuItemRightPoint(menuBGPoint.x - winSize.width / 2 + right1->getContentSize().width / 2, menuBGPoint.y - winSize.height / 2);

	menuItemLeft->setPosition(menuItemLeftPoint);
	menuItemRight->setPosition(menuItemRightPoint);
	dirMenu->addChild(menuItemLeft);
	dirMenu->addChild(menuItemRight);


	/*跳跃键*/
	Menu* jumpMenu = Menu::create();
	addChild(jumpMenu);

	Texture2D* textureAB_normal = TextureCache::getInstance()->addImage("AB_normal.png");
	Texture2D* textureAB_select = TextureCache::getInstance()->addImage("AB_select.png");
	auto jumpNormal = Sprite::createWithTexture(textureAB_normal);
	auto jumpSelect = Sprite::createWithTexture(textureAB_select);
	MenuItemSprite* jumpItem = MenuItemSprite::create(jumpNormal,
													  jumpSelect,
													  std::bind(&SceneGame::jumpKeyCallback, this, std::placeholders::_1));
	jumpItem->setPosition(Vec2(190, -120));
	jumpMenu->addChild(jumpItem);

}
void SceneGame::addMapObjectGroup(){
	//加载地图对象元素
	TMXObjectGroup* objGroup = _map->getObjectGroup("objects");
	ValueVector& values = objGroup->getObjects();

	for (auto& value : values){

		ValueMap &map = value.asValueMap();
		const Value& type = map.at("type");
		if (type.asString() == "BirthPoint"){
			const Value& x = map.at("x");
			const Value& y = map.at("y");

			//创建mario
			_mario = Mario::getInstance();
			_mario->setDead(false);
			_mario->setPosition(Vec2(x.asInt(), y.asInt() - _map->getTileSize().height));
			_mario->ignoreAnchorPointForPosition(true);
			_mario->setLocalZOrder(Common::ZO_MARIO);
			
			_map->addChild(_mario);
		}
		else{
			
			Item* item = Item::create(map);
			
			if (item){
				
				switch (item->getType())
				{
				case Item::ItemType::IT_FLAGPOINT:
					_itemFlagpoint = item;
					break;
				case Item::ItemType::IT_FINALPOINT:
					_finalPoint = item;
					break;
				case Item::ItemType::IT_MUSHROOADDLIFE:
					_mushrooms.push_back(item);
				case Item::ItemType::IT_MUSHROOMREWARD:
					_mushrooms.push_back(item);
				}

				_map->addChild(item);
			}
		}
	}
}
void SceneGame::addAnimationToCache(){
	//加载蘑菇怪资源
	//加载动画资源
	{
		Animation* animation = Common::createAnimation("Mushroom0.png", 0, 2,
															  16, 0.1f);
		AnimationCache::getInstance()->addAnimation(animation, "mushroomMoving");

		SpriteFrame* dead1 = Common::getSpriteFrame("Mushroom0.png", 2, 16);
		SpriteFrame* dead2 = Common::getSpriteFrame("Mushroom0.png", 2, 16);

		SpriteFrameCache::getInstance()->addSpriteFrame(dead1, "mushroomDead1");
		SpriteFrameCache::getInstance()->addSpriteFrame(dead2, "mushroomDead2");
	}
	{
		//加载乌龟的动画资源
		Animation* animation1 = Common::createAnimation("tortoise0.png", 2, 2,
															  18, 0.4f);
		AnimationCache::getInstance()->addAnimation(animation1, "tortoiseLeftMoving");

		Animation* animation2 = Common::createAnimation("tortoise0.png", 4, 2,
														 18, 0.4f);
		AnimationCache::getInstance()->addAnimation(animation2, "tortoiseRightMoving");

		Animation* animation3 = Common::createAnimation("tortoise0.png", 8, 2,
														  18, 0.4f);
		AnimationCache::getInstance()->addAnimation(animation3, "tortoiseDead");

		SpriteFrame* dead1 = Common::getSpriteFrame("tortoise0.png", 2, 16);
		SpriteFrame* dead2 = Common::getSpriteFrame("tortoise0.png", 2, 16);

		
	}

	{
		//加载飞天乌龟的动画资源
		Animation* animation1 = Common::createAnimation("tortoise0.png", 0, 2,
														  18, 0.4f);
		AnimationCache::getInstance()->addAnimation(animation1, "tortoiseFlyLeft");

		Animation* animation2 = Common::createAnimation("tortoise0.png", 6, 2,
														  18, 0.4f);
		AnimationCache::getInstance()->addAnimation(animation2, "tortoiseFlyRight");



	}
	{
		//加载花资源
		Animation* animation = Common::createAnimation("flower0.png", 0, 2,
														 16, 0.5f);
		AnimationCache::getInstance()->addAnimation(animation, "flowerShow");

	}
	{
		SpriteFrame* frame=	Common::getSpriteFrame("rewardMushroomSet.png", 0, 16);
		SpriteFrameCache::getInstance()->addSpriteFrame(frame, "rewardMushroom");
	}
	{
		
		Animation* animation1 = Common::createAnimation("flyFishLeft.png", 0, 6,
														  16, 0.4f);
		AnimationCache::getInstance()->addAnimation(animation1, "flyFishLeft");

		Animation* animation2 = Common::createAnimation("flyFishRight.png", 0, 6,
														  16, 0.4f);
		AnimationCache::getInstance()->addAnimation(animation2, "flyFishRight");


	}
	{
		//boss
		Animation* animation1 = Common::createAnimation("boss.png", 0, 4,
														  32, 0.4f);
		AnimationCache::getInstance()->addAnimation(animation1, "bossLeft");

		Animation* animation2 = Common::createAnimation("boss.png", 4, 4,
														  32, 0.4f);
		AnimationCache::getInstance()->addAnimation(animation2, "bossRight");


	}
}

void SceneGame::onEnter(){
	Scene::onEnter();

	//播放背景音乐
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("OnLand.wma",true);
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5f);
	scheduleUpdate();
	schedule(CC_SCHEDULE_SELECTOR(SceneGame::checkMarioTouchPole));
}
void SceneGame::onExit(){

	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	Scene::onExit();
}
void SceneGame::update(float delta){
	moveMarioCheck(delta);
	marioEatCoinCheck(delta);


}
void SceneGame::marioEatCoinCheck(float delta){
	TMXLayer* coinLayer = _map->getLayer("coin");
	if (!coinLayer)
		return;
	Vec2 pts[4];
	Rect rc = _mario->getBoundingBox();
	pts[0] = Vec2(rc.getMinX(), rc.getMinY());
	pts[1] = Vec2(rc.getMinX(), rc.getMaxY());
	pts[2] = Vec2(rc.getMaxX(), rc.getMinY());
	pts[3] = Vec2(rc.getMaxX(), rc.getMaxY());
	for (int i = 0; i < 4; ++i){
		
		Vec2 ptTile = Common::pointToMap(_map, pts[i]);
		if (ptTile.x < 0 || ptTile.y < 0 || ptTile.y >= _map->getMapSize().height)
			continue;
		int gid = coinLayer->tileGIDAt(ptTile);
		if (gid){
			//播放吃金币的声音
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("EatCoin.wma");
			coinLayer->setTileGID(0, ptTile);
		}
	}
}
void SceneGame::moveRightCallback(Ref*){
	_menuShow->setTexture(_textureDirRight);
	_menuDir = Common::RIGHT;
}

void SceneGame::moveLeftCallback(Ref*){
	_menuShow->setTexture(_textureDirLeft);
	_menuDir = Common::LEFT;
}

void SceneGame::moveMarioCheck(float dt){
	
#ifdef WIN32
	short key;
	key = GetKeyState('A');
	if (key < 0)_menuDir = Common::LEFT;
	
	key = GetKeyState('D');
	if (key < 0)_menuDir = Common::RIGHT;
	key = GetKeyState('J');
	if (key < 0)jumpKeyCallback(NULL);
#endif

	Common::Direction dir = _menuDir;
	switch (dir)
	{
	
	case Common::RIGHT:
	case Common::LEFT:
		_mario->moveHorizontal(dir);
		_menuDir = Common::NONE;
		break;
	
	case Common::NONE:
		_menuShow->setTexture(_textureDirNone);
		_mario->stop();
		break;
	}

	//每个帧循环都去往上移动,竖直方向有速度则移动,没速度则静止
	_mario->moveVerticalCheck(dt);

	//每个帧循环都去左右移动,左右方向有速度则移动,没速度则静止
	_mario->moveHorizontalCheck(dt);

}

void SceneGame::jumpKeyCallback(Ref*){
	_mario->jump();
}
void SceneGame::fireKeyCallback(Ref*){}

void SceneGame::gameOver(){
	
	Director::getInstance()->replaceScene(SceneStart::create());
}

void SceneGame::checkMarioTouchPole(float dt){
	
	auto rcMario = _mario->boundingBox();
	Vec2 pt = Vec2(rcMario.getMidX(), rcMario.getMaxY());
	Vec2 ptTile = Common::pointToMap(_map, pt);
	Rect mapSizeRect(Vec2(0,0),_map->getMapSize()-Size(1,1));
	
	
	if (!mapSizeRect.containsPoint(ptTile))
		return;

	TMXLayer* layer = _map->getLayer("flagpole");
	
	if (layer->tileGIDAt(ptTile)){
		//碰到旗杆了
		_mario->autoRun();
		unschedule(CC_SCHEDULE_SELECTOR(SceneGame::checkMarioTouchPole));

		//开启是否碰到终点碰撞检测
		schedule(CC_SCHEDULE_SELECTOR(SceneGame::checkMarioTouchEndPointCallback));
	}
}

void SceneGame::checkMarioTouchEndPointCallback(float dt){

	if (_mario->getPositionX() >= _finalPoint->getPositionX()){
		unschedule(CC_SCHEDULE_SELECTOR(SceneGame::checkMarioTouchEndPointCallback));
		
		if (_level < ALL_LEVEL_NUM){
						
			_mario->endAutoRun();
			
			//因为下一个场景需要用到，所以让_mario存储的父亲节点指针置空
			_mario->removeFromParent();
			Director::getInstance()->
				replaceScene(SceneGame::create(_level + 1));
		}
		else{
			
			//全部过关
			Director::getInstance()->
				replaceScene(SceneStart::create());
		}
		
	}
}







