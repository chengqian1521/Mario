#include "LayerStart.h"
#include "LayerSelectLevel.h"

bool LayerStart::init(){
	CCLayer::init();

	CCSprite* bg = CCSprite::create("background.png");
	addChild(bg);
	bg->setPosition(ccp(winSize.width / 2, winSize.height / 2));

	CCMenu* menu = CCMenu::create();
	addChild(menu);

	CCMenuItemImage* start = CCMenuItemImage::create("startgame_normal.png",
													 "startgame_select.png",
													 this,
													 menu_selector(LayerStart::startCallback)

													 );
	CCMenuItemImage* setup = CCMenuItemImage::create("Setting_n.png",
													 "Setting_s.png",
													 this,
													 menu_selector(LayerStart::setupCallback)

													 );
	CCMenuItemImage* quit = CCMenuItemImage::create("quitgame_normal.png",
													 "quitgame_select.png",
													 this,
													 menu_selector(LayerStart::quitCallback)

													 );
	CCMenuItemImage* about = CCMenuItemImage::create("about_normal.png",
													 "about_select.png",
													 this,
													 menu_selector(LayerStart::aboutCallback)

													 );

	menu->addChild(start);
	menu->addChild(setup);
	menu->addChild(quit);
	menu->addChild(about);

	//设置四个按钮的位置
	//start->setPosition(start->getPosition() - ccp(0, 50));
	setup->setPosition(start->getPosition() - ccp(0, 50));
	quit->setPosition(start->getPosition() - ccp(0, 100));
	about->setPosition(start->getPosition() - ccp(-195, 150));


	return true;
}

void LayerStart::startCallback(CCObject*){
	CCDirector::sharedDirector()->replaceScene(Common::scene(LayerSelectLevel::create()));
}
void LayerStart::setupCallback(CCObject*){}
void LayerStart::quitCallback(CCObject*){}
void LayerStart::aboutCallback(CCObject*){}