#ifndef __LAYERSTART_H__
#define __LAYERSTART_H__
#include "Common.h"
class LayerStart:public CCLayer
{
public:
	CREATE_FUNC(LayerStart);
	bool init();
	void startCallback(CCObject*);
	void setupCallback(CCObject*);
	void quitCallback(CCObject*);
	void aboutCallback(CCObject*);
};

#endif