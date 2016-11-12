#ifndef __COMMON_H__
#define __COMMON_H__
#include "cocos2d.h"

USING_NS_CC;

#include "AppMacros.h"

class Common
{
public:
	enum Direction
	{
		UP,
		RIGHT,
		DOWN,
		LEFT,
		NONE
	};
	enum ZO
	{
		ZO_MARIO=1000,
		ZO_MUSHROOM=100
	};
	
	static void format(char * ret, char * prefix, int v, char *suffix);
	static Animation* createAnimation(const char * finename, int start, int num, int width, float  duration);
	static Vec2 mapPointToG_LBPoint(TMXTiledMap*, const Vec2& ptMap);
	static	Vec2 pointToMap(TMXTiledMap*, const Vec2& pt);
	static SpriteFrame* getSpriteFrame(const char* filename, int pos, int width);
};

#endif


