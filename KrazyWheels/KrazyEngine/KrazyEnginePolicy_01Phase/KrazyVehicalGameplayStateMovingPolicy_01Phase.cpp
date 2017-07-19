#include "StdAfx.h"
#include "KrazyVehicalGameplayStateMovingPolicy_01Phase.h"
#include "KrazyLaneNode.h"
#include "KrazyUNTanglers.h"
#include "KrazyLevelThriver.h"
#include "KrazyDutyKueue.h"
#include "KrazyRouteThriver.h"
//
KrazyVehicalGameplayStateMovingPolicy_01Phase::KrazyVehicalGameplayStateMovingPolicy_01Phase(void)
{
}
//
KrazyVehicalGameplayStateMovingPolicy_01Phase::~KrazyVehicalGameplayStateMovingPolicy_01Phase(void)
{
}
//
void KrazyVehicalGameplayStateMovingPolicy_01Phase::onExecute(KrazyVehical* _vehical , KrazyALGODiagnostic** diagnostic)
{
	ON_EXECUTE_DEFAULT_GAMEPLAYSTATE();
	*diagnostic = &_AlgoDiagnostic;
}
//
void KrazyVehicalGameplayStateMovingPolicy_01Phase::reset()
{
}
//
void KrazyVehicalGameplayStateMovingPolicy_01Phase::create_gameplay_route(KrazyVehical* _vehical)
{
	/*
	 this algorthim will work with connectd lanes .., if u want to test it with disconnected lanes..,
	 u can go to hell;
	*/
	unsigned int _turn_limit = _vehical->gameplay_turn_limit();
	ON_CREATE_ROUTE_for_GAMPLAY_VEHICAL(_vehical);
	KrazyTrafficIndicatorList* _car_route = _vehical->get_TrafficIndicatorMap();
	int _next_index = _vehical->getNextTurnIndex();
	//.........
	KrazyTrafficIndicator* _indicator_aux = 0;
	KrazyTrafficIndicator* _lane_next_indicator = _car_route->at(_next_index);
	switch(_lane_next_indicator->Type())
	{
	case KRAZY_TRAFFIC_INDC_NoEntry : 
	case KRAZY_TRAFFIC_INDC_Wait	: 
		_indicator_aux = _lane_next_indicator;
		 break;
	}
	//.....
	if(_next_index + 1 < _car_route->size())
		_next_index = _next_index + 1;
	//.....	
	_lane_next_indicator = _car_route->at(_next_index);
	_car_route->clear();KrazyLaneNode* _next_lane = 0;_next_index = 0;
	if(_indicator_aux){
		_car_route->push_back(_indicator_aux);
	}
	//
	for(unsigned int turn = 0; turn < _turn_limit;){
		glm::vec2* _match_vec = 0;
		switch(_lane_next_indicator->Type()){
		case KRAZY_TRAFFIC_INDC_FlipPoint : 
			_next_lane  = _lane_next_indicator->PTRParentLaneNode()->getOppositeSingleLane();
			_next_index = 0;
			break;
		case KRAZY_TRAFFIC_INDC_Turn      : 
			{
				KrazyTrafficIndicator* _destination_indc = _lane_next_indicator->get_DestinationIndc(0);
				_next_lane = _destination_indc->PTRParentLaneNode();
				_match_vec = _destination_indc->Vector();
				turn++;	_next_index = 0;
			}
			break;
		default:
			_next_lane = _lane_next_indicator->PTRParentLaneNode();
			_next_index++;
			break;
	 	}
		_car_route->push_back(_lane_next_indicator);
		if(_next_lane){
			KrazyTrafficIndicatorList* _indicators = _next_lane->get_TrafficIndicators();
			if(_match_vec){
				int indicators = _indicators->size();
				bool _found_indicator = false;
				for(int i = 0; i < indicators && !_found_indicator; i++){
					KrazyTrafficIndicator* _indicator = _indicators->at(i);
					switch(_next_lane->getProperties()->get_LaneDirection())
					{
					case KRAZY_LANE_DIRECTION_UP_HORZS  :if(_match_vec->x > _indicator->Vector()->x){_lane_next_indicator = _indicator;_found_indicator = true;continue;} break;
					case KRAZY_LANE_DIRECTION_DOWN_HORZS:if(_match_vec->x < _indicator->Vector()->x){_lane_next_indicator = _indicator;_found_indicator = true;continue;} break;
					case KRAZY_LANE_DIRECTION_UP_VERTS  :if(_match_vec->y < _indicator->Vector()->y){_lane_next_indicator = _indicator;_found_indicator = true;continue;} break;
					case KRAZY_LANE_DIRECTION_DOWN_VERTS:if(_match_vec->y > _indicator->Vector()->y){_lane_next_indicator = _indicator;_found_indicator = true;continue;} break;
					}
				}
			}
			else{
				_lane_next_indicator = _indicators->at(_next_index);
			}
		}
	}
	//
	_vehical->on_setNextTurnIndex(0);
}
//
void KrazyVehicalGameplayStateMovingPolicy_01Phase::create_exit_route(KrazyVehical* _vehical)
{
	KrazyTrafficIndicator* _exit = _vehical->get_exit();
	KrazyLaneNode* _source_lane =  _vehical->PTRCurrentAssignedSingleLane();
	KrazyLaneNode* _destination_lane = _exit->PTRParentLaneNode();
	KrazyTrafficIndicatorList* _car_route = _vehical->get_TrafficIndicatorMap();
	_car_route->clear();_destination_lane->setDestinationEnd(_exit);
	get_route_for_vehical(_vehical , _source_lane , _destination_lane , _car_route);
	_destination_lane->setDestinationEnd(0);
	adjust_blocks_in_the_reckoning(_vehical);
	//
	_vehical->route_mode() = KRAZY_ROUTEMODE_Reckoning;
	int _routes = _car_route->size();
	if(_routes > 0){
		_vehical->on_setNextTurnIndex(_routes - 1);
	}
	else{
		printf("\n something wrong with creating exit route !");
	}
}
//
void KrazyVehicalGameplayStateMovingPolicy_01Phase::adjust_blocks_in_the_reckoning(KrazyVehical* _vehical)
{
	// Filling the route .., with block and N Entries
	KrazyTrafficIndicatorList* _route = _vehical->get_TrafficIndicatorMap();
	KrazyTrafficIndicatorList _aux_route;
	KrazyLaneNode* _assigned_lane_for_vehical = _vehical->PTRCurrentAssignedSingleLane();
	int _count = _route->size();
	for(int i = _count - 1; i > -1; i--){
		KrazyTrafficIndicator* _indc = _route->at(i);
		KrazyLaneNode* _indc_parent_lane = _indc->PTRParentLaneNode();
		if(_indc_parent_lane == _assigned_lane_for_vehical){
			_aux_route.push_back(_indc);
		}
		else{
			KrazyTrafficIndicatorList* _indc_list = _indc_parent_lane->get_TrafficIndicators();
			int _list_count = _indc_list->size();
			for(int k = 0; k < _list_count; k++){
				KrazyTrafficIndicator* _indc_from_list = _indc_list->at(k);
				switch(_indc_from_list->Type()){
				case KRAZY_TRAFFIC_INDC_NoEntry	:
				case KRAZY_TRAFFIC_INDC_Wait	:
					_aux_route.push_back(_indc_from_list); 			 
					break;
				}
				if(_indc_from_list == _indc){
					_aux_route.push_back(_indc_from_list);
					break;
				}
			}
		}
	}
	//..,
	_route->clear();_count = _aux_route.size();
	for( int i = _count - 1; i > -1; i--){
		_route->push_back(_aux_route[i]);
	}
	//..,
}
///////////////////////////////////////////////////////// Executing Gameplay State /////////////////////////////////////////////////////////////////////////////
KrazyBool KrazyVehicalGameplayStateMovingPolicy_01Phase::onExecuteGamePlayMovingState_Busted(KrazyVehical* ptr_vehical)
{
	if(ptr_vehical->wait()++ == ptr_vehical->required_wait()){
		ptr_vehical->wait() = 0;
		ptr_vehical->gameplay_movingstate() = KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Default;
		ptr_vehical->in_violation() = KRAZY_BOOL_False;
#ifdef _KRAZY_ALGO_TRACE_ENBALED
		printf("\n %s reseted to default" , ptr_vehical->to_string().c_str());
#endif
//..........................................
#if 1
		 ON_CREATE_ROUTE_BYVEHICAL(ptr_vehical);
#endif
//...................................
	KrazyLevelThriver::get_default()->level_mode() = KRAZY_LEVELMODE_gameover;
	char _msg[1024];
	sprintf(_msg , "%s Busted!" , ptr_vehical->to_string().c_str());
	KrazyLevelThriver::get_default()->help_tip() = string(_msg);
//....................................
	}
	else{
		return KRAZY_BOOL_False;
	}
	return KRAZY_BOOL_True;
}
//
KrazyBool KrazyVehicalGameplayStateMovingPolicy_01Phase::onExecuteGamePlayMovingState_Waiting_at_Block(KrazyVehical* ptr_vehical)
{
	KrazyTrafficIndicator* _indicator = ptr_vehical->get_blocking_indicator();
	if(!_indicator)
		return KRAZY_BOOL_True;
	//
	KrazyBool _is_green = _indicator->is_green();
	switch(_is_green){
		case KRAZY_BOOL_True  :  ptr_vehical->on_set_blocking_indicator(0); break;
	}
	return _is_green;
}
//
KrazyBool KrazyVehicalGameplayStateMovingPolicy_01Phase::onExecuteGamePlayMovingState_Waiting_at_NoEntry(KrazyVehical* ptr_vehical)
{
	//
	if(ptr_vehical->wait()++ == ptr_vehical->required_wait()){
		ptr_vehical->wait() = 0;
		ptr_vehical->gameplay_movingstate() = KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_Before_OnSpotFlip;
		ptr_vehical->on_set_blocking_indicator(0);
	}
	return KRAZY_BOOL_False;
}
//
//#define _KRIME_SIMULATION_AT_EVERYTOUCH
KrazyBool KrazyVehicalGameplayStateMovingPolicy_01Phase::onExecuteGamePlayMovingState_Waiting_after_Touch(KrazyVehical* _vehical)
{
#ifdef _KRIME_SIMULATION_AT_EVERYTOUCH
	switch(_vehical->touched())
	{
	case KRAZY_BOOL_False  : 
		   _vehical->gameplay_movingstate() = KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Moving_after_Violation;
		   _vehical->in_violation() = KRAZY_BOOL_True;
		   return KRAZY_BOOL_False;
	}
#else
	  ON_CREATE_GAMEPLAYROUTE(_vehical);
	 _vehical->gameplay_movingstate() = KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Moving_after_Touch;
	 _vehical->moving_after_touch() = KRAZY_BOOL_True;
#endif
	return KRAZY_BOOL_True;;
}
//
void KrazyVehicalGameplayStateMovingPolicy_01Phase::onExecutePostMatchDistance(KrazyVehical* ptr_vehical)
{
	if(ptr_vehical->get_NextTurn()){
		switch(ptr_vehical->in_violation())
		{
		case KRAZY_BOOL_True:
			switch(ptr_vehical->waitstate()){
			case KRAZY_VEHICAL_WAIT_STATE_Turned :
				{
					KrazyVehical* _sherrif = ptr_vehical->PTRCurrentAssignedSingleLane()->get_Sherrif();
					if(_sherrif){
						_sherrif->get_violator_register()->write(ptr_vehical);
					}
				}
				break;
			}
			 break;
		}
		//
		switch(ptr_vehical->moving_after_touch())
		{
			case KRAZY_BOOL_True:
			switch(ptr_vehical->waitstate()){
				case KRAZY_VEHICAL_WAIT_STATE_Turned : 
					switch(ptr_vehical->flip_mode())
					{
					case KRAZY_VEHICAL_FLIP_MODE_None:
						{
							unsigned int _turn_limit = ptr_vehical->gameplay_turn_limit();
							unsigned int _turns = ptr_vehical->gameplay_turns();_turns++;
							if(_turns == _turn_limit){
								ptr_vehical->gameplay_turns() = 0;
								ptr_vehical->gameplay_movingstate() = KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_after_TurnsOver;
								ptr_vehical->moving_after_touch() = KRAZY_BOOL_False;
							}
							else{
								 ptr_vehical->gameplay_turns() = _turns;
							}
						}
						break;
					}
					 break;
			}
			break;
		}
		switch(ptr_vehical->gameplay_movingstate()){
#if 0
		case KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Moving_after_Touch:
		//case KRAZY_BOOL_True:
			switch(ptr_vehical->waitstate()){
				case KRAZY_VEHICAL_WAIT_STATE_Turned : 
					switch(ptr_vehical->flip_mode())
					{
					case KRAZY_VEHICAL_FLIP_MODE_None:
						{
							unsigned int _turn_limit = ptr_vehical->gameplay_turn_limit();
							unsigned int _turns = ptr_vehical->gameplay_turns();_turns++;
							if(_turns == _turn_limit){
								ptr_vehical->gameplay_turns() = 0;
								ptr_vehical->gameplay_movingstate() = KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_after_TurnsOver;
								ptr_vehical->moving_after_touch() = KRAZY_BOOL_False;
							}
							else{
								 ptr_vehical->gameplay_turns() = _turns;
							}
						}
						break;
					}
					 break;
			}
			break;
#endif
			//
		case KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_for_OnSpotFlip:
			 switch(ptr_vehical->waitstate()){
			 case KRAZY_VEHICAL_WAIT_STATE_Turned :
				{
					ptr_vehical->gameplay_movingstate() = KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_After_OnSpotFlip;
				}
				break;
			}
			 break;
		}
	}
}
//..................................................................
void KrazyVehicalGameplayStateMovingPolicy_01Phase::on_move_if_not_intrupted(KrazyVehical* _vehical)
{

}
//...............................................................
void KrazyVehicalGameplayStateMovingPolicy_01Phase::onExecuteDefaultMove(KrazyVehical* ptr_vehical)
{	
	// 
	KrazyTrafficIndicator* _traffic_indc_matched = 0;
	KrazyBool _Turn = KRAZY_BOOL_False;
	switch(ptr_vehical->route_mode()){
		case KRAZY_ROUTEMODE_Reckoning  :	_Turn = getFinishingTrafficIndicator(ptr_vehical , &_traffic_indc_matched);	break;
		case KRAZY_ROUTEMODE_ByVehical  :   _Turn = get_DetectedTrafficIndicator_in_Gameplay_ByVehical(ptr_vehical , &_traffic_indc_matched);	break;
		case KRAZY_ROUTEMODE_Normal     :
		case KRAZY_ROUTEMODE_Gameplay   :   _Turn = get_DetectedTrafficIndicatorSeq(ptr_vehical , &_traffic_indc_matched); break;
	}
	//
	switch(_Turn)
	{
		case KRAZY_BOOL_False : 
			 ptr_vehical->on_move();
			 return;
	}
	//
	if(_traffic_indc_matched){
		switch(_traffic_indc_matched->Type()){
		case KRAZY_TRAFFIC_INDC_NoEntry:
			ptr_vehical->gameplay_movingstate() = KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_at_NoEntry;
			ptr_vehical->on_set_blocking_indicator(_traffic_indc_matched);
			return;
		case KRAZY_TRAFFIC_INDC_Wait:
			{
				ptr_vehical->gameplay_movingstate() = KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_at_Block;
				_traffic_indc_matched->set_WaitingVehical(ptr_vehical); 
				_traffic_indc_matched->waiting_vehical_state() = KRAZY_VEHICAL_WAITING_Start;
				ptr_vehical->on_set_blocking_indicator(_traffic_indc_matched);
				return;
			}
			break;
		case KRAZY_TRAFFIC_INDC_Exit:
			{
				ptr_vehical->gameplay_movingstate() = KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_Before_Exit;
				return;
			}
			break;
		}
		ptr_vehical->on_turning(_traffic_indc_matched);
	}
	//.............................
}
//..................................................
KrazyBool KrazyVehicalGameplayStateMovingPolicy_01Phase::onExecuteGamePlayMovingState_Moving_after_Violation(KrazyVehical* ptr_vehical)
{
	return KRAZY_BOOL_True;
}
//
KrazyBool KrazyVehicalGameplayStateMovingPolicy_01Phase::onExecuteGamePlayMovingState_Waiting_after_TurnisOver(KrazyVehical* ptr_vehical)
{
	ON_CREATE_ROUTE_BYVEHICAL(ptr_vehical);
	ptr_vehical->gameplay_movingstate() = KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Default;
	return KRAZY_BOOL_True;
}
//
KrazyBool KrazyVehicalGameplayStateMovingPolicy_01Phase::onExecuteGamePlayMovingState_Moving_after_Touch(KrazyVehical* _vehical)
{
	return KRAZY_BOOL_True;
}
//
KrazyBool KrazyVehicalGameplayStateMovingPolicy_01Phase::onExecuteGamePlayMovingState_Waiting_Before_Exit(KrazyVehical* _vehical)
{
#ifdef _KRAZY_ALGO_TRACE_ENBALED
		printf("\n  %s exiting..," , _vehical->to_string().c_str());
#endif
//
#if 0
	ON_CREATE_ROUTE_BYVEHICAL(_vehical);
	KrazyVehical* _sherrif = _vehical->PTRCurrentAssignedSingleLane()->get_Sherrif();
	KrazyViolatorRegister* _regs = _sherrif->get_violator_register();
	_vehical->gameplay_movingstate() = KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Default;

	_vehical->in_violation() = KRAZY_BOOL_False;
	if(_regs->get_violatedVehical() == _vehical){
		_sherrif->get_violator_register()->on_clear();
		_sherrif->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_DetectSherrifBay;
    }
	//...................
	if(_Callback)
		_Callback->on_exit(_vehical);
	//...................
#endif
	//.....................................................................................................................
	return KRAZY_BOOL_False;
}
//
KrazyBool KrazyVehicalGameplayStateMovingPolicy_01Phase::onExecuteGamePlayMovingState_Waiting_Before_OnSpotFlip(KrazyVehical* ptr_vehical)
{
	KrazyLaneNode* _lane = ptr_vehical->PTRCurrentAssignedSingleLane()->getOppositeSingleLane();
	ptr_vehical->on_setOnSpotFlipIndicator(_lane->createOnSpotFlipIndicator());
	flip_on_spot(ptr_vehical, _lane);
	ON_CLEAR_ROUTE_MAP(ptr_vehical);
	ptr_vehical->gameplay_movingstate() = KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_for_OnSpotFlip;
	//
	return KRAZY_BOOL_False;
}
//
KrazyBool KrazyVehicalGameplayStateMovingPolicy_01Phase::onExecuteGamePlayMovingState_Waiting_After_OnSpotFlip(KrazyVehical* ptr_vehical)
{
	return KRAZY_BOOL_True;
}
//
KrazyBool KrazyVehicalGameplayStateMovingPolicy_01Phase::onExecuteGamePlayMovingState_Waiting_for_OnSpotFlip(KrazyVehical* ptr_vehical)
{
	return KRAZY_BOOL_True;
}
//
KrazyBool KrazyVehicalGameplayStateMovingPolicy_01Phase::onExecuteTouch(KrazyVehical* _vehical)
{
	switch(_vehical->gameplay_movingstate())
	{
		case KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_at_Block    : break;
		case KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_at_NoEntry  : return onExecuteTouch_for_NEntryCrossing(_vehical);
		case KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_after_Touch : return onExecuteGamePlayMovingState_Waiting_after_Touch(_vehical);
		case KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_Before_Exit : return onExecuteTouch_for_Exiting(_vehical);
		default:_vehical->gameplay_movingstate() = KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_after_Touch;break;
	}
	return KRAZY_BOOL_True;
}
//
KrazyBool KrazyVehicalGameplayStateMovingPolicy_01Phase::onExecuteReflectionTouch(KrazyVehical* _vehical)
{
	return KRAZY_BOOL_False;
}
//
KrazyBool KrazyVehicalGameplayStateMovingPolicy_01Phase::onExecuteTouch_for_NEntryCrossing(KrazyVehical* _vehical)
{
	 ON_CREATE_EXITROUTE(_vehical);
	_vehical->gameplay_movingstate() = KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Moving_after_Violation;
	_vehical->was_in_violation() = KRAZY_BOOL_False;
	_vehical->in_violation() = KRAZY_BOOL_True;	
	_vehical->wait() = 0;
	switch(_vehical->moving_after_touch())
	{
	case KRAZY_BOOL_True : 
		 _vehical->gameplay_turns() = 0;
		 _vehical->moving_after_touch() = KRAZY_BOOL_False;
		 break;
	}
	_vehical->on_set_blocking_indicator(0);
	return KRAZY_BOOL_True;
}
//
KrazyBool  KrazyVehicalGameplayStateMovingPolicy_01Phase::onExecuteTouch_for_Exiting(KrazyVehical* _vehical)
{
	ON_CREATE_ROUTE_BYVEHICAL(_vehical);
	KrazyTrafficIndicatorList* _route = _vehical->get_TrafficIndicatorMap();
	int inidcators = _route->size();
	bool _skipped_exit = false;
	for(int indicator = _vehical->getNextTurnIndex(); indicator < inidcators && !_skipped_exit; indicator++){
		KrazyTrafficIndicator* _indicator = _route->at(indicator);
		switch(_indicator->Type())
		{
		case KRAZY_TRAFFIC_INDC_Exit:
			_vehical->on_setNextTurnIndex(indicator + 1);
			_skipped_exit = true;
			 continue;
		}
	}
	_vehical->gameplay_movingstate() = KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Default;
	return KRAZY_BOOL_True;
}
//..,
KrazyBool  KrazyVehicalGameplayStateMovingPolicy_01Phase::onExecuteCrossingBlock(KrazyVehical* _vehical)
{
	ON_CREATE_ROUTE_BYVEHICAL(_vehical);
	_update_waitat_blocks_after_touch_state(_vehical , KRAZY_BOOL_True);
	_vehical->gameplay_movingstate() = KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Default;
	return KRAZY_BOOL_True;
}
//..,
void KrazyVehicalGameplayStateMovingPolicy_01Phase::_update_waitat_blocks_after_touch_state(KrazyVehical* _vehical  , KrazyBool _skip_first_indicator /*= KRAZY_BOOL_False*/)
{
	switch(_vehical->moving_after_touch())
	{
	case KRAZY_BOOL_True : 
		 _vehical->gameplay_turns() = 0;
		 _vehical->moving_after_touch() = KRAZY_BOOL_False;
		 break;
	}
	//..,
	switch(_skip_first_indicator)
	{
	case KRAZY_BOOL_True   :  _vehical->on_setNextTurnIndex(1); break;
	}
	//..,
	switch(_vehical->add_to_dutyQ())
	{
	case KRAZY_BOOL_True : 
		{
			KrazyDutyKueue* _dutyQ = KrazyDutyKueue::getDefault();
			//.., this seems to be dirty.., 
			KrazyUnaBrotherDuty_for_StoppingVehical* _duty = new KrazyUnaBrotherDuty_for_StoppingVehical(_vehical);
			_dutyQ->appendNewDuty(_duty);
		}
		break;
	}
	//..,
	_vehical->on_set_blocking_indicator(0);
}