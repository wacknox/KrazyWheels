#pragma once
#include "krazywindow.h"
//..,
#if 0
#ifndef _KrazyTutorials
#define _KrazyTutorials
#endif
#endif
//..,
#ifdef _KrazyTutorials
#define _KRAZY_MAX_LEVELS		16
#else
#define _KRAZY_MAX_LEVELS		7
#endif
#define _KRAZY_MAINMENU_LL      string("Main Menu")
class KrazyLevelListingWindow :
	public KrazyWindow
{
public:
	KrazyLevelListingWindow(void);
	virtual ~KrazyLevelListingWindow(void);
	//..,
	void create(KrazyWindowStyle _ws_ = KRAZY_WS_None);
	void on_load();
};