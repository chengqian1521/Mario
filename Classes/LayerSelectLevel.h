#ifndef __LAYERSELECTLEVEL_H__
#define __LAYERSELECTLEVEL_H__
#include "Common.h"
class LayerSelectLevel :public CCLayer
{
public:
	CREATE_FUNC(LayerSelectLevel);
	bool init();
	void backItemCallback(CCObject*);

	// default implements are used to call script callback if exist
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	
	bool isClick(CCTouch *pTouch);
	void startGameCallback();

public:
	CCScrollView* m_view;
	int			  m_level;
};

#endif