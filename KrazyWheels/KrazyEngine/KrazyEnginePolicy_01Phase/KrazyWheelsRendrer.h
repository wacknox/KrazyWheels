/*
 * GameRendrer.h
 *
 *  Created on: Dec 5, 2013
 *      Author: Siddharth
		EDITED  :- WITH FINAL DESIGN NOV 01 2014
        Renamed to KrazyWheelsRendrer..,
 */
#ifndef _KRAZYWHEELSRENDRER_H_
#define _KRAZYWHEELSRENDRER_H_
#include "KrazyGameRenderingPolicy_01Phase.h"
//#include "KrazyPrimitiveDataCollections.h"
#include "KrazyLevelCityContextInterface.h"
#include "KrazyDefaultAlgoMovingVehical_01Phase.h"
#include "KrazyWindowController.h"
#include "KrazyDoubleLaneRoadRendrer.h"
#include <vector>
using namespace std;
//..,
#define _KRAZY_MIN_LEVEL		   1
#ifdef _KrazyTutorials
#define _KRAZY_MAX_LEVEL		   16
#else
#define _KRAZY_MAX_LEVEL		   7//15
#endif
#define _KRAZY_MAIN_LEVEL_START    4
#define _KRAZY_ENABLE_LOCK
//..,
enum KrazyGameRendrerVersion
{
	KRAZY_GAME_RENDRER_Version_01Phase_,
};
//....,
enum KrazyGameScreenState
{
	KRAZY_GAME_SCREEN_STATE_Default			  = 0x0000,
	KRAZY_GAME_SCREEN_STATE_InitLevel		  = KRAZY_GAME_SCREEN_STATE_Default + 5,
	KRAZY_GAME_SCREEN_STATE_WaitOn			  = KRAZY_GAME_SCREEN_STATE_Default + 6,
	KRAZY_GAME_SCREEN_STATE_GameScreen		  = KRAZY_GAME_SCREEN_STATE_Default + 7,
	KRAZY_GAME_SCREEN_STATE_GameOn			  = KRAZY_GAME_SCREEN_STATE_Default + 8,
	KRAZY_GAME_SCREEN_STATE_ThriverOn		  = KRAZY_GAME_SCREEN_STATE_Default + 9,
	KRAZY_GAME_SCREEN_STATE_CloggedOn         = KRAZY_GAME_SCREEN_STATE_Default + 10,
	KRAZY_GAME_SCREEN_STATE_ExitSequence      = KRAZY_GAME_SCREEN_STATE_Default + 11,
};
//..,
struct KrazyThriverInvokeParam
{
	KrazyThriverInvokeParam(string     _msg         ,
		KrazyLevelThriverWindowType    _wnd_typ     ,
		KrazyWindowControllerStateId   _wnd_ctrl_stateId 
		) : 
	 _Msg(_msg)
	,_WndTyp(_wnd_typ)
	,_WndCtrlStateId(_wnd_ctrl_stateId)
	{}
	 //..,
	KrazyLevelThriverWindowType  _WndTyp;
	KrazyWindowControllerStateId _WndCtrlStateId;
	string						 _Msg;
};
//..,
enum __KrazyCloggiDisplayState
{
	KRAZY_CLOGGI_DISPLAY_STATE_Display ,
	KRAZY_CLOGGI_DISPLAY_STATE_Hide    ,
	KRAZY_CLOGGI_DISPLAY_STATE_Over    ,
};

