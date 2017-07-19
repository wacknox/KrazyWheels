#pragma once
#include "KrazyWindow.h"
//..,
#define _KRAZY_EXIT_SEQUENCE				string("                          Exit Sequence !    ")
#define _KRAZY_EXIT_SEQUENCE_GAMEON         string("  Game On !")
//..,
class KrazyExitSequenceWindow : public KrazyWindow
{
public:
	KrazyExitSequenceWindow(void);
	virtual ~KrazyExitSequenceWindow(void);
	//..,
	virtual void create(KrazyWindowStyle _ws_ = KRAZY_WS_None);
	virtual void render();
	virtual KrazyBool on_touch(float _x , float _y);
	virtual void on_refresh();
	//..,
	void on_load();
};