#include "LayerSelectLevel.h"
#include "LayerStart.h"
#include "LayerGame.h"
bool LayerSelectLevel::init(){
	CCLayer::init();
	
	//±³¾°Í¼Æ¬
	CCSprite* bg = CCSprite::create("bg.png");
	addChild(bg);
	bg->setPosition(ccp(winSize.width / 2, winSize.height / 2));

	CCNode* node = CCNode::create();
	char finename[32] = {0};
	for (int i = 0; i <8; ++i){

		Common::format(finename, "select", i+1, ".jpg");

		CCSprite* spr = CCSprite::create(finename);
		spr->setPosition(ccp(winSize.width / 2 + i*winSize.width, winSize.height/2));
		spr->setTag(1000 + i);
		node->addChild(spr);
	}
	CCScrollView* view = CCScrollView::create(winSize, node);
	addChild(view);
	m_view = view;
	view->setContentSize(CCSize(8*winSize.width,winSize.height));
	view->setDirection(kCCScrollViewDirectionHorizontal);


	//·µ»Ø°´Å¥
	CCMenuItemImage* backItem = CCMenuItemImage::create("backA.png",
														"backB.png",
														this,
														menu_selector(LayerSelectLevel::backItemCallback));
	backItem->setAnchorPoint(ccp(1, 0));
	backItem->setPosition(ccp(winSize.width/2,-winSize.height/2));
	CCMenu* menu = CCMenu::create(backItem, NULL);
	addChild(menu);

	//¿ªÆô´¥Ãþ
	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);

	return true;
}
void LayerSelectLevel::backItemCallback(CCObject*){

	CCDirector::sharedDirector()->replaceScene(Common::scene(LayerStart::create()));
}

// default implements are used to call script callback if exist
bool LayerSelectLevel::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
	
	return true;
}
void LayerSelectLevel::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){

}
void LayerSelectLevel::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){

	if (isClick(pTouch)){
		CCNode* node = m_view->getContainer();
		CCPoint pt=node->convertTouchToNodeSpace(pTouch);
		CCSprite * clickSpr;
		for (int i = 0; i < 8; ++i){
			clickSpr = (CCSprite *)node->getChildByTag(1000 + i);
			if (clickSpr->boundingBox().containsPoint(pt)){
				m_level = i + 1;
				//½øÈëÓÎÏ·³¡¾°
				clickSpr->setZOrder(1000);
				CCMoveTo*   to = CCMoveTo::create(0.2f, node->convertToNodeSpace(center));
				
				CCScaleBy* by = CCScaleBy::create(0.2f, 1.2f);
				CCCallFunc* startGameFunc = CCCallFunc::create(this,callfunc_selector(LayerSelectLevel::startGameCallback));
				clickSpr->runAction(CCSequence::create(to,by, startGameFunc, NULL));
				break;
			}
			
		}

		

	}

}
void LayerSelectLevel::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){}

bool LayerSelectLevel::isClick(CCTouch *pTouch){
	CCPoint ptStart = pTouch->getStartLocation();
	CCPoint endLoc = pTouch->getLocation();

	return ptStart.getDistanceSq(endLoc) <= 9;
}

void LayerSelectLevel::startGameCallback(){
	CCDirector::sharedDirector()->replaceScene(Common::scene(LayerGame::create(m_level)));
}