/*
 * GameRendrer.cpp
 *
 *  Created on: Dec 5, 2013
 *      Author: Siddharth
 */

#include "stdafx.h"
#include <Windows.h>
#include "KrazyWheelsRendrer.h"
#include "KrazyLevelCityNetwork.h"
#include "KrazyVehicalCache.h"
#include "KrazyViewPortContext.h"
#include "KrazyEntryIndicatorCache.h"
#include "KrazyLevelThriver.h"
#include "KrazyTextRendrer.h"
#include "KrazyRouteThriver.h"
#include "KrazyDutyKueue.h"
#include "KrazyPlayToolRect.h"
#include "KrazyColorDefinations.h"
#include "KrazyAsset.h"
#include "KrazyAudioThriver.h"
#include "KrazyClogDetector.h" 
#include "KrazyRes.h"
#include "KrazyVehicalShades.h"
#include "KrazyTutorialThriver.h"
#include "GLeish.h"
#include "KrazyBackgroundTrack.h"

#if 0
#ifndef _KrazyTutorials
#define _KrazyTutorials
#endif
#endif

#ifdef _WINDOWS
#define _SLEEP_VAL_FRAME_CTRL  20
#endif

///////////////////////////////////////////////////////
//#define _ENABLE_FIX_THRIVER
#ifndef _ENABLE_PUBLIC_CTOR
KrazyWheelsRendrer* KrazyWheelsRendrer::_PTRMe = 0;
#endif
///////////////////////////////////////////////////////
KrazyWheelsRendrer::KrazyWheelsRendrer() :
  _GameRendrerVersion(KRAZY_GAME_RENDRER_Version_01Phase_)
