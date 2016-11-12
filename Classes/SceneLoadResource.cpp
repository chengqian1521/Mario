#include "SceneLoadResource.h"
#include "SimpleAudioEngine.h"
#include "SceneStart.h"


bool SceneLoadResource::init() {
	Scene::init();
	
	copyResourceFileNameToClassMember();
	

	//进度条背景
	auto  bar_bg = Sprite::create("sliderTrack.png");
	bar_bg->setPosition(center);
	addChild(bar_bg);

	auto timer_spr = Sprite::create("sliderProgress.png");

	//加载进度条
	_bar = ProgressTimer::create(timer_spr);
	addChild(_bar);

	_bar->setPosition(center);
	_bar->setPercentage(0);
	_bar->setType(ProgressTimer::Type::BAR);
	_bar->setMidpoint(Vec2(0,0));
	_bar->setBarChangeRate(Vec2(1,0));
	
	return true;
}

void SceneLoadResource::onEnter() {
	Scene::onEnter();

	
	_allFineNum = _picfiles.size() + _musicfiles.size();;
	auto textur_cache = Director::getInstance()->getTextureCache();

	for (int i = 0; i < _musicfiles.size(); ++i) {

		CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(_musicfiles[i]);

	}
	_loadFileCount += _musicfiles.size();

	for (int i = 0; i < _picfiles.size(); ++i) {

		textur_cache->addImageAsync(_picfiles[i], [&](Texture2D*){
			++_loadFileCount;
			_bar->setPercentage(_loadFileCount*100 / _allFineNum);
			
			if (_loadFileCount == _allFineNum) {
				//加载完成，跳进开始游戏菜单
				
				Director::getInstance()->replaceScene(SceneStart::create());
			}
		});
	}
	

}

void SceneLoadResource::copyResourceFileNameToClassMember(){
	
	char* picFileNames[] = {
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
		"WalkRight_fire.png",


	};

	char* musicfiles[] = {
		"sound/achievement.wav",
		"sound/big_spaceship_flying.wav",
		"sound/bullet.wav",
		"sound/button.wav",
		"sound/enemy1_down.wav",
		"sound/enemy2_down.wav",
		"sound/enemy3_down.wav",
		"sound/game_music.wav",
		"sound/game_over.wav",
		"sound/get_bomb.wav",
		"sound/get_double_laser.wav",
		"sound/out_porp.wav",
		"sound/use_bomb.wav",
	};
	
	for (auto picFileName : picFileNames){
		_picfiles.push_back(picFileName);
	}
	for (auto musicFileName : musicfiles){
		_musicfiles.push_back(musicFileName);
	}
}