struct __KrazyCloggiiThriver
{
	__KrazyCloggiiThriver(KrazyVehical* _vehical) : 
	 _Vehical(_vehical)
    , _State(KRAZY_CLOGGI_DISPLAY_STATE_Hide)
	, _Wait(0)
	, _MaxHideCount(0)
	, _MaxDisplayCount(0)
	{
	}
    //..,
   __KrazyCloggiDisplayState     _State;
   KrazyVehical*				 _Vehical;
   unsigned int					 _Wait;
   unsigned int					 _MaxHideCount;
   unsigned int					 _MaxDisplayCount;
  
};
//..
class __KrazyCloggiiThriverList : public vector<__KrazyCloggiiThriver*> {};
class KrazyLevelThriver;
#define _ENABLE_PUBLIC_CTOR
class KrazyWheelsRendrer 
{
#ifdef _ENABLE_PUBLIC_CTOR
public:
	KrazyWheelsRendrer();
	~KrazyWheelsRendrer();
#else
private:
	KrazyWheelsRendrer();
	~KrazyWheelsRendrer();
#endif
	//...................,
	static void on_touched(KrazyTextArea* _area);
	//...................,
public:

#ifndef _ENABLE_PUBLIC_CTOR
	static KrazyWheelsRendrer* getDefault();
	static void  releaseDefault();
#endif
//..
	void create_all_rendrer();
	//..,
	 void on_resume();
	 void on_pause();
	 void on_surface_created();
	 void on_render();
	 void on_resize(int width , int height);
	 void on_touch(float x , float y);
	 void on_preferance();
	 void on_thrive(KrazyThriverInvokeParam& _thrive);
	 void on_levellisting();
	 void on_moves();
	 void on_moves_next();
	 void on_story();
	 void on_pause_on();
	 //........................
	 void onPlaySameLevelAgain();
	 void onPlayNextLevel();
	 void onBringLevels();
	 void onBringLevelScreen();
	 void onBringSettings();
	 void onBringMoves();
	 void onBringStory();
	 void onSkipStory();
	 void onMoveNext();
	 void onPlayLevel(int _index);
	 void onPause();
	 void onResume();
	 void onMainMenu_from_Level();
	 void onMute();
	 void onUnMute();
	 void onGameOn();
	 //.........................
	void on_clogged(__KrazyVehicalList& _clogged_vehicals);
	void on_clogged_on();
	 //.........................
private:
	void init();
	void init_level_tutorials();
	void on_init();
	void on_init_level();
	void clean_everything();
	void cleanUp();
	void clean_data();
	void on_refreshLeveCityContext(int level);
	void clean_rendrers();
	void create_rendrers();
	void render_window();
	void clean_for_next_level();
	void on_init_tool();
	void on_exit_sequence();
	void clean_clogilist();
	void on_set_tutorial_mode_if_there();
	//......
#ifndef _ENABLE_PUBLIC_CTOR
	static KrazyWheelsRendrer* 				_PTRMe;
#endif
//..,,,,
	KrazyGameRenderingPolicy_01Phase*       _GameRenderingPolicy;
	KrazyGameRendrerVersion					_GameRendrerVersion;
	KrazyGameScreenState					_CurrentScreenState;
	KrazyLevelCityContext*					_LevelCityContext;
	KrazyDoubleLaneRendrer*					_DoubleLaneRendrer;
	int										_Wait;
	unsigned int                            _CurrentLevel;
	unsigned int                            _CurrentThriver;
	string									_ThriverMsg;
	__KrazyCloggiiThriverList               _CloggiThriverList;
	KrazyIntList							_TutorialLevels;
	 //.................................................................
};
//..,
class KrazyWheelsPtr
{
public:
	KrazyWheelsPtr()
	{
		 _Rendrer = new KrazyWheelsRendrer;
		 _Width   = 0.0f;
		 _Height  = 0.0f;
	}
	//..,
	~KrazyWheelsPtr()
	{
		delete _Rendrer;_Rendrer = 0;
	}

	KrazyWheelsRendrer* _rendrer() { return _Rendrer; }
	void onSetMetrics(float _width , float _height)
	{
		_Width  = _width;
		_Height = _height;
	}
	void getMetrics(float& _width , float& _height)
	{
		_width  = _Width;
		_height = _Height;
	}
//..,
private:
	KrazyWheelsRendrer* _Rendrer;
	float _Width;
	float _Height;
};

#endif /* GAMERENDRER_H_ */