, _CurrentScreenState(KRAZY_GAME_SCREEN_STATE_GameScreen)
, _Wait(0)
, _CurrentLevel(_KRAZY_MIN_LEVEL)
{
	_DoubleLaneRendrer = new KrazyDoubleLaneRendrer;
	_LevelCityContext  = new KrazyLevelCityContext;
	_GameRenderingPolicy = new KrazyGameRenderingPolicy_01Phase;
#ifdef _KrazyTutorials
	KrazyTutorialThriver::get_default();
	init_level_tutorials();
#endif
}
///////////////////////////////////////////////////////
KrazyWheelsRendrer::~KrazyWheelsRendrer()
{
	clean_everything();
	KLEAN_a_bit_of_KRAZINESS(_DoubleLaneRendrer);
	KLEAN_a_bit_of_KRAZINESS(_LevelCityContext);
	KLEAN_a_bit_of_KRAZINESS(_GameRenderingPolicy);
	
}
/////////////////////////////////////////////////////
#ifndef _ENABLE_PUBLIC_CTOR
KrazyWheelsRendrer* KrazyWheelsRendrer::getDefault()
{

	if(_PTRMe == 0){
		_PTRMe = new KrazyWheelsRendrer;
	}
	return _PTRMe;
}
//////////////////////////////////////////////////////
void  KrazyWheelsRendrer::releaseDefault()
{
	if(_PTRMe)
	{
		delete _PTRMe;_PTRMe = NULL;
	}
}
#endif
//////////////////////////////////////////////////////
void KrazyWheelsRendrer::init_level_tutorials()
{
	_TutorialLevels.push_back(1);
	_TutorialLevels.push_back(2);
	_TutorialLevels.push_back(6);
	_TutorialLevels.push_back(8);
	_TutorialLevels.push_back(9);
	_TutorialLevels.push_back(11);
	//_TutorialLevels.push_back(15);
}
//..,
void KrazyWheelsRendrer::create_all_rendrer()
{
	KrazyBitmapFontFile::getDefault();
	KrazyTextAreaRendrer::getDefault();
	KrazyWindowRendrer::getDefault();
	KrazyToolRendrer::getDefault();
	_DoubleLaneRendrer->initialize();
	_GameRenderingPolicy->create_rendrer();
}
/////////////////////////////////////////////////////
void KrazyWheelsRendrer::on_init()
{
	KrazyNetworkCity_01Phase::getNetwork();
	KrazyLevelThriver::create_with_level_context(_LevelCityContext);
	KrazyVehicalCache::getCache();
	KrazyEntryIndicatorCache::getCache();
	KrazyRouteThriverFactory::getDefault();
	KrazyDutyKueue::getDefault();
	KrazyClogDetector::getDefault()->attach_rendrer(this);
	_LevelCityContext->on_refresh_level_onli(_CurrentLevel);
	_GameRenderingPolicy->reset();
	//..,
#if 1
	//..,on_exit_sequence();
#else
	on_init_tool();
#endif
}
///..,
void KrazyWheelsRendrer::on_init_level()
{
	_CurrentScreenState = KRAZY_GAME_SCREEN_STATE_ExitSequence;
	_DoubleLaneRendrer->onRefresh(_LevelCityContext);
	//on_exit_sequence();
}
///..,
////////////////////////////////////////////////////
void KrazyWheelsRendrer::init()
{
	_CurrentScreenState = KRAZY_GAME_SCREEN_STATE_ExitSequence;
	KrazyNetworkCity_01Phase::getNetwork()->on_set_rendrer(_DoubleLaneRendrer);
	KrazyLevelThriver::create_with_level_context(_LevelCityContext);
	KrazyVehicalCache::getCache();
	KrazyEntryIndicatorCache::getCache();
	KrazyRouteThriverFactory::getDefault();
	KrazyDutyKueue::getDefault();
	KrazyClogDetector::getDefault()->attach_rendrer(this);
#if 0
	_CurrentLevel = _KRAZY_MAX_LEVEL;
#endif
	on_refreshLeveCityContext(_CurrentLevel);
	//..,
#if 1
	on_exit_sequence();
#else
	on_init_tool();
#endif
}
//////////////////////////////////////////////////////
void KrazyWheelsRendrer::on_init_tool()
{
	if(!KrazyPlayToolRect::get_default()->empty()){
		KrazyWindowController::getDefault()->on_changeState(KRAZY_WINDOW_DISPLAY_ONLI_PlayTool);
	}
}
///
void KrazyWheelsRendrer::on_exit_sequence()
{
	KrazyWindowController::getDefault()->on_changeState(KRAZY_WINDOW_DISPLAY_ONLI_ExitSequence);
}
//..,
void KrazyWheelsRendrer::clean_everything()
{
	cleanUp();
	clean_rendrers();
	KrazyWindowController::releaseDefault();
#ifndef _WINDOWS
	KrazyAssetManager::releaseDefault();
	KrazyAudioThriver::releaseDefault();
#endif
	KrazyPlayToolRect::release_default();
	KrazyRes::releaseDefault();
	KrazyVehicalShadeFactory::releaseDefault();
#ifdef _KrazyTutorials
	KrazyTutorialThriver::release_default();
#endif
	
}
///.
void KrazyWheelsRendrer::cleanUp()
{
	//.., this cleanup is only for gameplay data ..,, corrosponding to levels..., 
	clean_clogilist();
	KrazyNetworkCity_01Phase::destroyNetwork();
	KrazyVehicalCache::destroyCache();
	KrazyEntryIndicatorCache::destroyCache();
	KrazyLevelThriver::release_default();
	KrazyRouteThriverFactory::releaseDefault();
	KrazyDutyKueue::releaseDefault();
	KrazyClogDetector::releaseDefault();
#ifdef _KrazyTutorials
	KrazyTutorialThriver::release_default();
#endif
}
//////////////////////////////////////////////////
void KrazyWheelsRendrer::clean_clogilist()
{
	int _count = _CloggiThriverList.size();
	for(int i = 0; i < _count; i++){
		__KrazyCloggiiThriver* _clogi = _CloggiThriverList[i];
		delete _clogi;_clogi = 0;
	}
	//..,
	_CloggiThriverList.clear();
	_CurrentThriver = 0;
}

