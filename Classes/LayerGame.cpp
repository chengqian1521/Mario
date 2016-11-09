#include "LayerGame.h"
#include "SimpleAudioEngine.h"
#include "MenuCtrl.h"
#include "LayerStart.h"
LayerGame* LayerGame::create(int level){
	 LayerGame* pRet = new LayerGame();
	 if (pRet&&pRet->init(level)){
		 pRet->autorelease();
	 }
	 else{
		 delete pRet;
		 pRet = NULL;
	 }

	 return pRet;
}
bool LayerGame::init(int level){
	CCLayer::init();
	//加载地图
	m_level = level;
	char mapName[32];
	Common::format(mapName, "MarioMap", level, ".tmx");

	m_map = CCTMXTiledMap::create(mapName);
	
	m_map->retain();
	
	static bool flag_loaded = false;
	if (!flag_loaded) {

		loadFirst();
		flag_loaded = true;
	}
	else {
		//已经加载过资源
		loadNewLevel();

	}
	

	
	
	m_menuDir = Common::NONE;


	
	return true;
}
void LayerGame::loadFirst() {
	//进度条背景
	m_bar = ProgressBar::create("sliderTrack.png", "sliderProgress.png");

	addChild(m_bar);
	initItemsResource();
	loadResource();
	
}
void LayerGame::loadNewLevel() {
	startGame();
}


void LayerGame::loadResource(){
	//加载图片,音乐
	static const char* picfiles[] = {
		"about_normal.png",
		"about_select.png",
		"AB_normal.png",
		"AB_select.png",
		"allow_walkLeft.png",
		"allow_walkRight.png",
		"arrow.png",
		"arrowActionL.png",
		"arrowActionR.png",
		"arrowBroken.png",
		"arrowBullet.png",
		"arrow_die.png",
		"axe.png",
		"backA.png",
		"backB.png",
		"background.png",
		"backKeyImage.png",
		"backKeyLeft.png",
		"backKeyRight.png",
		"backToMenu.png",
		"battery.png",
		"batteryBoom1.png",
		"batteryBoom2.png",
		"batteryBoom3.png",
		"batteryBullet.png",
		"bg.png",
		"blinkcoin.png",
		"boss.png",
		"bossBullet.png",
		"brokencoin.png",
		"bulletBorder.png",
		"CloseNormal.png",
		"CloseSelected.png",
		"cloud.png",
		"coinani.png",
		"controlUI.png",
		"darkCloud.png",
		"fireActionL.png",
		"fireActionR.png",
		"fireBall.png",
		"fireLeft.png",
		"fireRight.png",
		"fire_die.png",
		"flag.png",
		"flower0.png",
		"flyFishLeft.png",
		"flyFishRight.png",
		"fps_images-hd.png",
		"fps_images-ipadhd.png",
		"fps_images.png",
		"gameover.png",
		"HelloWorld.png",
		"ladder.png",
		"left.png",
		"leftright.png",
		"lighting.png",
		"Mushroom0.png",
		"music_off.png",
		"music_on.png",
		"M_n.png",
		"M_s.png",
		"newgameA.png",
		"newgameB.png",
		"nextlevel_normal.png",
		"nextlevel_select.png",
		"normal_die.png",
		"PassFailure.png",
		"PassSuccess.png",
		"princess.png",
		"quitgame_normal.png",
		"quitgame_select.png",
		"restart_n.png",
		"restart_s.png",
		"resume_n.png",
		"resume_s.png",
		"retry_normal.png",
		"retry_select.png",
		"rewardMushroomSet.png",
		"right.png",
		"select_n.png",
		"select_s.png",
		"Setting_n.png",
		"setting_s.png",
		"Set_Menu.png",
		"Set_Music.png",
		"set_n.png",
		"set_s.png",
		"shanshuodecoin.png",
		"singleblock.png",
		"sliderProgress.png",
		"sliderTrack.png",
		"smallWalkLeft.png",
		"smallWalkRight.png",
		"small_die.png",
		"sound_effect_off.png",
		"sound_effect_on.png",
		"startgame_normal.png",
		"startgame_select.png",
		"superMarioMap.png",
		"switchBg.png",
		"Tools.png",
		"tortoise0.png",
		"walkLeft.png",
		"WalkLeft_fire.png",
		"walkRight.png",
		"WalkRight_fire.png"

	};
	static const char* musicfiles[] = {
		"EatCoin.wma"
	};
	int picFileNum = sizeof(picfiles) / sizeof(*picfiles); 
	int musicFileNum=sizeof(musicfiles) / sizeof(*musicfiles);
	 m_totalFileNum = picFileNum;
	 m_loadFileCount = 0;
	for (int i = 0; i < picFileNum; ++i){
		
		CCTextureCache::sharedTextureCache()->addImageAsync(picfiles[i], this, callfuncO_selector(LayerGame::loadImageCallback));
		
		
	}
	for (int i = 0; i < musicFileNum; ++i){
		
		CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(musicfiles[i]);
		
	}

}

