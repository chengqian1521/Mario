#include "ProgressBar.h"

ProgressBar* ProgressBar::create(const char * bgFile, const char * fgFile){
	ProgressBar* pRet = new ProgressBar();
	if (pRet&&pRet->init(bgFile, fgFile)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}

	return pRet;

}
bool ProgressBar::init(const char * bgFile, const char * fgFile){
	CCLayer::init();

	//进度条背景
	CCSprite*  bar_bg = CCSprite::create(bgFile);
	bar_bg->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	addChild(bar_bg);

	CCSprite*       timer_spr = CCSprite::create(fgFile);

	//加载进度条
	m_bar = CCProgressTimer::create(timer_spr);
	addChild(m_bar);
	m_bar->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	m_bar->setPercentage(0);
	m_bar->setType(kCCProgressTimerTypeBar);
	m_bar->setMidpoint(ccp(0, 0));
	m_bar->setBarChangeRate(ccp(1, 0));

	


	return true;
}
void ProgressBar::setPercentage(float per){
	m_bar->setPercentage(per);
}
void ProgressBar::setBarPostion(CCPoint pt){
	m_bar->setPosition(pt);
}
float ProgressBar::getPercentage(){
	return m_bar->getPercentage();
}


