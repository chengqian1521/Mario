#ifndef __APPMACROS_H__
#define __APPMACROS_H__

#define winSize (Director::getInstance()->getWinSize())
#define center  Vec2(winSize.width/2,winSize.height/2)
#define ALL_LEVEL_NUM 8
#define ARG_GRAVITY   10
#define CCLOG_DYNAMIC_ERR CCLOG("%s:%d dynamic err",__FILE__,__LINE__)

#endif /* __APPMACROS_H__ */