void KrazyWheelsRendrer::clean_data()
{
	KrazyNetworkCity_01Phase::cleanNetworkData();
	KrazyVehicalCache::cleanData();
	KrazyEntryIndicatorCache::cleanData();
	KrazyLevelThriver::get_default()->reset();
}
//
void KrazyWheelsRendrer::clean_for_next_level()
{
	KrazyNetworkCity_01Phase::destroyNetwork();
	KrazyVehicalCache::destroyCache();
	KrazyEntryIndicatorCache::destroyCache();
}
//
void KrazyWheelsRendrer::on_refreshLeveCityContext(int level)
{
	switch(_GameRendrerVersion){
	case KRAZY_GAME_RENDRER_Version_01Phase_:
#if 0
		clean_data();
#endif
		_LevelCityContext->on_refresh(level);
		_GameRenderingPolicy->reset();
	}
}
//
void KrazyWheelsRendrer::clean_rendrers()
{	
	
	KrazyBitmapFontFile::releaseDefault();
	KrazyTextAreaRendrer::releaseDefault();
	KrazyWindowRendrer::releaseDefault();
	KrazyToolRendrer::releaseDefault();
	_DoubleLaneRendrer->reset();
	_GameRenderingPolicy->reset_rendrer();
	
}
//
void KrazyWheelsRendrer::create_rendrers()
{
	_GameRenderingPolicy->create_rendrer();
}
/////////////////////////////////////////////////////////   JNI Thriver ////////////////////////////////////////////////////////// 
void KrazyWheelsRendrer::on_resume()
{
	KrazyWindowController::getDefault()->on_attachRendrer(this);
	bool _refresh(false);bool _pause_on(false);
	switch(_CurrentScreenState)
	{
	case KRAZY_GAME_SCREEN_STATE_ExitSequence : _refresh = true;break;
	case KRAZY_GAME_SCREEN_STATE_WaitOn		  : 
	case KRAZY_GAME_SCREEN_STATE_GameOn		  : 	 
		 _refresh = true;_pause_on = true;
		 break;
	}
	//..,
	if(_refresh){
		_DoubleLaneRendrer->onRefresh(_LevelCityContext);
	}
	//..,
	if(_pause_on){
		on_pause_on(); 
	}
}
//
void KrazyWheelsRendrer::on_pause()
{

}
//
void KrazyWheelsRendrer::on_surface_created()
{
	clean_rendrers();
	create_all_rendrer();
	KrazyWindowController::getDefault()->on_refreshRendrer();
}
//
#define _RESET_WAIT  200
#ifdef _ENABLE_FIX_THRIVER
void KrazyWheelsRendrer::on_render()
{
	glClearColor(0.0f, 0.0f , 0.0f , 1.0f);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	switch(_GameRendrerVersion){
	case KRAZY_GAME_RENDRER_Version_01Phase_   :
		switch(_CurrentScreenState){
		case KRAZY_GAME_SCREEN_STATE_CloggedOn             : 
			 _DoubleLaneRendrer->render();
			 on_clogged_on();         
			 break;
		case KRAZY_GAME_SCREEN_STATE_InitLevel	           : on_init_level();/*init();*/				  break;
		case KRAZY_GAME_SCREEN_STATE_GameScreen            : 	 
			{
				
				KrazyWindowController::getDefault()->on_attachRendrer(this);
				on_preferance();

			}
			break;
		case KRAZY_GAME_SCREEN_STATE_WaitOn      :
			_DoubleLaneRendrer->render();
			_GameRenderingPolicy->render_vehical();
			break;
		case KRAZY_GAME_SCREEN_STATE_ExitSequence : /*_DoubleLaneRendrer->render();*/
			/*CurrentScreenState = KRAZY_GAME_SCREEN_STATE_GameOn;
			 */
			on_exit_sequence();
			break;
		case KRAZY_GAME_SCREEN_STATE_GameOn:
			{
				//Call game rendering policy directly..,
				KrazyLevelThriver* _levelTriver = KrazyLevelThriver::get_default();
				_DoubleLaneRendrer->render();
				_GameRenderingPolicy->onExecute();
				switch(_levelTriver->level_mode())
				{
				case KRAZY_LEVELMODE_gameover       : 
					{
						_CurrentScreenState = KRAZY_GAME_SCREEN_STATE_WaitOn;
						KrazyThriverInvokeParam _thrive(_levelTriver->help_tip() ,
							KRAZY_LEVEL_THRIVER_WND_TYPE_GameOver , 
							KRAZY_WINDOW_DISPLAY_LevelTriver_and_PlayTool);
						//..,
						on_thrive(_thrive);
					}
					break;
				case KRAZY_LEVELMODE_levelcomplete  : 
					{
						_CurrentScreenState = KRAZY_GAME_SCREEN_STATE_WaitOn; 
						KrazyThriverInvokeParam _thrive(_levelTriver->derrogation() ,
							KRAZY_LEVEL_THRIVER_WND_TYPE_LevelComplete , 
							KRAZY_WINDOW_DISPLAY_LevelTriver_and_PlayTool);
						//..,
						on_thrive(_thrive);
					}
					break;
				}
			}
			break;
		case KRAZY_GAME_SCREEN_STATE_ThriverOn	: 
			{
				_CurrentScreenState = KRAZY_GAME_SCREEN_STATE_WaitOn;
				KrazyThriverInvokeParam _thrive(_ThriverMsg ,
							KRAZY_LEVEL_THRIVER_WND_TYPE_LevelComplete , 
							KRAZY_WINDOW_DISPLAY_LevelTriver_and_PlayTool);
						//..,
				on_thrive(_thrive);
			}
			break;
		}
		render_window();
		break;
	}
}
#else
void KrazyWheelsRendrer::on_render()
{
	glClearColor(0.0f, 0.0f , 0.0f , 1.0f);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	switch(_GameRendrerVersion){
	case KRAZY_GAME_RENDRER_Version_01Phase_   :
		switch(_CurrentScreenState){
		case KRAZY_GAME_SCREEN_STATE_CloggedOn             : 
			 _DoubleLaneRendrer->render();
			 on_clogged_on();         
			 break;
		case KRAZY_GAME_SCREEN_STATE_InitLevel	           : init();		  break;
		case KRAZY_GAME_SCREEN_STATE_GameScreen            : 	 
			{
				
				KrazyWindowController::getDefault()->on_attachRendrer(this);
				on_preferance();

			}
			break;
		case KRAZY_GAME_SCREEN_STATE_WaitOn      :
			_DoubleLaneRendrer->render();
			_GameRenderingPolicy->render_vehical();
			break;
		case KRAZY_GAME_SCREEN_STATE_ExitSequence : _DoubleLaneRendrer->render();	break;
		case KRAZY_GAME_SCREEN_STATE_GameOn:
			{
				//Call game rendering policy directly..,
				KrazyLevelThriver* _levelTriver = KrazyLevelThriver::get_default();
				_DoubleLaneRendrer->render();
				_GameRenderingPolicy->onExecute();
				switch(_levelTriver->level_mode())
				{
				case KRAZY_LEVELMODE_gameover       : 
					{
						_CurrentScreenState = KRAZY_GAME_SCREEN_STATE_WaitOn;
						KrazyThriverInvokeParam _thrive(_levelTriver->help_tip() ,
							KRAZY_LEVEL_THRIVER_WND_TYPE_GameOver , 
							KRAZY_WINDOW_DISPLAY_LevelTriver_and_PlayTool);
						//..,
						on_thrive(_thrive);
					}
					break;
				case KRAZY_LEVELMODE_levelcomplete  : 
					{
						_CurrentScreenState = KRAZY_GAME_SCREEN_STATE_WaitOn; 
						KrazyThriverInvokeParam _thrive(_levelTriver->derrogation() ,
							KRAZY_LEVEL_THRIVER_WND_TYPE_LevelComplete , 
							KRAZY_WINDOW_DISPLAY_LevelTriver_and_PlayTool);
						//..,
						on_thrive(_thrive);
					}
					break;
				}
#ifdef _WINDOWS
				Sleep(_SLEEP_VAL_FRAME_CTRL);
#endif
			}
			break;
		case KRAZY_GAME_SCREEN_STATE_ThriverOn	: 
			{
				_CurrentScreenState = KRAZY_GAME_SCREEN_STATE_WaitOn;
				KrazyThriverInvokeParam _thrive(_ThriverMsg ,
							KRAZY_LEVEL_THRIVER_WND_TYPE_LevelComplete , 
							KRAZY_WINDOW_DISPLAY_LevelTriver_and_PlayTool);
						//..,
				on_thrive(_thrive);
			}
			break;
		}
		render_window();
		break;
	}
	
}
#endif
//
void KrazyWheelsRendrer::on_resize(int width , int height)
{
	KrazyViewPortContext::_Width  = width;
	KrazyViewPortContext::_Height = height;
//...........
	_DoubleLaneRendrer->on_resize();
	_GameRenderingPolicy->on_resize(width , height);
	glViewport(0,0,width,height);
}
//..,
void KrazyWheelsRendrer::render_window()
{
	KrazyWindowController::getDefault()->render();
}
//..,
void KrazyWheelsRendrer::on_touch(float x , float y)
{  
	switch(_CurrentScreenState){
	case KRAZY_GAME_SCREEN_STATE_GameOn :
		{
			KrazyVehicalCache* ptr_cache = KrazyVehicalCache::getCache(); // make the list directly retrieved..,
			int vehical_count = ptr_cache->get_VehicalsInTheNetworkCount();
			for(int i = 0; i < vehical_count; i++){
				KrazyVehical* _vehical = ptr_cache->get_VehicalFromTheNetwork(i);
				switch(_vehical->touch_detection_enbaled())
				{
				case KRAZY_BOOL_True:
					switch(_vehical->on_touch(x,y))
					{
					case KRAZY_BOOL_True:
						switch(_vehical->waitstate())
						{
						case KRAZY_VEHICAL_WAIT_STATE_Default	:	_vehical->on_touched();	 break;
						}
						return;
					}
					break;
				}
				//..,
#if 0
				switch(_vehical->reflection_touch_detection_enabled())
				{
				case KRAZY_BOOL_True:
					switch(_vehical->on_touch_reflection(x,y))
					{
					case KRAZY_BOOL_True:return;
					}
					break;
				}
#endif
			}
			//..,
			for(int i = 0; i < vehical_count; i++){
				KrazyVehical* _vehical = ptr_cache->get_VehicalFromTheNetwork(i);
#if 0
				switch(_vehical->waitstate())
				{
				case KRAZY_VEHICAL_WAIT_STATE_Default	:
					 break;
				default:continue;
				}
#endif
				//..,
				switch(_vehical->reflection_touch_detection_enabled())
				{
				case KRAZY_BOOL_True:
					switch(_vehical->on_touch_reflection(x,y))
					{
					case KRAZY_BOOL_True:return;
					}
					break;
				}
			}
			//..,
			switch(KrazyWindowController::getDefault()->on_touch(x , y))
			{
			case KRAZY_BOOL_True  : return;
			}
	}
	break;
	default : 	
		{
			switch(KrazyWindowController::getDefault()->on_touch(x , y))
			{
			case KRAZY_BOOL_True  : return;
			}
		}
		break;
	}
	
}
//....,
void KrazyWheelsRendrer::on_preferance()
{
	_CurrentScreenState = KRAZY_GAME_SCREEN_STATE_Default;
	KrazyWindowController::getDefault()->on_changeState(KRAZY_WINDOW_DISPLAY_ONLI_Preferance);
}
//....,
void KrazyWheelsRendrer::on_thrive(KrazyThriverInvokeParam& _thrive)
{
	KrazyWindowThriverParam _param(_thrive._WndTyp , _thrive._Msg);
	KrazyWindowController::getDefault()->on_changeState(_thrive._WndCtrlStateId , &_param);
	
}
//..,
void KrazyWheelsRendrer::on_levellisting()
{
	KrazyWindowController::getDefault()->on_changeState(KRAZY_WINDOW_DISPLAY_ONLI_LevelListing);
	_CurrentScreenState = KRAZY_GAME_SCREEN_STATE_Default;
}
//..,
void KrazyWheelsRendrer::on_moves()
{
	KrazyWindowController::getDefault()->on_changeState(KRAZY_WINDOW_DISPLAY_ONLI_Moves_a);
	_CurrentScreenState = KRAZY_GAME_SCREEN_STATE_Default;
}
//..,
void KrazyWheelsRendrer::on_moves_next()
{
	KrazyWindowController::getDefault()->on_changeState(KRAZY_WINDOW_DISPLAY_ONLI_Moves_b);
	_CurrentScreenState = KRAZY_GAME_SCREEN_STATE_Default;
}
//..,
void KrazyWheelsRendrer::on_story()
{
	KrazyWindowController::getDefault()->on_changeState(KRAZY_WINDOW_DISPLAY_ONLI_Story);
	_CurrentScreenState = KRAZY_GAME_SCREEN_STATE_Default;
}
//..,
void KrazyWheelsRendrer::on_pause_on()
{
	KrazyWindowController::getDefault()->on_changeState(KRAZY_WINDOW_DISPLAY_ONLI_Pause);
	_CurrentScreenState = KRAZY_GAME_SCREEN_STATE_WaitOn;
}
//..,
void KrazyWheelsRendrer::on_touched(KrazyTextArea* _area)
{
	string _text = _area->get_Text();
}
//
void KrazyWheelsRendrer::onPlaySameLevelAgain()
{
	cleanUp();_GameRenderingPolicy->reset();
#ifdef _KrazyTutorials
	on_set_tutorial_mode_if_there();
#endif
	_CurrentScreenState = KRAZY_GAME_SCREEN_STATE_InitLevel;
}
//..
void KrazyWheelsRendrer::onPause()
{
	on_pause_on();
}
//..
void KrazyWheelsRendrer::onResume()
{
	_CurrentScreenState = KRAZY_GAME_SCREEN_STATE_GameOn;
	on_init_tool();
}
 //..,
