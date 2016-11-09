#ifndef __PROGRESSBAR_H__
#define __PROGRESSBAR_H__
#include "Common.h"
class ProgressBar:public CCLayer
{
public:
	static ProgressBar* create(const char * bgFile, const char * fgFile);
	bool init(const char * bgFile, const char * fgFile);
	void setPercentage(float per);
	void setBarPostion(CCPoint pt);
	float getPercentage();
	
private:
	CCProgressTimer* m_bar;
};

#endif