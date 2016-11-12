#include "Common.h"
#include "stdio.h"


void Common::format(char * ret, char * prefix, int v, char *suffix){
	sprintf(ret, "%s%d%s", prefix, v, suffix);
}

Animation* Common::createAnimation(const char * finename, int start, int num, int width, float  duration){

	Texture2D*  texture = TextureCache::getInstance()->addImage(finename);
	Vector<SpriteFrame*> vec;
	for (int i = start; i < start + num; ++i){
		SpriteFrame* frame = SpriteFrame::createWithTexture(texture, Rect(width*i, 0, width, texture->getContentSize().height));
		vec.pushBack(frame);
	}

	return Animation::createWithSpriteFrames(vec, duration);


}

Vec2 Common::mapPointToG_LBPoint(TMXTiledMap* map, const Vec2& ptMap){


	return Vec2(ptMap.x*map->getTileSize().width,
				(map->getMapSize().height - ptMap.y - 1)*map->getTileSize().height);
}
Vec2 Common::pointToMap(TMXTiledMap* map, const Vec2& pt){

	return Vec2((int)(pt.x / map->getTileSize().width),
				map->getMapSize().height - 1 - (int)(pt.y / map->getTileSize().height));

}

SpriteFrame* Common::getSpriteFrame(const char* filename, int pos, int width){
	Texture2D*  texture = TextureCache::getInstance()->addImage(filename);
	return  SpriteFrame::createWithTexture(texture, Rect(width*pos, 0, width, texture->getContentSize().height));

}