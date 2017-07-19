#include "stdafx.h"
#include "KrazyWindowController.h"
#include "KrazyWheelsRendrer.h"
//..,
KrazyWindowController* KrazyWindowController::_Me = 0;
KrazyWindowController::KrazyWindowController(void) : 
_Rendrer(0)
,_CurrentState(0)
{
	
}
//...,
KrazyWindowController::~KrazyWindowController(void)
{
	clean_up();
}
//...,
KrazyWindowController* KrazyWindowController::getDefault()
{
	if(!_Me){
		_Me = new KrazyWindowController;
		_Me->init();
	}

	return _Me;
}
//...,
void KrazyWindowController::releaseDefault()
{
	if(_Me){
		delete _Me;_Me = 0;
	}
}
//...,
void KrazyWindowController::init()
{
	
}
//...,
void KrazyWindowController::clean_up()
{
	KLEAN_a_bit_of_KRAZINESS(_CurrentState);_CurrentState = 0;
	clean_previousState_if_any();
}
//...,
void KrazyWindowController::create_respective_state(KrazyWindowControllerStateId _state , KrazyWindow* _krazy_window)
{
	if(_CurrentState){ 
		 _PreviousStateList.push_back(_CurrentState); 
	 } 
	//..,
	_CurrentState = new KrazyWindowControllerState(_state); 
	_CurrentState->on_appendWindow(_krazy_window); 
	
}
//..,
void KrazyWindowController::on_changeState(KrazyWindowControllerStateId _state , KrazyWindowStateParam* _param /*= 0*/)
{
		switch(_state)
		{
		case KRAZY_WINDOW_DISPLAY_ONLI_Preferance			: 
			{
			 KrazyWindow* _preferanceWindow = new KrazyWindowPreferance;
			 _preferanceWindow->create();
			 _KRAZY_CREATE_RESPECTIVE_STATE(_state , _preferanceWindow);
			}
			 break;
		case KRAZY_WINDOW_DISPLAY_ONLI_PlayTool             : 
			{
			 KrazyWindow* _toolWindow = new KrazyToolWindow(_Rendrer);
			 _toolWindow->create();
			 _KRAZY_CREATE_RESPECTIVE_STATE(_state , _toolWindow);
			}
			 break;
		case KRAZY_WINDOW_DISPLAY_LevelTriver_and_PlayTool  : 
			{
				KrazyWindowThriverParam* _thriver_param = (KrazyWindowThriverParam*)_param;
				KrazyWindow* _levelThriverWindow = new KrazyLevelThriverWindow(_thriver_param->_ThriverWndType , _thriver_param->_ThriverMsg);
				_levelThriverWindow->create();
				_KRAZY_CREATE_RESPECTIVE_STATE(_state , _levelThriverWindow);
			}
			break;
		case KRAZY_WINDOW_DISPLAY_ONLI_LevelListing         : 
			{
			 KrazyWindow* _levelListing = new KrazyLevelListingWindow;
			 _levelListing->create();
			 _KRAZY_CREATE_RESPECTIVE_STATE(_state , _levelListing);
			}
			 break;
		case KRAZY_WINDOW_DISPLAY_ONLI_Story:
			{
				KrazyStoryWindow* _storyWindow = new KrazyStoryWindow;
				KrazyStringList _list;
#if 1
				_list.push_back(g_story_line);
#else
				_list.push_back(g_story_line_1);
				_list.push_back(g_story_line_2);
				_list.push_back(g_story_line_3);
				_list.push_back(g_story_line_4);
				_list.push_back(g_story_line_5);
				_list.push_back(g_story_line_6);
				_list.push_back(g_story_line_7);
#endif
				_list.push_back(_KRAZY_SKIP_);
				_storyWindow->append_text(_list);
				_storyWindow->create();
				_KRAZY_CREATE_RESPECTIVE_STATE(_state , _storyWindow);
			 }
			 break;
		case KRAZY_WINDOW_DISPLAY_ONLI_Moves_a:
			 {
				KrazyMovesWindow* _movesWindow = new KrazyMovesWindow(KRAZY_MOVES_WINDOW_Id_a);
				KrazyStringList _list;
#if 1
				_list.push_back(g_moves_frame_1);
#else
				_list.push_back(g_moves_frame_1_line_1);
				_list.push_back(g_moves_frame_1_line_2);
				_list.push_back(g_moves_frame_1_line_3);
				_list.push_back(g_moves_frame_1_line_4);
				_list.push_back(g_moves_frame_1_line_5);
				_list.push_back(g_moves_frame_1_line_6);
				_list.push_back(g_moves_frame_1_line_7);
				_list.push_back(g_moves_frame_1_line_8);
#endif
				_list.push_back(_KRAZY_SKIP_);
				_list.push_back(_KRAZY_NEXT_);
				_movesWindow->append_text(_list);
				_movesWindow->create();
				_KRAZY_CREATE_RESPECTIVE_STATE(_state , _movesWindow);
			 }
			 break;
		case KRAZY_WINDOW_DISPLAY_ONLI_Moves_b:
			 {
				KrazyMovesWindow* _movesWindow = new KrazyMovesWindow(KRAZY_MOVES_WINDOW_Id_b);
				KrazyStringList _list;
				_list.push_back(g_moves_frame_2);
#if 0
				_list.push_back(g_moves_frame_2_line_1);
				_list.push_back(g_moves_frame_2_line_2);
				_list.push_back(g_moves_frame_2_line_3);
				_list.push_back(g_moves_frame_2_line_4);
				_list.push_back(g_moves_frame_2_line_5);
#endif
				_list.push_back(_KRAZY_SKIP_);
				_list.push_back(_KRAZY_BACK_);
				_movesWindow->append_text(_list);
				_movesWindow->create();
				_KRAZY_CREATE_RESPECTIVE_STATE(_state , _movesWindow);
				
			 }
			 break;
		case KRAZY_WINDOW_DISPLAY_ONLI_Pause:
			{
				KrazyWindow* _pauseWindow = new KrazyPauseWindow;
				_pauseWindow->create();
				_KRAZY_CREATE_RESPECTIVE_STATE(_state , _pauseWindow);
			}
			break;
		case KRAZY_WINDOW_DISPLAY_ONLI_ExitSequence:
			{
				KrazyExitSequenceWindow* _exitSequence = new KrazyExitSequenceWindow;
				_exitSequence->create();
				_KRAZY_CREATE_RESPECTIVE_STATE(_state , _exitSequence);

			}
			break;
#if 0
		case KRAZY_WINDOW_DISPLAY_None:
			{
				if(_CurrentState)
				{
					_CurrentState = 0;
					_PreviousStateList.push_back(_CurrentState);
				}
			}
			break;
#endif
		}
}
//..,
void KrazyWindowController::on_attachRendrer(KrazyWheelsRendrer* _rendrer)
{
	_Rendrer = _rendrer;
}
//..,,
void KrazyWindowController::render()
{
	clean_previousState_if_any();
	//..,
	if(_CurrentState)
		_CurrentState->render();
}
//...,
KrazyBool KrazyWindowController::on_touch(float _x , float _y)
{
	/* current state is assigned to auxillary state to avoid any unwinding problems if
	   current state is changed
	*/

	KrazyWindowControllerState* _aux_state = _CurrentState;
	if(_aux_state)
		return _aux_state->on_touch(_x , _y);
	//..,
	return KRAZY_BOOL_False;
}
//..,
void KrazyWindowController::on_textarea_touched(KrazyTextArea* _area)
{
	if(!_Rendrer)
		return;
	
	/* current state is assigned to auxillary state to avoid any unwinding problems if
	   current state is changed
	*/

	KrazyWindowControllerState* _aux_state = _CurrentState;
	if(_aux_state){
		switch(_aux_state->stateId())
		{
		case KRAZY_WINDOW_DISPLAY_ONLI_Preferance			: on_touched_preferances(_area);   break;
		case KRAZY_WINDOW_DISPLAY_ONLI_PlayTool             : on_touched_playtool(_area);      break;
		case KRAZY_WINDOW_DISPLAY_LevelTriver_and_PlayTool  : on_touched_levelthriver(_area);  break;
		case KRAZY_WINDOW_DISPLAY_ONLI_LevelListing         : on_touched_levellisting(_area);  break;
		case KRAZY_WINDOW_DISPLAY_ONLI_Story				: on_touched_story(_area);		   break;
		case KRAZY_WINDOW_DISPLAY_ONLI_Moves_a				:
		case KRAZY_WINDOW_DISPLAY_ONLI_Moves_b				: on_touched_moves(_area);		   break;
		case KRAZY_WINDOW_DISPLAY_ONLI_Pause				: on_touched_pause(_area);         break;
		case KRAZY_WINDOW_DISPLAY_ONLI_ExitSequence         : on_touched_exit_sequence(_area); break;
		}
	}
}
//..,
void KrazyWindowController::on_touched_preferances(KrazyTextArea* _area)
{
	string _text = _area->get_Text();
	if(_text == _KRAZY_GAME_PLAY){
		_Rendrer->onBringLevelScreen();
		//clean window..,
	}
	//..,
	else if(_text == _KRAZY_GAME_MOVES){
		_Rendrer->onBringMoves();
	} 
	//..,
	else if(_text == _KRAZY_GAME_STORY){
		_Rendrer->onBringStory();
	}
	//..,
	else if(_text == _KRAZY_GAME_MUSIC_MUTE){
		_Rendrer->onMute();
		_area->on_setText(_KRAZY_GAME_MUSIC_UNMUTE);
		_area->calculate_textarea();
	}
	//..,
	else if(_text == _KRAZY_GAME_MUSIC_UNMUTE){
		_Rendrer->onUnMute();
		_area->on_setText(_KRAZY_GAME_MUSIC_MUTE);
		_area->calculate_textarea();
	}
	 
}
//..,
void KrazyWindowController::on_touched_playtool(KrazyTextArea* _area)
{
	string _text = _area->get_Text();
	
}
//..,
void KrazyWindowController::on_touched_levelthriver(KrazyTextArea* _area)
{
	string _text = _area->get_Text();
	if(_text == _KRAZY_TOUCH_REPLAY){
		_Rendrer->onPlaySameLevelAgain();
		//clean window..,
	}
	else if(_text == _KRAZY_TOUCH_NEXT){
		_Rendrer->onPlayNextLevel();
		//clean window..,
	}		
}
//...
void KrazyWindowController::on_touched_levellisting(KrazyTextArea* _area)
{
	string _text = _area->get_Text();
	int _level = atoi(_text.c_str());
	if(_level > 0){
		_Rendrer->onPlayLevel(_level);
	}
	else{
		if(_text == _KRAZY_MAINMENU_LL){
			_Rendrer->onSkipStory();
		}
	}

}
//..,
void KrazyWindowController::on_touched_story(KrazyTextArea* _area)
{
	string _text = _area->get_Text();
	if(_text == _KRAZY_SKIP_){
		_Rendrer->onSkipStory();
	}
}
//..,
void KrazyWindowController::on_touched_moves(KrazyTextArea* _area)
{
	string _text = _area->get_Text();
	if(_text == _KRAZY_SKIP_){
		_Rendrer->onSkipStory();
	}
	else if(_text == _KRAZY_NEXT_){
		_Rendrer->onMoveNext();
	}
	else if(_text == _KRAZY_BACK_){
		_Rendrer->onBringMoves();
	}
}
//..,
void KrazyWindowController::on_touched_pause(KrazyTextArea* _area)
{
	string _text = _area->get_Text();
	if(_text == _KRAZY_RESUME){
		_Rendrer->onResume();
	}
	//..,
	else if(_text == _KRAZY_MENU){
		_Rendrer->onMainMenu_from_Level();
	}
	//..,
	else if(_text == _KRAZY_GAME_MUSIC_MUTE){
		_Rendrer->onMute();
		_area->on_setText(_KRAZY_GAME_MUSIC_UNMUTE);
		_area->calculate_textarea();
	}
	//..,
	else if(_text == _KRAZY_GAME_MUSIC_UNMUTE){
		_Rendrer->onUnMute();
		_area->on_setText(_KRAZY_GAME_MUSIC_MUTE);
		_area->calculate_textarea();
	}
}
//..,
void KrazyWindowController::on_touched_exit_sequence(KrazyTextArea* _area)
{
	string _text = _area->get_Text();
	if(_text == _KRAZY_EXIT_SEQUENCE_GAMEON){
		_Rendrer->onGameOn();
	}
}
//..,
void KrazyWindowController::clean_previousState_if_any()
{
	int _count = _PreviousStateList.size();
	if(_count > 0){
		for(int i = 0; i < _count; i++){
			KrazyWindowControllerState* _window_state = _PreviousStateList[i];
			delete _window_state;_window_state = 0;
		}
		//..,
		_PreviousStateList.clear();
	}
}
//..,
void KrazyWindowController::on_refreshRendrer()
{
	if(_CurrentState){
		_CurrentState->on_refresh();
	}
}