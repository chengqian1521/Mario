#include "Common.h"
#include "stdio.h"
CCScene* Common::scene(CCLayer * layer){
	CCScene* scene = CCScene::create();
	scene->addChild(layer);

	return scene;
}

void Common::format(char * ret, char * prefix, int v, char *suffix){

	sprintf(ret, "%s%d%s", prefix, v, suffix);

}

 CCAnimation* Common::createAnimation(const char * finename, int start, int num, int width, float  duration){

	 CCTexture2D*  texture = CCTextureCache::sharedTextureCache()->addImage(finename);
	 CCArray* arr = CCArray::create();
	 for (int i = start; i < start+num; ++i){
		 CCSpriteFrame* frame = CCSpriteFrame::createWithTexture(texture,CCRectMake(width*i,0,width,texture->getContentSize().height));
		 arr->addObject(frame);
	 }

	 return CCAnimation::createWithSpriteFrames(arr, duration);

	 
}

 CCPoint Common::mapPointToG_LBPoint(CCTMXTiledMap* map,CCPoint ptMap){


	 return CCPoint(ptMap.x*map->getTileSize().width,
					(map->getMapSize().height - ptMap.y - 1)*map->getTileSize().height);
 }
 CCPoint Common::pointToMap(CCTMXTiledMap* map,CCPoint pt){
	
	 return CCPoint((int)(pt.x / map->getTileSize().width),
					map->getMapSize().height - 1 - (int)(pt.y / map->getTileSize().height));

 }

  CCSpriteFrame* Common::getSpriteFrame(const char* filename, int pos, int width){
	  CCTexture2D*  texture = CCTextureCache::sharedTextureCache()->addImage(filename);
	 return  CCSpriteFrame::createWithTexture(texture, CCRectMake(width*pos, 0, width, texture->getContentSize().height));

 
 }