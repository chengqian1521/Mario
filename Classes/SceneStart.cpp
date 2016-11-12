#include "SceneStart.h"
#include "SceneSelectLevel.h"

bool SceneStart::init(){
	Scene::init();

	Sprite* bg = Sprite::create("background.png");
	addChild(bg);
	bg->setPosition(center);

	Menu* menu = Menu::create();
	addChild(menu);

	MenuItemImage* start = MenuItemImage::create("startgame_normal.png",
													 "startgame_select.png",
													 std::bind(
													&SceneStart::startCallback,this,std::placeholders::_1)

													 );
	MenuItemImage* setup = MenuItemImage::create("Setting_n.png",
													 "Setting_s.png",
													 std::bind(
													 &SceneStart::setupCallback, this, std::placeholders::_1)

													 );
	MenuItemImage* quit = MenuItemImage::create("quitgame_normal.png",
													 "quitgame_select.png",
													 std::bind(
													 &SceneStart::quitCallback, this, std::placeholders::_1)

													 );
	MenuItemImage* about = MenuItemImage::create("about_normal.png",
													 "about_select.png",
													 std::bind(
													 &SceneStart::aboutCallback, this, std::placeholders::_1)

													 );

	menu->addChild(start);
	menu->addChild(setup);
	menu->addChild(quit);
	menu->addChild(about);

	//设置四个按钮的位置
	
	setup->setPosition(start->getPosition() - Vec2(0, 50));
	quit->setPosition(start->getPosition() - Vec2(0, 100));
	about->setPosition(start->getPosition() - Vec2(-195, 150));


	return true;
}

void SceneStart::startCallback(Ref*){
	Director::getInstance()->replaceScene(SceneSelectLevel::create());
}
void SceneStart::setupCallback(Ref*){

}
void SceneStart::quitCallback(Ref*){
	Director::getInstance()->popScene();
}
void SceneStart::aboutCallback(Ref*){
	
}