void LayerGame::loadImageCallback(CCObject*){
	
	++m_loadFileCount;
	m_bar->setPercentage((m_loadFileCount )*100.0 / m_totalFileNum);
	
	if (m_loadFileCount == m_totalFileNum){
		//已经加载完毕
		m_bar->removeFromParent();
		startGame();
	}
}

void LayerGame::startGame(){
	//加入地图
	
	addChild(m_map);
	m_map->release();
	m_map->setPosition(m_map->getPosition() + ccp(0, winSize.height - m_map->getContentSize().height));


	//加入控制按钮
	addCtrlButton();

	addJumpFireMenuCtrl();

	

	//创建mario
	m_mario = Mario::getInstance();

	//加载地图对象元素
	CCTMXObjectGroup* objGroup=m_map->objectGroupNamed("objects");
	CCArray* objs = objGroup->getObjects();
	CCObject* obj;
	CCARRAY_FOREACH(objs, obj){
		CCDictionary* dict = (CCDictionary*)obj;
		const CCString* x = dict->valueForKey("x");
		const CCString* y = dict->valueForKey("y");
		const CCString* type = dict->valueForKey("type");
		if (type->m_sString == "BirthPoint"){
			
			
			m_mario->setPosition(ccp(x->intValue(),y->intValue()-m_map->getTileSize().height));
			m_mario->ignoreAnchorPointForPosition(true);
			m_mario->setZOrder(Common::ZO_MARIO);
			
			m_map->addChild(m_mario);
		}
		else{
			//items
			Item* item = Item::create(dict);
			
			if (item){
				//CCLOG("%d", item->m_type);
				//m_items.insert(item);
				m_map -> addChild(item);
			}
		}
	}

	schedule(schedule_selector(LayerGame::checkMarioTouchPole));
	scheduleUpdate();
}
void LayerGame::initItemsResource(){
	//加载蘑菇怪资源
	//加载动画资源
	{
		CCAnimation* animation = Common::createAnimation("Mushroom0.png", 0, 2,
															  16, 0.1f);
		CCAnimationCache::sharedAnimationCache()->addAnimation(animation, "mushroomMoving");

		CCSpriteFrame* dead1 = Common::getSpriteFrame("Mushroom0.png", 2, 16);
		CCSpriteFrame* dead2 = Common::getSpriteFrame("Mushroom0.png", 2, 16);

		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(dead1, "mushroomDead1");
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(dead2, "mushroomDead2");
	}
	{
		//加载乌龟的动画资源
		CCAnimation* animation1 = Common::createAnimation("tortoise0.png", 2, 2,
															  18, 0.4f);
		CCAnimationCache::sharedAnimationCache()->addAnimation(animation1, "tortoiseLeftMoving");

		CCAnimation* animation2 = Common::createAnimation("tortoise0.png", 4, 2,
														 18, 0.4f);
		CCAnimationCache::sharedAnimationCache()->addAnimation(animation2, "tortoiseRightMoving");

		CCAnimation* animation3 = Common::createAnimation("tortoise0.png", 8, 2,
														  18, 0.4f);
		CCAnimationCache::sharedAnimationCache()->addAnimation(animation3, "tortoiseDead");

		CCSpriteFrame* dead1 = Common::getSpriteFrame("tortoise0.png", 2, 16);
		CCSpriteFrame* dead2 = Common::getSpriteFrame("tortoise0.png", 2, 16);

		
	}

	{
		//加载飞天乌龟的动画资源
		CCAnimation* animation1 = Common::createAnimation("tortoise0.png", 0, 2,
														  18, 0.4f);
		CCAnimationCache::sharedAnimationCache()->addAnimation(animation1, "tortoiseFlyLeft");

		CCAnimation* animation2 = Common::createAnimation("tortoise0.png", 6, 2,
														  18, 0.4f);
		CCAnimationCache::sharedAnimationCache()->addAnimation(animation2, "tortoiseFlyRight");



	}
	{
		//加载花资源
		CCAnimation* animation = Common::createAnimation("flower0.png", 0, 2,
														 16, 0.5f);
		CCAnimationCache::sharedAnimationCache()->addAnimation(animation, "flowerShow");

	}
	{
		CCSpriteFrame* frame=	Common::getSpriteFrame("rewardMushroomSet.png", 0, 16);
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(frame, "rewardMushroom");
	}
	{
		
		CCAnimation* animation1 = Common::createAnimation("flyFishLeft.png", 0, 6,
														  16, 0.4f);
		CCAnimationCache::sharedAnimationCache()->addAnimation(animation1, "flyFishLeft");

		CCAnimation* animation2 = Common::createAnimation("flyFishRight.png", 0, 6,
														  16, 0.4f);
		CCAnimationCache::sharedAnimationCache()->addAnimation(animation2, "flyFishRight");


	}
	{
		//boss
		CCAnimation* animation1 = Common::createAnimation("boss.png", 0, 4,
														  32, 0.4f);
		CCAnimationCache::sharedAnimationCache()->addAnimation(animation1, "bossLeft");

		CCAnimation* animation2 = Common::createAnimation("boss.png", 4, 4,
														  32, 0.4f);
		CCAnimationCache::sharedAnimationCache()->addAnimation(animation2, "bossRight");


	}
}
void LayerGame::addCtrlButton(){



	//增加控制按钮背景
	CCSprite* crtlBG = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("controlUI.png"));
	addChild(crtlBG);
	crtlBG->setPosition(ccp(0, 0));
	crtlBG->setAnchorPoint(CCPointZero);

	MenuCtrl * menu = MenuCtrl::create();
	addChild(menu);
	m_textureDirNone = CCTextureCache::sharedTextureCache()->addImage("backKeyImage.png");
	m_textureDirLeft = CCTextureCache::sharedTextureCache()->addImage("backKeyLeft.png");
	m_textureDirRight = CCTextureCache::sharedTextureCache()->addImage("backKeyRight.png");
	m_pMenuShow = CCSprite::createWithTexture(m_textureDirNone);
	addChild(m_pMenuShow);
	
	CCPoint menuBGPoint(80, 50);
	m_pMenuShow->setPosition(menuBGPoint);
	CCSprite* left1, *left2;
	left1 = CCSprite::create();
	left2 = CCSprite::create();

	left1->setContentSize(CCSizeMake(m_textureDirNone->getContentSize().width / 2, m_textureDirNone->getContentSize().height));
	left2->setContentSize(CCSizeMake(m_textureDirNone->getContentSize().width / 2, m_textureDirNone->getContentSize().height));
	CCMenuItemSprite* menuItemLeft = CCMenuItemSprite::create(left1, left2, this, menu_selector(LayerGame::moveLeftCallback));
	
	

	CCSprite* right1, *right2;
	right1 = CCSprite::create();
	right2 = CCSprite::create();
	right1->setContentSize(CCSizeMake(m_textureDirNone->getContentSize().width / 2, m_textureDirNone->getContentSize().height));
	right2->setContentSize(CCSizeMake(m_textureDirNone->getContentSize().width / 2, m_textureDirNone->getContentSize().height));
	CCMenuItemSprite* menuItemRight = CCMenuItemSprite::create(right1, right2, this, menu_selector(LayerGame::moveRightCallback));
	
	

	CCPoint  menuItemLeftPoint(menuBGPoint.x - winSize.width / 2 - right1->getContentSize().width / 2, menuBGPoint.y - winSize.height / 2);
	CCPoint  menuItemRightPoint(menuBGPoint.x - winSize.width / 2 + right1->getContentSize().width / 2, menuBGPoint.y - winSize.height / 2);
	
	menuItemLeft->setPosition(menuItemLeftPoint);
	menuItemRight->setPosition(menuItemRightPoint);
	menu->addChild(menuItemLeft);
	menu->addChild(menuItemRight);

	schedule(schedule_selector(LayerGame::moveMarioCallback));
}
void LayerGame::addJumpFireMenuCtrl(){
	CCMenu* menu = CCMenu::create();
	addChild(menu);

	CCTexture2D* textureAB_normal = CCTextureCache::sharedTextureCache()->addImage("AB_normal.png");
	CCTexture2D* textureAB_select = CCTextureCache::sharedTextureCache()->addImage("AB_select.png");
	CCSprite* jumpNormal = CCSprite::createWithTexture(textureAB_normal);
	CCSprite* jumpSelect = CCSprite::createWithTexture(textureAB_select);
	CCMenuItemSprite* jumpItem = CCMenuItemSprite::create(jumpNormal,
														  jumpSelect,
														  this, menu_selector(LayerGame::jumpKeyCallback));
	jumpItem->setPosition(ccp(190, -120));
	menu->addChild(jumpItem);



}
void LayerGame::moveRightCallback(CCObject*){
	//CCLOG("moveRightCallback");
	m_pMenuShow->setTexture(m_textureDirRight);
	m_menuDir = Common::RIGHT;

}

