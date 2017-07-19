#include "StdAfx.h"
#include "KrazyLevelThriver.h"

#define _KRAZY_LEVEL_COMPLETE() \
	    _LevelMode = KRAZY_LEVELMODE_levelcomplete; \
		return KRAZY_BOOL_False; \

#define _KRAZY_GAME_OVER() \
		_LevelMode = KRAZY_LEVELMODE_gameover; \
		return KRAZY_BOOL_False; \
//
KrazyThriving::KrazyThriving()
{
	reset();
}
//..,
KrazyThriving::~KrazyThriving()
{
}
//..,	
void KrazyThriving::reset()
{
	_WaitFurther  = KRAZY_BOOL_True;
	_WaitFurther_for_Rangatoona = KRAZY_BOOL_True;
	_ExitSequence = KRAZY_VEHICAL_in_GAMEPLAY_Nobody;
}
//..., 
KrazyBool&         KrazyThriving::wait_further()  { return _WaitFurther;  }
KrazyBool&		   KrazyThriving::wait_further_for_Rangtoona() { return _WaitFurther_for_Rangatoona; }
KrazyVehicalName&  KrazyThriving::vehical_name()  { return _ExitSequence; }
//..................................................................................................................................................................................
KrazyLevelThriver* KrazyLevelThriver::_Me = 0;
KrazyLevelThriver::KrazyLevelThriver() :
_LevelContext(0)
{
	reset();
}
//
KrazyLevelThriver::KrazyLevelThriver(KrazyLevelCityContext* _level_context) :
_LevelContext(_level_context)
{
	reset();
}
//
KrazyLevelThriver::~KrazyLevelThriver(void)
{
	clear_thrivinglist();
}
//
KrazyLevelMode&   KrazyLevelThriver::level_mode()  { return _LevelMode; }
string&			  KrazyLevelThriver::help_tip()    { return _HelpTip; }
string            KrazyLevelThriver::derrogation() { return _Derrogations; }
string&           KrazyLevelThriver::exit_sequencetext() { return _ExitSequenceText; }
//
void KrazyLevelThriver::create_with_level_context(KrazyLevelCityContext* _level_context)
{
	if(!_Me){
		_Me = new KrazyLevelThriver(_level_context);
	}
}
//
KrazyLevelThriver* KrazyLevelThriver::get_default()
{
	if(!_Me){
		_Me = new KrazyLevelThriver;
	}
	return _Me;
}
//
void KrazyLevelThriver::release_default()
{
	if(_Me){
		delete _Me;_Me = 0;
	}
}
//
void KrazyLevelThriver::reset()
{
	_Exit = 0;
	_Ganguni = 0;
	_Next_Player_to_Finish = KRAZY_VEHICAL_in_GAMEPLAY_Ganungi;
	_LastPlayer_to_Finish  = KRAZY_VEHICAL_in_GAMEPLAY_RangToona;
	_LevelMode             = KRAZY_LEVELMODE_gameon;
	_ThriverState          = KRAZY_LEVELPROGRESS_Start_to_Finish;
	_RangatoonaWaiting	   = 0;
	_IgnoreDefaultWhileRangatoonaExiting = KRAZY_BOOL_False;
	//..........................................................
	_Derrogations.empty();
	_HelpTip.empty();
	_ExitSequence.clear();
	clear_thrivinglist();
}
//
void KrazyLevelThriver::clear_thrivinglist()
{
	int _count = _ThrivingList.size();
	for(int i = 0; i < _count; i++){
		KrazyThriving* _thriving = _ThrivingList[i];
		delete _thriving;_thriving = 0;
	}
	//
	_ThrivingList.clear();
}
//
KrazyTrafficIndicator*  KrazyLevelThriver::get_exit()
{
	KrazyTrafficIndicatorList _indicators;KrazyLevelThriver::get_default()->get_context()->on_set_blockingTI(&_indicators);
	int indicators = _indicators.size();
	bool _found_exit(false);
	for(int indicator = 0; indicator < indicators && !_found_exit; indicator++){
		_Exit = _indicators[indicator];
		switch(_Exit->Type())
		{
		case KRAZY_TRAFFIC_INDC_Exit: _found_exit = true; continue; 
		}
	}
	//
	return _Exit;
}
//
void KrazyLevelThriver::on_set_ExitSequence(KrazyIntList& _exit_sequence)
{
	_KRAZY_COPY_LIST(_exit_sequence ,  _ExitSequence);
}
//
void KrazyLevelThriver::on_appendThriving(KrazyThriving* _Thriving)
{
	_ThrivingList.push_back(_Thriving);
}
//..,
void KrazyLevelThriver::on_exit_player(KrazyVehical* _vehical)
{
	KrazyVehicalName _name = _vehical->vehical_name() ;
	if(_name ==_Next_Player_to_Finish){
		switch(_name)
		{
		case KRAZY_VEHICAL_in_GAMEPLAY_Ganungi:
			{
				KrazyVehical* _rear_vehical = _vehical->PTRRearVehical();
				if(_rear_vehical && _rear_vehical->vehical_name() == KRAZY_VEHICAL_in_GAMEPLAY_BangToona){
					_Next_Player_to_Finish = KRAZY_VEHICAL_in_GAMEPLAY_RangToona;
				}
				else{
					_LevelMode = KRAZY_LEVELMODE_gameover;
				}
			}
			break;
		case KRAZY_VEHICAL_in_GAMEPLAY_RangToona:
			_LevelMode = KRAZY_LEVELMODE_levelcomplete;
			break;
		}
	}
	else
	{
		_LevelMode = KRAZY_LEVELMODE_gameover;
	}
}
//
KrazyBool KrazyLevelThriver::onExecute(KrazyVehical* _vehical)
{
	switch(_ThriverState){
	case KRAZY_LEVELPROGRESS_Start_to_Finish        : return onExecute_Start_to_Finish(_vehical);
	case KRAZY_LEVELPROGRESS_Ganguni_Exiting        : return onExecute_Ganguni_Exiting(_vehical);
	case KRAZY_LEVELPROGRESS_Gangtoona_Exiting      : return onExecute_Gangtoona_Exiting(_vehical);
	case KRAZY_LEVELPROGRESS_Waiting_for_Rangatoona : return onExecute_Waiting_for_Rangatoona(_vehical);
	}
	return KRAZY_BOOL_True;
}
//
KrazyBool KrazyLevelThriver::onExecute_Start_to_Finish(KrazyVehical* _vehical)
{
	switch(_vehical->gameplay_movingstate())
	{
	case KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_Before_Exit:
		{
			KrazyThriving* _thriving = _ThrivingList[0];
			KrazyVehicalName _vehical_name = _thriving->vehical_name();
			KrazyBool _wait_further = _thriving->wait_further();
			if(_vehical_name == _vehical->vehical_name()){
				switch(_vehical_name)
				{
				case KRAZY_VEHICAL_in_GAMEPLAY_Ganungi		:
					switch(_wait_further){
					case KRAZY_BOOL_True  :
						_ThriverState = KRAZY_LEVELPROGRESS_Ganguni_Exiting; 
						_Ganguni = _vehical;
						break;
					case KRAZY_BOOL_False : 
						 _Derrogations = "It Was Smooth !";
						 _KRAZY_LEVEL_COMPLETE();
						 break;
					}
					break;
				case KRAZY_VEHICAL_in_GAMEPLAY_BangToona	: 
					_ThriverState = KRAZY_LEVELPROGRESS_Waiting_for_Rangatoona;
					_IgnoreDefaultWhileRangatoonaExiting = KRAZY_BOOL_True;
					_HelpTip = "Was In...,,";
					break;
				case KRAZY_VEHICAL_in_GAMEPLAY_RangToona	: 
					_Derrogations = "It Was Smooth !";
					_KRAZY_LEVEL_COMPLETE();
					break;
				default  :
					_HelpTip = "Engine Bug PLease Restart";
					_KRAZY_GAME_OVER();
					break;
				}
			}
			else
			{
				char sz[256];
				sprintf(sz , "%s Will Exit First" , KrazyVehicalUtility::_convert_vehical_name(_vehical_name).c_str());
				_HelpTip = sz;
				_KRAZY_GAME_OVER();
			}
		}
	}
	//..,
	return KRAZY_BOOL_True;
	//..,
}
//
KrazyBool KrazyLevelThriver::onExecute_Ganguni_Exiting(KrazyVehical* _vehical)
{
	if(_Ganguni && _vehical == _Ganguni){
		// assuming that there will be only one exit
		KrazyTrafficIndicator* _exit = _Ganguni->get_exit();
		if(_exit){
			//check the vehical is on the same lane.., as that of exit
			KrazyTrafficIndicatorList* _route = _vehical->get_TrafficIndicatorMap();
			KrazyTrafficIndicator* _next_indicator = _route->at(_vehical->getNextTurnIndex());
			if(_next_indicator->PTRParentLaneNode() == _exit->PTRParentLaneNode() && _next_indicator->index() > _exit->index()){
				// moved across the exit.., check rear vehical;
				KrazyVehical* _rear_vehical = _Ganguni->PTRRearVehical();
				if(_rear_vehical){
					switch(_rear_vehical->vehical_name())
					{
					case KRAZY_VEHICAL_in_GAMEPLAY_BangToona:
						{
							KrazyThriving* _thriving = _ThrivingList[0];
							switch(_thriving->wait_further_for_Rangtoona())
							{
							case KRAZY_BOOL_True:
								{
									_rear_vehical =  _rear_vehical->PTRRearVehical();
									if(_rear_vehical){
										switch(_rear_vehical->vehical_name())
										{
										case KRAZY_VEHICAL_in_GAMEPLAY_RangToona : 
											_Derrogations = string("Flawless");
											_KRAZY_LEVEL_COMPLETE();
											break;
											//..,
										default									 : 
											_HelpTip = string("Rangtoona is Expected to follow Bangtoona!");
											_KRAZY_GAME_OVER();
											break;
										}
									}
									else{
										_ThriverState = KRAZY_LEVELPROGRESS_Waiting_for_Rangatoona;
									}
								}
								break;
								//..,
							case KRAZY_BOOL_False:
								{
									_Derrogations = string("It Was Smooth !");
									_KRAZY_LEVEL_COMPLETE();
								}
								break;
								//..,
							}
					   }
					    break;
					default :
						_HelpTip = string("Bangtoona was expected to follow Ganguni");
						_KRAZY_GAME_OVER();
						break;
					}
				}
				else{
					 _HelpTip = string("Bangtoona was expected to follow Ganguni");
					_KRAZY_GAME_OVER();
				}

			}
			else{
#if 0
				_HelpTip = string("Dont Worry Engine Bug!");
				_KRAZY_GAME_OVER();
#endif
			}
		}
		else{
			_HelpTip = string("Dont Worry Engine Bug!");
			_KRAZY_GAME_OVER();
		}
	}
	return KRAZY_BOOL_True;
}
//
KrazyBool KrazyLevelThriver::onExecute_Gangtoona_Exiting(KrazyVehical* _vehical)
{
	return KRAZY_BOOL_True;
}
//
#define _RANGATOONA_WAIT_LIM    500000
KrazyBool KrazyLevelThriver::onExecute_Waiting_for_Rangatoona(KrazyVehical* _vehical)
{
	if(_RangatoonaWaiting++ == _RANGATOONA_WAIT_LIM){
		_RangatoonaWaiting = 0;
		_HelpTip = string("Rangtoona missed the exit window!");
		_KRAZY_GAME_OVER();
	}
	else{
		switch(_vehical->gameplay_movingstate())
		{
		case KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_Before_Exit:
			{
				switch(_vehical->vehical_name())
				{
				case KRAZY_VEHICAL_in_GAMEPLAY_RangToona : 
					 _Derrogations = string("It Was Smooth !");
					 _KRAZY_LEVEL_COMPLETE();
				default       : 
					switch(_IgnoreDefaultWhileRangatoonaExiting)
					{
					case KRAZY_BOOL_False  :  
						{
							char _msg[1024];
							sprintf(_msg , "RangToona Was Expected To Exit! instead of %s" , _vehical->to_string().c_str());
							_HelpTip = string(_msg);
							_KRAZY_GAME_OVER();
						}
						break;
					}
					break;
				}
			}
			break;
		}
	}
	return KRAZY_BOOL_True;
}
//