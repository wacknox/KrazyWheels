#pragma once
#include "krazywindow.h"
//..,
#define _KRAZY_GAME_OVER                string("                          Game Over !        ")
#define _KRAZY_LEVEL_COMPLETE           string("                        Level Complete ! ")
#define _KRAZY_TOUCH_REPLAY             string("  Play Again ?")
#define _KRAZY_TOUCH_NEXT				string("  Play Next !")
//..
enum KrazyLevelThriverWindowType{
	KRAZY_LEVEL_THRIVER_WND_TYPE_GameOver , 
	KRAZY_LEVEL_THRIVER_WND_TYPE_LevelComplete
};
//..,
const unsigned short KRAZY_WINDOW_THRIVE_PARAM_Id = KRAZY_WINDOW_STATEPARAM_Id + 1;
struct KrazyWindowThriverParam : public KrazyWindowStateParam
{

	KrazyWindowThriverParam(KrazyLevelThriverWindowType _type , string& _msg) : KrazyWindowStateParam(KRAZY_WINDOW_THRIVE_PARAM_Id)
    , _ThriverWndType(_type)
	, _ThriverMsg(_msg)
	{
	}
	//
	KrazyLevelThriverWindowType		_ThriverWndType;
	string                          _ThriverMsg;
}
;
class KrazyLevelThriverWindow :
	public KrazyWindow
{
public:
	KrazyLevelThriverWindow(KrazyLevelThriverWindowType _typ , string& _msg);
	virtual ~KrazyLevelThriverWindow(void);
	//....,
	virtual void create(KrazyWindowStyle _ws_ = KRAZY_WS_None);
#if 0
	virtual void render();
#endif
	void on_load();
	//..,
private:
		KrazyLevelThriverWindowType		_WndType;
		string							_Msg;
};

