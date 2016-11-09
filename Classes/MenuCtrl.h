#ifndef __MENUCTRL_H__
#define __MENUCTRL_H__
#include "Common.h"
class MenuCtrl:public CCMenu
{
public:
	CREATE_FUNC(MenuCtrl);
	bool init();
	void update(float);
};

#endif