#include "SceneLoadResource.h"
#include "SimpleAudioEngine.h"
#include "SceneStart.h"


bool SceneLoadResource::init() {
	Scene::init();
	
	copyResourceFileNameToClassMember();
	
	//���ر���
	Sprite* bg = Sprite::create(IMAGE_bg);
	bg->setPosition(center);
	bg->setScale(winSize.height/bg->getBoundingBox().size.height);
	addChild(bg);

	//����������
	auto  bar_bg = Sprite::create(IMAGE_sliderTrack);
	bar_bg->setPosition(center);
	addChild(bar_bg);

	auto timer_spr = Sprite::create(IMAGE_sliderProgress);

	//���ؽ�����
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
				//������ɣ�������ʼ��Ϸ�˵�
				
				Director::getInstance()->replaceScene(SceneStart::create());
			}
		});
	}
	

}

void SceneLoadResource::copyResourceFileNameToClassMember(){
	
	char* picFileNames[] = {
		IMAGE_about_normal,
		IMAGE_about_select,
		IMAGE_AB_normal,
		IMAGE_AB_select,
		IMAGE_allow_walkLeft,
		IMAGE_allow_walkRight,
		IMAGE_arrow,
		IMAGE_arrowActionL,
		IMAGE_arrowActionR,
		IMAGE_arrowBroken,
		IMAGE_arrowBullet,
		IMAGE_arrow_die,
		IMAGE_axe,
		IMAGE_backA,
		IMAGE_backB,
		IMAGE_background,
		IMAGE_backKeyImage,
		IMAGE_backKeyLeft,
		IMAGE_backKeyRight,
		IMAGE_backToMenu,
		IMAGE_battery,
		IMAGE_batteryBoom1,
		IMAGE_batteryBoom2,
		IMAGE_batteryBoom3,
		IMAGE_batteryBullet,
		IMAGE_bg,
		IMAGE_blinkcoin,
		IMAGE_boss,
		IMAGE_bossBullet,
		IMAGE_brokencoin,
		IMAGE_bulletBorder,
		IMAGE_CloseNormal,
		IMAGE_CloseSelected,
		IMAGE_cloud,
		IMAGE_coinani,
		IMAGE_controlUI,
		IMAGE_darkCloud,
		IMAGE_fireActionL,
		IMAGE_fireActionR,
		IMAGE_fireBall,
		IMAGE_fireLeft,
		IMAGE_fireRight,
		IMAGE_fire_die,
		IMAGE_flag,
		IMAGE_flower0,
		IMAGE_flyFishLeft,
		IMAGE_flyFishRight,
		IMAGE_fps_images_hd,
		IMAGE_fps_images_ipadhd,
		IMAGE_fps_images,
		IMAGE_gameover,
		IMAGE_HelloWorld,
		IMAGE_ladder,
		IMAGE_left,
		IMAGE_leftright,
		IMAGE_lighting,
		IMAGE_Mushroom0,
		IMAGE_music_off,
		IMAGE_music_on,
		IMAGE_M_n,
		IMAGE_M_s,
		IMAGE_newgameA,
		IMAGE_newgameB,
		IMAGE_nextlevel_normal,
		IMAGE_nextlevel_select,
		IMAGE_normal_die,
		IMAGE_PassFailure,
		IMAGE_PassSuccess,
		IMAGE_princess,
		IMAGE_quitgame_normal,
		IMAGE_quitgame_select,
		IMAGE_restart_n,
		IMAGE_restart_s,
		IMAGE_resume_n,
		IMAGE_resume_s,
		IMAGE_retry_normal,
		IMAGE_retry_select,
		IMAGE_rewardMushroomSet,
		IMAGE_right,
		IMAGE_select_n,
		IMAGE_select_s,
		IMAGE_Setting_n,
		IMAGE_setting_s,
		IMAGE_Set_Menu,
		IMAGE_Set_Music,
		IMAGE_set_n,
		IMAGE_set_s,
		IMAGE_shanshuodecoin,
		IMAGE_singleblock,
		IMAGE_sliderProgress,
		IMAGE_sliderTrack,
		IMAGE_smallWalkLeft,
		IMAGE_smallWalkRight,
		IMAGE_small_die,
		IMAGE_sound_effect_off,
		IMAGE_sound_effect_on,
		IMAGE_startgame_normal,
		IMAGE_startgame_select,
		IMAGE_superMarioMap,
		IMAGE_switchBg,
		IMAGE_Tools,
		IMAGE_tortoise0,
		IMAGE_walkLeft,
		IMAGE_WalkLeft_fire,
		IMAGE_walkRight,
		IMAGE_WalkRight_fire
	};

	char* musicfiles[] = {
		SOUND_achievement,
		SOUND_big_spaceship_flying,
		SOUND_bullet,
		SOUND_button,
		SOUND_enemy1_down,
		SOUND_enemy2_down,
		SOUND_enemy3_down,
		SOUND_game_music,
		SOUND_game_over,
		SOUND_get_bomb,
		SOUND_get_double_laser,
		SOUND_out_porp,
		SOUND_use_bomb
	};
	
	for (auto picFileName : picFileNames){
		_picfiles.push_back(picFileName);
	}
	for (auto musicFileName : musicfiles){
		_musicfiles.push_back(musicFileName);
	}
}