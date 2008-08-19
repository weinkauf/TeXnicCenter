#pragma once

#include "NavigatorView.h"

class CBibView : public NavigatorTreeCtrl
{
public:
	//CBibView();
	//virtual ~CBibView();

protected:
	virtual void OnUpdate(CProjectView* pSender, LPARAM lHint, LPVOID pHint);
};
