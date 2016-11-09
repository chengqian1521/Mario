#ifndef __COMMON_H__
#define __COMMON_H__
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
#define winSize CCDirector::sharedDirector()->getWinSize()
#define center  ccp(winSize.width/2,winSize.height/2)

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
	static CCScene* scene(CCLayer * layer);
	static void format(char * ret, char * prefix, int v, char *suffix);
	static CCAnimation* createAnimation(const char * finename, int start, int num, int width, float  duration);
	static CCPoint mapPointToG_LBPoint(CCTMXTiledMap*,CCPoint ptMap);
	static	CCPoint pointToMap(CCTMXTiledMap*,CCPoint pt);

	static CCSpriteFrame* getSpriteFrame(const char* filename,int pos,int width);
};

#endif