void LayerGame::moveLeftCallback(CCObject*){
	
	m_pMenuShow->setTexture(m_textureDirLeft);
	m_menuDir = Common::LEFT;
}

void LayerGame::moveMarioCallback(float dt){
	
#ifdef WIN32
	short key;
	key = GetKeyState('A');
	if (key < 0)m_menuDir = Common::LEFT;
	
	key = GetKeyState('D');
	if (key < 0)m_menuDir = Common::RIGHT;
	key = GetKeyState('J');
	if (key < 0)jumpKeyCallback(NULL);
#endif
	Common::Direction dir = m_menuDir;
	switch (dir)
	{
	
	case Common::RIGHT:
	case Common::LEFT:
		m_mario->moveHorizontal(dir);
		

		m_menuDir = Common::NONE;
		break;
	
	case Common::NONE:
		m_pMenuShow->setTexture(m_textureDirNone);
		m_mario->stop();

		break;
	
	}

	//每个帧循环都去往上移动,竖直方向有速度则移动,没速度则静止
	m_mario->moveVerticalCheck(dt);
	//每个帧循环都去左右移动,左右方向有速度则移动,没速度则静止
	m_mario->moveHorizontalCheck(dt);

}

void LayerGame::jumpKeyCallback(CCObject*){
	m_mario->jump();
}
void LayerGame::fireKeyCallback(CCObject*){}