void KrazyWheelsRendrer::onMainMenu_from_Level()
{
	cleanUp();_GameRenderingPolicy->reset();
	on_preferance();
}
 //..,
#ifdef _ENABLE_FIX_THRIVER
void KrazyWheelsRendrer::onPlayNextLevel()
{
	if(++_CurrentLevel > _KRAZY_MAX_LEVEL){
		_CurrentLevel = 1;
	}
	//..,
	cleanUp();_GameRenderingPolicy->reset();
	_CurrentScreenState = KRAZY_GAME_SCREEN_STATE_InitLevel;
	on_init();
}
#else
void KrazyWheelsRendrer::onPlayNextLevel()
{
	if(++_CurrentLevel > _KRAZY_MAX_LEVEL){
		_CurrentLevel = 1;
	}
	//..,
	cleanUp();_GameRenderingPolicy->reset();
#ifdef _KrazyTutorials
	on_set_tutorial_mode_if_there();
#endif
	_CurrentScreenState = KRAZY_GAME_SCREEN_STATE_InitLevel;
}
#endif
//
void KrazyWheelsRendrer::onBringLevels()
{
}
//
void KrazyWheelsRendrer::onBringLevelScreen()
{
	on_levellisting();
}
//
void KrazyWheelsRendrer::onBringSettings()
{
}
//
void KrazyWheelsRendrer::onBringMoves()
{
	on_moves();
}
//
void KrazyWheelsRendrer::onBringStory()
{
	on_story();
}
//
//#define _Enable_Test_Level
#ifdef _ENABLE_FIX_THRIVER
void KrazyWheelsRendrer::onPlayLevel(int _index)
{
#ifdef _Enable_Test_Level
	_CurrentLevel = 0;
	KrazyWindowController::getDefault()->on_changeState(KRAZY_WINDOW_DISPLAY_None);
	_CurrentScreenState = KRAZY_GAME_SCREEN_STATE_InitLevel;
#else
	_CurrentLevel = _index;
	_CurrentScreenState = KRAZY_GAME_SCREEN_STATE_InitLevel;
	on_init();
#endif
}
#else
void KrazyWheelsRendrer::onPlayLevel(int _index)
{
#ifdef _Enable_Test_Level
	_CurrentLevel = 0;
	KrazyWindowController::getDefault()->on_changeState(KRAZY_WINDOW_DISPLAY_None);
	_CurrentScreenState = KRAZY_GAME_SCREEN_STATE_InitLevel;
#else
	_CurrentLevel = _index;
#ifdef _KrazyTutorials
	on_set_tutorial_mode_if_there();
#endif
	_CurrentScreenState = KRAZY_GAME_SCREEN_STATE_InitLevel;
#endif
}
#endif
//..,
void KrazyWheelsRendrer::onSkipStory()
{
	on_preferance();
}
//..,
void KrazyWheelsRendrer::onMoveNext()
{
	on_moves_next();
}
//..,
void KrazyWheelsRendrer::on_clogged(__KrazyVehicalList& _clogged_vehicals)
{
	clean_clogilist();
	int _count = _clogged_vehicals.size();
	for(int i = 0; i < _count; i++){
		_CloggiThriverList.push_back(new __KrazyCloggiiThriver(_clogged_vehicals[i]));
	}
	//..,
	_CurrentThriver = 0;
	_CurrentScreenState = KRAZY_GAME_SCREEN_STATE_CloggedOn;
}
//....,
const unsigned int  _MAX_BLINK      = 10;
const unsigned int  _MAX_BLINK_ANIM = 10;
//....,
void KrazyWheelsRendrer::on_clogged_on()
{
	KrazyShadedVehicalRendrer* _rendrer = _GameRenderingPolicy->get_vehical_rendrer();
	KrazyVehicalCache* ptr_cache = KrazyVehicalCache::getCache(); // make the list directly retrieved..,
	int vehical_count = ptr_cache->get_VehicalsInTheNetworkCount();
	for(int i = 0; i < vehical_count; i++){
		KrazyVehical* _vehical = ptr_cache->get_VehicalFromTheNetwork(i);
		int _thrivers = _CloggiThriverList.size();
		if(_CurrentThriver < _thrivers){
			__KrazyCloggiiThriver* _thriver = _CloggiThriverList[_CurrentThriver];
			if(_vehical == _thriver->_Vehical){
				switch(_thriver->_State)
				{
				case KRAZY_CLOGGI_DISPLAY_STATE_Display  : 
					if(++_thriver->_Wait == _MAX_BLINK){
						_thriver->_Wait  = 0;
						_thriver->_MaxDisplayCount++;
						_thriver->_State = KRAZY_CLOGGI_DISPLAY_STATE_Hide;
					}
					else{
						_rendrer->render(_thriver->_Vehical);
					}
					continue;
				case KRAZY_CLOGGI_DISPLAY_STATE_Hide     : 
					if(_thriver->_MaxHideCount == _MAX_BLINK_ANIM && _thriver->_MaxDisplayCount == _MAX_BLINK_ANIM){
						_thriver->_State = KRAZY_CLOGGI_DISPLAY_STATE_Over;
					}
					else if(++_thriver->_Wait == _MAX_BLINK){
						_thriver->_Wait  = 0;
						_thriver->_MaxHideCount++;
						_thriver->_State = KRAZY_CLOGGI_DISPLAY_STATE_Display;
					}
					continue; 
				case KRAZY_CLOGGI_DISPLAY_STATE_Over     :  _rendrer->render(_vehical);	_CurrentThriver++;  continue;
				}
			}
			else{
				_rendrer->render(_vehical);
			}
		}
		else{
			string _msg = string("Clogged Vehicals !");
			KrazyLevelThriver::get_default()->level_mode() = KRAZY_LEVELMODE_gameover;
			KrazyLevelThriver::get_default()->help_tip()   = _msg;
			_CurrentScreenState = KRAZY_GAME_SCREEN_STATE_WaitOn; 
			KrazyThriverInvokeParam _thrive( _msg ,
			KRAZY_LEVEL_THRIVER_WND_TYPE_GameOver , 
			KRAZY_WINDOW_DISPLAY_LevelTriver_and_PlayTool);
			on_thrive(_thrive);
		}
	}
	
}
//..,
void KrazyWheelsRendrer::onMute()
{
#ifndef _WINDOWS
	KrazyAudioThriver::getDefault()->on_stop();
	
#else
	KrazyBackgroundTrack::getDefault()->stop();
#endif
}
//..,
void KrazyWheelsRendrer::onUnMute()
{
#ifndef _WINDOWS
	KrazyAudioThriver::getDefault()->on_play();
#else
	KrazyBackgroundTrack::getDefault()->play(BACKGROUND_TRACK);
#endif
}
//..,
void KrazyWheelsRendrer::onGameOn()
{
	_CurrentScreenState = KRAZY_GAME_SCREEN_STATE_GameOn;
	on_init_tool();
}
//..,
void KrazyWheelsRendrer::on_set_tutorial_mode_if_there()
{
	int _count = _TutorialLevels.size();
	bool _is_tutorial(false);
	for(int i = 0; i < _count; i++){
		if( _CurrentLevel == _TutorialLevels[i]){
			_is_tutorial = true;
		}
	}
	if(_is_tutorial){
		KrazyTutorialThriver::get_default()->mode() = KRAZY_BOOL_True;
	}
	else{
		KrazyTutorialThriver::get_default()->mode() = KRAZY_BOOL_False;
	}
}
//..,