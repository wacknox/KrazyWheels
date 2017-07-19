#pragma once
#include "KrazyLevelThriverWindow.h"
#include "KrazyWindowPreferance.h"
#include "KrazyLevelListingWindow.h"
#include "KrazyMassiveTextWindow.h"
#include "KrazyKallbacks.h"
#include "KrazyMassiveText.h"
#include "KrazyTool.h"
#include "KrazyPauseWindow.h"
#include "KrazyExitSequenceWindow.h"
//...,
enum KrazyWindowControllerStateId
{
	KRAZY_WINDOW_DISPLAY_None					   ,
	KRAZY_WINDOW_DISPLAY_ONLI_Preferance           ,
	KRAZY_WINDOW_DISPLAY_ONLI_PlayTool             ,
	KRAZY_WINDOW_DISPLAY_LevelTriver_and_PlayTool  ,
	KRAZY_WINDOW_DISPLAY_ONLI_LevelListing		   ,
	KRAZY_WINDOW_DISPLAY_ONLI_Moves_a   		   , 
	KRAZY_WINDOW_DISPLAY_ONLI_Moves_b   		   , 
	KRAZY_WINDOW_DISPLAY_ONLI_Story				   ,
	KRAZY_WINDOW_DISPLAY_ONLI_Pause                ,
	KRAZY_WINDOW_DISPLAY_ONLI_ExitSequence         ,
};
//...,
class KrazyWindowControllerState
{
public :
	KrazyWindowControllerState(KrazyWindowControllerStateId _id) :
	     _Id(_id)
	{
	}
	//...,
	virtual ~KrazyWindowControllerState()
	{
		clean_up();
	}
	//...,
	KrazyWindowControllerStateId stateId() { return _Id; }
	//..,
	void clean_up()
	{
		int _windows = _Windows.size();
		for(int window = 0; window < _windows; window++){
			KrazyWindow* _window = _Windows[window];
			delete _window;_window = 0;
		}
		//..,
		_Windows.clear();
	}
	//..,
    void on_refresh()
	{
		int _windows = _Windows.size();
		for(int window = 0; window < _windows; window++){
			_Windows[window]->on_refresh();
		}
	}
	//...,
	void on_appendWindow(KrazyWindow* _window)
	{
		_Windows.push_back(_window);
	}
	//...,
	void render()
	{
		int _windows = _Windows.size();
		for(int window = 0; window < _windows; window++){
			_Windows[window]->render();
		}
	}
	//..,
	KrazyBool on_touch(float _x , float _y)
	{ 
		int _windows = _Windows.size();
		for(int window = 0; window < _windows; window++){
			KrazyBool _touched = _Windows[window]->on_touch(_x , _y);
			switch(_touched)
			{
			case KRAZY_BOOL_True	:   return _touched;
			}
		}
		//..,
		return KRAZY_BOOL_False;

	}
	//..,
private:
	//..,
	KrazyWindowControllerStateId       _Id;
	vector<KrazyWindow*> _Windows;
	//...,
};
//...,
class KrazyWindowStateList : public vector<KrazyWindowControllerState*> {};
//..,
#define _KRAZY_CREATE_RESPECTIVE_STATE(_state , _krazy_window) \
	if(_CurrentState){ \
		 _PreviousStateList.push_back(_CurrentState); \
	 } \
	 _CurrentState = new KrazyWindowControllerState(_state); \
	 _CurrentState->on_appendWindow(_krazy_window); \

//...,

class KrazyWheelsRendrer; 
;class KrazyWindowController
{
public:
	static KrazyWindowController* getDefault();
	static void releaseDefault();
	//..,
	KrazyBool on_touch(float _x , float _y);
	void on_textarea_touched(KrazyTextArea* _area);
	void render();
	//..,
	void on_refreshRendrer();
	void on_attachCallback(_KrazyOnTouch _callback){ _Callback = _callback; }
	void on_attachRendrer(KrazyWheelsRendrer* _rendrer);
	//..,
	void on_touched_preferances(KrazyTextArea* _area);
	void on_touched_playtool(KrazyTextArea* _area);
	void on_touched_levelthriver(KrazyTextArea* _area);
	void on_touched_levellisting(KrazyTextArea* _area);
	void on_touched_story(KrazyTextArea* _area);
	void on_touched_moves(KrazyTextArea* _area);
	void on_touched_pause(KrazyTextArea* _area);
	void on_touched_exit_sequence(KrazyTextArea* _area);
	//..,
public:
	void on_changeState(KrazyWindowControllerStateId _state , KrazyWindowStateParam* _param = 0);
	//..,
private:
	void clean_up();
	KrazyWindowController(void);
	virtual ~KrazyWindowController(void);
	
	void init();
	void create_respective_state(KrazyWindowControllerStateId _state , KrazyWindow* _krazy_window);
	void clean_previousState_if_any();
	//..,
	static KrazyWindowController*	_Me;
	KrazyWheelsRendrer*			    _Rendrer;
	//..,
	KrazyWindowControllerState*     _CurrentState;
	KrazyWindowStateList			_PreviousStateList;
	_KrazyOnTouch			        _Callback;
	//..,
};
//..,