void LayerGame::gameOver(){

	
	CCDirector::sharedDirector()->replaceScene(Common::scene(LayerStart::create()));
}

void LayerGame::checkMarioTouchPole(float dt){
	
	CCRect rcMario = m_mario->boundingBox();
	CCPoint pt = ccp(rcMario.getMidX(), rcMario.getMaxY());
	CCPoint ptTile = Common::pointToMap(m_map,pt);
	CCTMXLayer* layer = m_map->layerNamed("flagpole");

	if (ptTile.y >= m_map->getMapSize().height || ptTile.y < 0)
		return;

	if (layer->tileGIDAt(ptTile)){
		//碰到旗杆了
		m_mario->autoRun();
		unschedule(schedule_selector(LayerGame::checkMarioTouchPole));
		
		//开启是否碰到终点碰撞检测
		schedule(schedule_selector(LayerGame::checkMarioTouchEndPointCallback));
	}
}

void LayerGame::checkMarioTouchEndPointCallback(float dt){

	if (m_mario->getPositionX() >= Item::finalPoint->getPositionX()){
		if (m_level < 8){
			Item::sm_items->clear();
			m_map->removeChild(m_mario);
			m_mario->endAutoRun();
			CCDirector::sharedDirector()->
				replaceScene(Common::scene(LayerGame::create(m_level + 1)));
		}
		else{
			//全部过关
		
		}
		unschedule(schedule_selector(LayerGame::checkMarioTouchEndPointCallback));
	}
}

//检测马里奥是否吃到金币
void LayerGame::update(float delta){
	CCTMXLayer* coinLayer = m_map->layerNamed("coin");
	if (!coinLayer)
		return;
	CCPoint pts[4];
	CCRect rc = m_mario->boundingBox();
	pts[0] = ccp(rc.getMinX(), rc.getMinY());
	pts[1] = ccp(rc.getMinX(), rc.getMaxY());
	pts[2] = ccp(rc.getMaxX(), rc.getMinY());
	pts[3] = ccp(rc.getMaxX(), rc.getMaxY());
	for (int i = 0; i < 4; ++i){
		
		CCPoint ptTile = Common::pointToMap(m_map, pts[i]);
		if (ptTile.x < 0 || ptTile.y < 0 || ptTile.y >= m_map->getMapSize().height)
			continue;
		int gid = coinLayer->tileGIDAt(ptTile);
		if (gid){
			//播放吃金币的声音
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("EatCoin.wma");
			coinLayer->setTileGID(0, ptTile);
		}
	}
}