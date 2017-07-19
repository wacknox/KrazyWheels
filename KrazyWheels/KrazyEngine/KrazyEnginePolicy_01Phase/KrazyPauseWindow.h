#include "KrazyWindow.h"
#define _KRAZY_GAME_TITLE_Pause   string( "          _Krazy_r_Wheels")
#define _KRAZY_RESUME			  string("                    Resume")
#define _KRAZY_MENU				  string("                   Main Menu")
//#define _KRAZY_GAME_MUSIC_MUTE     string("                    Mute")
//..,
class KrazyPauseWindow : public KrazyWindow
{
public:
	KrazyPauseWindow();
	virtual ~KrazyPauseWindow();
	virtual void create(KrazyWindowStyle _ws_ = KRAZY_WS_None);
	void on_load();

};