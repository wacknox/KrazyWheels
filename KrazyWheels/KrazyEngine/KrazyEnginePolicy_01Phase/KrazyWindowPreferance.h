#ifndef __KRAZY_WINDOW_PREFERANCE__
#define __KRAZY_WINDOW_PREFERANCE__
#pragma once
#include "krazywindow.h"
#define _KRAZY_GAME_TITLE   string( "          _Krazy_r_Wheels")
#define _KRAZY_GAME_PLAY    string("                    Play")
#define _KRAZY_GAME_MOVES   string("                    Moves")
#define _KRAZY_GAME_STORY   string("                    Story")
//..,
class KrazyWindowPreferance :
	public KrazyWindow
{
public:
	KrazyWindowPreferance(void);
	virtual ~KrazyWindowPreferance(void);
	//..,
	virtual void create(KrazyWindowStyle _ws_ = KRAZY_WS_None);
	void on_load();

};

#endif
