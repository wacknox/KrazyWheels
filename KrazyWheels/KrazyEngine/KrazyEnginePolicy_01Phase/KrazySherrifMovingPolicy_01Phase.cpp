#include "StdAfx.h"
#include "KrazySherrifMovingPolicy_01Phase.h"
#include "KrazyLaneNode.h"
#include "KrazyLevelCityNetwork.h"
#include "KrazyLevelThriver.h"
#include "KrazyUNTanglers.h"
#include "KrazyAudioThriver.h"
#include "KrazyClogDetector.h"

#define _KRAZY_TESTING_SHERRIF_MOVES
#ifdef  _KRAZY_TESTING_SHERRIF_MOVES
#define _Wait_to_Simulate_Crime   1000
#endif

#define _KRAZY_VIOLATOR_BUSTED(_violated_vehical)  \
	_violated_vehical->gameplay_movingstate() = KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Moving_Busted; \
	char _msg[1024];sprintf(_msg , "%s Busted!" , _violated_vehical->to_string().c_str()); \
	KrazyLevelThriver::get_default()->help_tip() = string(_msg); \
	KrazyLevelThriver::get_default()->level_mode() = KRAZY_LEVELMODE_gameover; \

//
KrazySherrifMovingPolicy_01Phase::KrazySherrifMovingPolicy_01Phase(void)
{
}
//
KrazySherrifMovingPolicy_01Phase::~KrazySherrifMovingPolicy_01Phase(void)
{
}
//
#define _WAIT_FOR_CRIME_SIMULATED		2000
#define _SHERRIF_WAIT_OVER_				10000
#define _SHERRIF_WAIT_GAME_OVER_		100
//
void KrazySherrifMovingPolicy_01Phase::onExecute(KrazyVehical* ptr_vehical , KrazyALGODiagnostic** diagnostic)
{
	switch(ptr_vehical->sherrif_state())
	{
	case KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Waiting  : switch(onExecuteMoveState_in_Waiting(ptr_vehical)){ case KRAZY_BOOL_False:return; } break;
	case KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Move_to_SherrifBay : switch(onExecuteMoveState_in_to_SherrifBay(ptr_vehical)){ case KRAZY_BOOL_False:return; } break;
	case KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Move_to_NabViolator_in_OppositeLane : switch(onExecuteMoveState_NabViolator_in_OppositeLane(ptr_vehical)){ case KRAZY_BOOL_False:return; } break;
	case KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Chasing : switch(onExecuteMoveState_in_Chasing(ptr_vehical)){ case KRAZY_BOOL_False:return; } break;
#if 0
	case KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Going_to_SherrifBay:switch(onExecuteMoveState_in_Going_to_SherrifBay(ptr_vehical)){case KRAZY_BOOL_False:return; } break;
#endif
	}
//.....................................traffic violation chase / nabbing / moving to base....................................................................................................................
	if(!KrazyDefaultAlgoMovingVehical_01Phase::matchDistance(ptr_vehical)){
		onExecutePostMatchDistance(ptr_vehical);
		return;
	}
	//..,
	ptr_vehical->wait_time_for_sherrif() = 0;
	KrazyClogDetector::getDefault()->clear(ptr_vehical);
	//..,
	switch(ptr_vehical->sherrif_state())
	{
		case KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_DetectPath :	switch(onExecuteMoveState_in_DetectPath(ptr_vehical)){ case KRAZY_BOOL_False:return; } break;
		case KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_DetectSherrifBay : switch(onExecuteMoveState_in_DetectSherrifBay(ptr_vehical)){ case KRAZY_BOOL_False:return; } break;
	}
	//
	onExecuteDefaultMove(ptr_vehical);
}
//
void KrazySherrifMovingPolicy_01Phase::reset()
{
}
//
KrazyBool KrazySherrifMovingPolicy_01Phase::_check_opposite_lane_violator(KrazyVehical* ptr_vehical)
{
	KrazyViolatorRegister* _regs = ptr_vehical->get_violator_register();
	KrazyVehical* _violated_vehical = _regs->get_violatedVehical();
	KrazyLaneNode* _current_assigned_lane = ptr_vehical->PTRCurrentAssignedSingleLane();
	KrazyTrafficIndicatorList* _route_map = ptr_vehical->get_TrafficIndicatorMap();
	//
#if 1
	KrazyVehicalStateInSiblingLanes _vehical_state = KRAZY_VEHICAL_STATE_IN_SIBLINLANES_Default;
	switch(_check_opposite_lane_vehical(ptr_vehical , _violated_vehical , _vehical_state))
	{
	case KRAZY_BOOL_True:
		 switch(_vehical_state)
		 {
		 case KRAZY_VEHICAL_STATE_IN_SIBLINLANES_approaching_from_opposite:
#ifdef _KRAZY_ALGO_TRACE_ENBALED
			printf("\n sherrif waiting to nab the violator in the opposite lane."); 
#endif
			_route_map->clear();
			ptr_vehical->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Move_to_NabViolator_in_OppositeLane;
			_route_map->push_back(_current_assigned_lane->getFlipTrafficIndicator());
			ptr_vehical->on_setNextTurnIndex(_route_map->size() - 1);
			return KRAZY_BOOL_True;
			//.....
		case KRAZY_VEHICAL_STATE_IN_SIBLINLANES_passing_from_opposite:
#ifdef _KRAZY_ALGO_TRACE_ENBALED
			printf("\n caught %s while passing from opposite lane fantastic job sherrif" , _violated_vehical->to_string().c_str());
#endif
			_regs->on_clear();
			_KRAZY_VIOLATOR_BUSTED(_violated_vehical);
			ptr_vehical->on_setNextTurnIndex(0);
			ptr_vehical->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_DetectSherrifBay;
			return KRAZY_BOOL_True;
			//..,
		case KRAZY_VEHICAL_STATE_IN_SIBLINLANES_passed_from_opposite : 
			ptr_vehical->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Passed_in_OppositeLane;
			return KRAZY_BOOL_True;
		 }
		 break;
	
	}
#else
	if(!_current_assigned_lane) 
		return KRAZY_BOOL_False;

	if(!_destination_lane)
		return KRAZY_BOOL_False;
	
	if(_current_assigned_lane != _destination_lane && _current_assigned_lane->getParent() == _destination_lane->getParent()){
		//vehical can be caught.., check the vehical in the opposite lane..,
		switch(_detect_target_vehical_in_oppositelane(ptr_vehical , _violated_vehical , _destination_lane))
		{
		case KRAZY_VEHICAL_STATE_IN_SIBLINLANES_approaching_from_opposite:
#ifdef _KRAZY_ALGO_TRACE_ENBALED
			printf("\n sherrif waiting to nab the violator in the opposite lane."); 
#endif
			_route_map->clear();
			ptr_vehical->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Move_to_NabViolator_in_OppositeLane;
			_route_map->push_back(_current_assigned_lane->getFlipTrafficIndicator());
			ptr_vehical->on_setNextTurnIndex(_route_map->size() - 1);
			return KRAZY_BOOL_True;
			//.....
		case KRAZY_VEHICAL_STATE_IN_SIBLINLANES_passing_from_opposite:
#ifdef _KRAZY_ALGO_TRACE_ENBALED
			printf("\n caught %s while passing from opposite lane fantastic job sherrif" , _violated_vehical->to_string().c_str());
#endif
			_regs->on_clear();
			_KRAZY_VIOLATOR_BUSTED(_violated_vehical);
			ptr_vehical->on_setNextTurnIndex(0);
			ptr_vehical->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_DetectSherrifBay;
			return KRAZY_BOOL_True;
		}
	}
#endif
	return KRAZY_BOOL_False;
}
//
void KrazySherrifMovingPolicy_01Phase::onExecutePostMatchDistance(KrazyVehical* ptr_vehical)
{
	KrazyViolatorRegister* _regs = ptr_vehical->get_violator_register();
	KrazyVehical* _violated_vehical = _regs->get_violatedVehical();
	KrazyVehical* _waiting_for = KrazyClogDetector::getDefault()->waiting_for(ptr_vehical);
	if(_waiting_for && _waiting_for == _violated_vehical){
		_regs->on_clear();
		_KRAZY_VIOLATOR_BUSTED(_violated_vehical);
		ptr_vehical->on_setNextTurnIndex(0);
		ptr_vehical->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_DetectSherrifBay;
		return;
	}
	//..,
	if(ptr_vehical->wait_time_for_sherrif()++ == _SHERRIF_WAIT_GAME_OVER_)
	{
		ptr_vehical->wait_time_for_sherrif() = 0;
		KrazyLevelThriver::get_default()->level_mode() = KRAZY_LEVELMODE_gameover;
		char _msg[1024];
		sprintf(_msg , "Sherrif waited for too long!");
		KrazyLevelThriver::get_default()->help_tip() = string(_msg);
	}
	//..,
	switch(ptr_vehical->sherrif_state())
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if 1   // .., this is for more strictness.............................................
	case KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Chasing:
		{
			
			switch(ptr_vehical->waitstate())
			{
			case KRAZY_VEHICAL_WAIT_STATE_Turned: 
				switch(_check_opposite_lane_violator(ptr_vehical)){
				case KRAZY_BOOL_True:return;
				}break;
			}
			KrazyVehical* _front_vehical = ptr_vehical->PTRFrontVehical();
			if(ptr_vehical->get_NextTurn() == 0 && _violated_vehical == _front_vehical)
			{
				// caught .., 
#ifdef _KRAZY_ALGO_TRACE_ENBALED
				printf("\n caught vehical %s " , _front_vehical->to_string().c_str());
#endif
				_regs->on_clear();
				_KRAZY_VIOLATOR_BUSTED(_front_vehical);
				ptr_vehical->on_setNextTurnIndex(0);
				ptr_vehical->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_DetectSherrifBay;
			}

		}
		break;
#endif
	case KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Going_to_SherrifBay:
		{
			switch(ptr_vehical->waitstate())
			{
			case KRAZY_VEHICAL_WAIT_STATE_Turned: 
				{
					switch(ptr_vehical->reached_slider_point())
					{
					case KRAZY_BOOL_True:
						{
							ptr_vehical->reached_slider_point() = KRAZY_BOOL_False;
							ptr_vehical->on_setDestinationLane(0);
							ptr_vehical->on_setNextTurnIndex(0);
							ptr_vehical->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Move_to_SherrifBay;
						}
						break;
					}
					
				}
				break;
			}
			//
		}
		break;
		//
	case KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_DetectPath:
		 {
			  switch(ptr_vehical->flip_mode())
			 {
			 case KRAZY_VEHICAL_FLIP_MODE_Spot:
				 {
#ifdef _KRAZY_ALGO_TRACE_ENBALED
			printf("\n sherrif on the lane!");
#endif				
					 ptr_vehical->was_in_sherrifBay() = KRAZY_BOOL_False;
				 }
				 break;
			 }
		 }
		 break;
	case KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Move_to_NabViolator_in_OppositeLane:
		{
			switch(ptr_vehical->flip_mode())
			{
				case KRAZY_VEHICAL_FLIP_MODE_Normal	: 
					 ptr_vehical->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Waiting;
					 break;
			}
		}
		 break;
	case KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Passed_in_OppositeLane :
		{
			switch(ptr_vehical->waitstate())
			{
				case KRAZY_VEHICAL_WAIT_STATE_Turned : 
					 ptr_vehical->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Chasing;
					 break;
			}
		}
		break;
	}
}
//
void KrazySherrifMovingPolicy_01Phase::onExecuteDefaultMove(KrazyVehical* ptr_vehical)
{
	KrazyTrafficIndicator* ptr_last_indc_met = 0;
	switch(getFinishingTrafficIndicator(ptr_vehical , &ptr_last_indc_met)){
	case KRAZY_BOOL_False:
		ptr_vehical->on_move();
		return;
	}
	//
	if(ptr_last_indc_met && !(ptr_vehical->getNextTurnIndex() < 0)){
		ptr_vehical->waitstate() = KRAZY_VEHICAL_WAIT_STATE_Turning;
		ptr_vehical->on_laneChange(ptr_last_indc_met);	
	}
}
//
void  KrazySherrifMovingPolicy_01Phase::_reset_sherrif_on_the_bay(KrazyVehical* ptr_vehical)
{
#ifdef _KRAZY_ALGO_TRACE_ENBALED
	printf("\n resting sherrif on the bay!");
#endif	
	KrazyTrafficIndicator* _sherrif_bay = ptr_vehical->get_sherrifBay();
	KrazyLaneNode* _assigned_lane = ptr_vehical->PTRCurrentAssignedSingleLane();
	ptr_vehical->on_attachFrontVehical(0);
	ptr_vehical->on_attachRearVehical(0);
	_assigned_lane->on_detachVehical(ptr_vehical);
	*ptr_vehical->Vector() = *_sherrif_bay->Vector();
	onExecuteMoveState_in_to_SherrifBay(ptr_vehical);
}
//////////////////////////////////////////////////// Executing Moving State ////////////////////////////////////////////////////////////////////////////
KrazyBool KrazySherrifMovingPolicy_01Phase::onExecuteMoveState_in_Waiting(KrazyVehical* ptr_vehical)
{
	ptr_vehical->reached_slider_point() = KRAZY_BOOL_False;
	KrazyViolatorRegister* _violators = ptr_vehical->get_violator_register();
#if 0
	switch(ptr_vehical->sherrif_was_waiting_for_violator_in_front()){
	case KRAZY_BOOL_True:
		if(ptr_vehical->wait_time_for_sherrif()++ == _SHERRIF_WAIT_OVER_)
		{
			ptr_vehical->wait_time_for_sherrif() = 0;
			ptr_vehical->sherrif_was_waiting_for_violator_in_front() = KRAZY_BOOL_False;
			KrazyViolatorRegister* _regs = ptr_vehical->get_violator_register();
			KrazyVehical* _violated_vehical = _regs->get_violatedVehical();
#ifdef _KRAZY_ALGO_TRACE_ENBALED
			printf("\n wait succesful %s caught thanx sherrif for your preservearance " , _violated_vehical->to_string().c_str());
#endif
			_regs->on_clear();
			_KRAZY_VIOLATOR_BUSTED(_violated_vehical);
			ptr_vehical->on_setNextTurnIndex(0);
			ptr_vehical->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_DetectSherrifBay;
		}
		else
		{
			if(!(_violators->get_violatedVehicals() > 0))
				return KRAZY_BOOL_False;

			KrazyVehical* _violated_vehical = _violators->get_violatedVehical();
			KrazyVehical* _front_vehical = ptr_vehical->PTRFrontVehical();
			if(_front_vehical && _front_vehical == _violated_vehical){
#ifdef _KRAZY_ALGO_TRACE_ENBALED
				printf("\n  %s caught thanx sherrif for your wit" , _violated_vehical->to_string().c_str());
#endif
				ptr_vehical->sherrif_was_waiting_for_violator_in_front() = KRAZY_BOOL_False;
				_violators->on_clear();
				_KRAZY_VIOLATOR_BUSTED(_violated_vehical);
				KrazyLevelThriver::get_default()->level_mode() = KRAZY_LEVELMODE_gameover;
				ptr_vehical->on_setNextTurnIndex(0);
				ptr_vehical->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_DetectSherrifBay;
			}
			else{
				return KRAZY_BOOL_False;
			}
		}
		break;
	}
#endif
	//
	if(_violators->get_violatedVehicals() > 0){
		ptr_vehical->on_setNextTurnIndex(0);
		ptr_vehical->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_DetectPath;
		switch(ptr_vehical->was_in_sherrifBay()){
		case KRAZY_BOOL_True:
			{
				KrazyLaneNode* _lane = ptr_vehical->get_sherrifBay()->PTRParentLaneNode();
				ptr_vehical->on_setOnSpotFlipIndicator(_lane->createOnSpotFlipIndicator());
				flip_on_spot(ptr_vehical , _lane);
#if 0
				KrazyAudioThriver::getDefault()->on_changeMusic(1);
#endif
				//..,,
#ifdef _KRAZY_ALGO_TRACE_ENBALED
				printf("\n sherrif waiting for empty lane");
#endif
			}
			break;
		case KRAZY_BOOL_False:
			{
#ifdef _KRAZY_ALGO_TRACE_ENBALED
				printf("\n sherrif's move pre-empted");
#endif
			}
			break;
		}
	}
	else{
		return KRAZY_BOOL_False;
	}
	return KRAZY_BOOL_True;
}
//
KrazyBool KrazySherrifMovingPolicy_01Phase::onExecuteMoveState_NabViolator_in_OppositeLane(KrazyVehical* ptr_vehical)
{
    //
	KrazyViolatorRegister* _regs = ptr_vehical->get_violator_register();
	KrazyVehical* _violated_vehical = _regs->get_violatedVehical();
	KrazyLaneNode* _lane = _violated_vehical->PTRCurrentAssignedSingleLane();
	KrazyLaneNode* _sherrif_lane = ptr_vehical->PTRCurrentAssignedSingleLane();
	//
	KrazyLaneDirection _lane_direction = _lane->getProperties()->get_LaneDirection();
		glm::vec2* _vec_flip_indicator = _lane->getFlipTrafficIndicator()->Vector();
		KrazyBool _violator_nabbed = KRAZY_BOOL_False;
		switch(_lane_direction)
		{
		case KRAZY_LANE_DIRECTION_UP_HORZS    : 
		case KRAZY_LANE_DIRECTION_DOWN_HORZS  :
			if(!(glm::distance(
				glm::vec2(ptr_vehical->VectorFrontBumper()->x , _vec_flip_indicator->y) , 
				glm::vec2(_violated_vehical->VectorFrontBumper()->x , _vec_flip_indicator->y)) > /*0.1f*/ ptr_vehical->gap())){
					_violator_nabbed = KRAZY_BOOL_True;
			}
			break;
		case KRAZY_LANE_DIRECTION_UP_VERTS	  :  
		case KRAZY_LANE_DIRECTION_DOWN_VERTS  : 
			if(!(glm::distance(
				glm::vec2(_vec_flip_indicator->x , ptr_vehical->VectorFrontBumper()->y) , 
				glm::vec2(_vec_flip_indicator->x , _violated_vehical->VectorFrontBumper()->y)) > /*0.1f*/ptr_vehical->gap())){
					_violator_nabbed = KRAZY_BOOL_True;
			}
			break;
		}
		//
		switch(_violator_nabbed)
		{
		case KRAZY_BOOL_True:
#ifdef _KRAZY_ALGO_TRACE_ENBALED
		 printf("\n caught vehical from opposite lane bravo sherrif! %s " , _violated_vehical->to_string().c_str());
#endif
			_regs->on_clear();
			_KRAZY_VIOLATOR_BUSTED(_violated_vehical);
			ptr_vehical->on_setNextTurnIndex(0);
			ptr_vehical->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_DetectSherrifBay;
			break;
		}
	return KRAZY_BOOL_True;
}
//
KrazyBool KrazySherrifMovingPolicy_01Phase::onExecuteMoveState_in_to_SherrifBay(KrazyVehical* ptr_vehical)
{
	 ptr_vehical->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Waiting;
	 ptr_vehical->get_TrafficIndicatorMap()->clear();
	 ptr_vehical->was_in_sherrifBay() = KRAZY_BOOL_True;
	 ptr_vehical->reached_slider_point() = KRAZY_BOOL_False;
#ifdef _KRAZY_ALGO_TRACE_ENBALED
	 printf("\n sherrif reached the bay");
#endif
	 return KRAZY_BOOL_True;
}
//
KrazyBool KrazySherrifMovingPolicy_01Phase::onExecuteMoveState_in_Chasing(KrazyVehical* ptr_vehical)
{	
	switch(_check_opposite_lane_violator(ptr_vehical))
	{ 
	case KRAZY_BOOL_True:return KRAZY_BOOL_False; 
	}
	//..,,
	KrazyViolatorRegister* _regs = ptr_vehical->get_violator_register();
	KrazyVehical* _violated_vehical = _regs->get_violatedVehical();
	//..,
	switch(_violator_in_rear(ptr_vehical , _violated_vehical))
	{
	case KRAZY_BOOL_True :
		_regs->on_clear();
		_KRAZY_VIOLATOR_BUSTED(_violated_vehical);
		ptr_vehical->on_setNextTurnIndex(0);
		ptr_vehical->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_DetectSherrifBay;
		return KRAZY_BOOL_False;
	}
	return KRAZY_BOOL_True;
}
//
KrazyBool KrazySherrifMovingPolicy_01Phase::onExecuteMoveState_in_DetectPath(KrazyVehical* ptr_vehical)
{
#ifdef _KRAZY_ALGO_TRACE_ENBALED
	
	switch(ptr_vehical->waitstate())
	{
	case KRAZY_VEHICAL_WAIT_STATE_Default:
	case KRAZY_VEHICAL_WAIT_STATE_Turned:
		  break;
	default:
		printf("\n wrong wait state for sherrif while detecting path %d" , ptr_vehical->waitstate());
		ptr_vehical->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Waiting;
		return KRAZY_BOOL_False;
	}
#endif
	KrazyTrafficIndicatorList* ptr_car_route = ptr_vehical->get_TrafficIndicatorMap();
	if(!ptr_car_route)
		return KRAZY_BOOL_False;
	//
	KrazyViolatorRegister* _regs = ptr_vehical->get_violator_register();
	KrazyVehical* _violated_vehical = _regs->get_violatedVehical();
	//
	if(ptr_vehical->PTRFrontVehical() == _violated_vehical)
	{
		ptr_car_route->clear();
		ptr_car_route->push_back(ptr_vehical->PTRCurrentAssignedSingleLane()->getFlipTrafficIndicator());
		ptr_vehical->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Chasing;
		ptr_vehical->on_setNextTurnIndex(ptr_car_route->size() - 1);
		return KRAZY_BOOL_False;
	}
	//
#if 1
	switch(_violator_in_rear(ptr_vehical , _violated_vehical))
	{
	case KRAZY_BOOL_True :
		_regs->on_clear();
		_KRAZY_VIOLATOR_BUSTED(_violated_vehical);
		ptr_vehical->on_setNextTurnIndex(0);
		ptr_vehical->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_DetectSherrifBay;
		return KRAZY_BOOL_False;
	}
#else
	KrazyVehical* _rear_vehical = ptr_vehical->PTRRearVehical();
	for(;_rear_vehical;){
		if(_rear_vehical == _violated_vehical){
#ifdef _KRAZY_ALGO_TRACE_ENBALED
			printf("\n  %s straight in the hand of sherrif" , _violated_vehical->to_string().c_str());
#endif
			_regs->on_clear();
			_KRAZY_VIOLATOR_BUSTED(_violated_vehical);
			ptr_vehical->on_setNextTurnIndex(0);
			ptr_vehical->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_DetectSherrifBay;
			return KRAZY_BOOL_False;
		}
		_rear_vehical = _rear_vehical->PTRRearVehical();
	}
#endif
#ifdef _KRAZY_ALGO_TRACE_ENBALED
	printf("\n detecting path to nab %s " , _violated_vehical->to_string().c_str());
#endif
	KrazyLaneNode* ptr_source_singlelane = ptr_vehical->PTRCurrentAssignedSingleLane();
	KrazyLaneNode* ptr_destination_singlelane = _violated_vehical->PTRCurrentAssignedSingleLane();
	//check if sherrif and the violated car is on the same lane after the violated vehical take new turns
	if(ptr_source_singlelane->getParent() == ptr_destination_singlelane->getParent()){
		if(ptr_source_singlelane == ptr_destination_singlelane){
			switch(ptr_vehical->sherrif_was_waiting_for_violator_in_front())
			{
			case KRAZY_BOOL_True:
				ptr_car_route->clear();
				ptr_car_route->push_back(ptr_source_singlelane->getFlipTrafficIndicator());
				ptr_vehical->on_setNextTurnIndex(ptr_car_route->size() - 1);
				ptr_vehical->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Waiting;
				return KRAZY_BOOL_False;
			}
		}
		else{
			ptr_car_route->clear();
			ptr_car_route->push_back(ptr_destination_singlelane->getFlipTrafficIndicator());
			ptr_car_route->push_back(ptr_source_singlelane->getFlipTrafficIndicator());
			ptr_vehical->on_setNextTurnIndex(ptr_car_route->size() - 1);
			ptr_vehical->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Chasing;
			return KRAZY_BOOL_False;
		}
	}
#if 0
	/*
	check if .., sherrif should wait for the violated vehical in the same assigned current lane..,
	till the violator comes in front..,
	*/
	KrazyTrafficIndicatorList* _indicator_list = ptr_destination_singlelane->get_TrafficIndicators();
	int indicators = _indicator_list->size();
	KrazyLaneNode* _parent_destination = ptr_destination_singlelane->getParent();
	KrazyLaneNodeList* _lanes = _parent_destination->getConnectedLanes();
	if(indicators ==  2 && _lanes->size() == 1){
		/*
		since flip indicators are mandatory to every lane , however if a lane have 
		one turn indicator .., making it two  indicators in the lane..,
		*/
		 switch(_lanes->at(0)->foundSingleLane(ptr_vehical->PTRCurrentAssignedSingleLane()))
		 {
		 case KRAZY_BOOL_True:
			 {
				 for(int indicator = 0; indicator < indicators; indicator++){
					 KrazyTrafficIndicator* _indicator = _indicator_list->at(indicator);
					 switch(_indicator->Type())
					 {
					 case KRAZY_TRAFFIC_INDC_Turn:
						 {
							 KrazyTrafficIndicator* _dest_indicator = _indicator->get_DestinationIndc(0);
							 if(ptr_source_singlelane == _dest_indicator->PTRParentLaneNode()){
								 KrazyBool _sherrif_should_wait = KRAZY_BOOL_False;
								 switch(ptr_source_singlelane->getProperties()->get_LaneDirection())
								 {
								 case KRAZY_LANE_DIRECTION_UP_HORZS :
									 _sherrif_should_wait = (ptr_vehical->VectorFrontBumper()->x > _dest_indicator->Vector()->x) ? KRAZY_BOOL_True : KRAZY_BOOL_False;
									 break;
								 case KRAZY_LANE_DIRECTION_DOWN_HORZS  :
									 _sherrif_should_wait = (ptr_vehical->VectorFrontBumper()->x < _dest_indicator->Vector()->x) ? KRAZY_BOOL_True : KRAZY_BOOL_False;
									 break;

								 case KRAZY_LANE_DIRECTION_UP_VERTS	  : 
									 _sherrif_should_wait = (ptr_vehical->VectorFrontBumper()->y < _dest_indicator->Vector()->y) ? KRAZY_BOOL_True : KRAZY_BOOL_False;
									 break;
								 case KRAZY_LANE_DIRECTION_DOWN_VERTS  : 
									 _sherrif_should_wait = (ptr_vehical->VectorFrontBumper()->y > _dest_indicator->Vector()->y) ? KRAZY_BOOL_True : KRAZY_BOOL_False;
									 break;
								 }
								 switch(_sherrif_should_wait)
								 {
								 case KRAZY_BOOL_True:
									 ptr_car_route->clear();
									 ptr_car_route->push_back(ptr_source_singlelane->getFlipTrafficIndicator());
									 ptr_vehical->on_setNextTurnIndex(ptr_car_route->size() - 1);
									 ptr_vehical->sherrif_was_waiting_for_violator_in_front() = KRAZY_BOOL_True;
									 ptr_vehical->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Waiting;
#ifdef _KRAZY_ALGO_TRACE_ENBALED
									 printf("\n sherrif in waiting for %s to nab from front" , _violated_vehical->to_string().c_str());
#endif
									 return KRAZY_BOOL_False;
								 }
							 }
						 }
						 break;
					 }
				 }
			 }
			 break;
		 }
	}
#endif
	//
	ptr_destination_singlelane->setDestinationEnd(ptr_destination_singlelane->getFlipTrafficIndicator());
	ptr_vehical->on_setDestinationLane(ptr_destination_singlelane);
	_VecSearchOrigin = *ptr_vehical->VectorFrontBumper();
	//
#ifdef _KRAZY_ALGO_TRACE_ENBALED
	getLaneIndex(ptr_source_singlelane);
	printf("\n detecting path to nab %s from search origin [x = %f , y = %f]" , _violated_vehical->to_string().c_str() , _VecSearchOrigin.x , _VecSearchOrigin.y);
#endif
	//
	switch(KrazyUNTanglers::_lost_track(ptr_vehical))
	{
	case KRAZY_BOOL_True :  _KRAZY_VIOLATOR_BUSTED(_violated_vehical); return KRAZY_BOOL_False; break;
	}
	//
	get_route_for_vehical(ptr_vehical , ptr_source_singlelane , ptr_destination_singlelane , ptr_car_route);
	//
	ptr_destination_singlelane->setSearchOrigin(0);
	ptr_source_singlelane->setDestinationEnd(0);
	if(ptr_car_route->size() > 0){
#ifdef _KRAZY_ALGO_TRACE_ENBALED
		if(ptr_car_route->at(ptr_car_route->size() - 1)->PTRParentLaneNode() != ptr_source_singlelane){
			printf("\n sorry sherrif you are confused in nabbing %s " , _violated_vehical->to_string().c_str());
			ptr_vehical->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Waiting;
			return KRAZY_BOOL_False;
		}
#endif
		ptr_vehical->on_setNextTurnIndex(ptr_car_route->size() - 1);
	}
	else
	{
#ifdef _KRAZY_ALGO_TRACE_ENBALED
		printf("\n sorry sherrif you lost %s " , _violated_vehical->to_string().c_str());
		ptr_vehical->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Waiting;
		return KRAZY_BOOL_False;
#endif
	}
	ptr_vehical->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Chasing;
#ifdef _KRAZY_ALGO_TRACE_ENBALED
	printf("\n chase on for %s " , _violated_vehical->to_string().c_str());
#endif
	return KRAZY_BOOL_True;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
KrazyBool KrazySherrifMovingPolicy_01Phase::onExecuteMoveState_in_DetectSherrifBay(KrazyVehical* ptr_vehical)
{
///////////////////////////////////////	
#ifdef _KRAZY_ALGO_TRACE_ENBALED
	
	switch(ptr_vehical->waitstate())
	{
	case KRAZY_VEHICAL_WAIT_STATE_Default:
	case KRAZY_VEHICAL_WAIT_STATE_Turned:
		  break;
	default:
		printf("\n wrong wait state for sherrif while detecting path %d" , ptr_vehical->waitstate());
		ptr_vehical->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Waiting;
		return KRAZY_BOOL_False;
	}
#endif
////////////////////
	KrazyTrafficIndicatorList* ptr_car_route = ptr_vehical->get_TrafficIndicatorMap();
	if(!ptr_car_route)
		return KRAZY_BOOL_False;

	KrazyTrafficIndicator* _sherrif_bay = ptr_vehical->get_sherrifBay();
	KrazyLaneNode* _destination_lane = _sherrif_bay->PTRParentLaneNode();
	KrazyLaneNode* _assigned_lane = ptr_vehical->PTRCurrentAssignedSingleLane();
	KrazyTrafficIndicator* _sliding_point = _destination_lane->get_slider_point();
	//Make Sure sliding_point vectors to be intialized properly before use;
	_sliding_point->on_set_bay(*_sherrif_bay->Vector() , _sherrif_bay);  
	if(_assigned_lane->getParent() == _destination_lane->getParent()){
		if(_assigned_lane == _destination_lane){
			KrazyVehical* _vehical_front = ptr_vehical->PTRFrontVehical();
			KrazyVehical* _vehical_rear = ptr_vehical->PTRRearVehical();
			
			if(_vehical_front)
				_vehical_front->on_attachRearVehical(_vehical_rear);

			if(_vehical_rear)
				_vehical_rear->on_attachFrontVehical(_vehical_front);
#ifdef _KRAZY_ALGO_TRACE_ENBALED
			printf("\n sherrif bay is on the destination lane");
#endif
			_reset_sherrif_on_the_bay(ptr_vehical);
		}
		else{
#ifdef _KRAZY_ALGO_TRACE_ENBALED
			printf("\n sherrif bay is on the opposite lane of destination lane");
#endif
			ptr_car_route->clear();
			ptr_car_route->push_back(_sliding_point);
			ptr_car_route->push_back(_assigned_lane->getFlipTrafficIndicator());
			ptr_vehical->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Going_to_SherrifBay;
			ptr_vehical->on_setNextTurnIndex(ptr_car_route->size() - 1);

		}
		return KRAZY_BOOL_False;
	}
	//
#ifdef _KRAZY_ALGO_TRACE_ENBALED
	printf("\n detecting sherrif bay");
#endif
	//
	KrazyLaneNode* _source_lane = ptr_vehical->PTRCurrentAssignedSingleLane();
	_destination_lane->setDestinationEnd(_sliding_point);
	ptr_vehical->on_setDestinationLane(_destination_lane);
	//
#ifdef _KRAZY_ALGO_TRACE_ENBALED
	getLaneIndex(_source_lane);
#endif
	//
	get_route_for_vehical(ptr_vehical , _source_lane , _destination_lane , ptr_car_route);
	_source_lane->setSearchOrigin(0);
	_destination_lane->setDestinationEnd(0);
	if(ptr_car_route->size() > 0){
		ptr_vehical->on_setNextTurnIndex(ptr_car_route->size() - 1);
	}
	else
	{
#ifdef _KRAZY_ALGO_TRACE_ENBALED
		printf("\n Path not found for sherrif bay");
#endif
		ptr_vehical->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Waiting;
		return KRAZY_BOOL_False;

	}
	ptr_vehical->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Going_to_SherrifBay;
#ifdef _KRAZY_ALGO_TRACE_ENBALED
	printf("\n going to sherrif bay");
#endif
	return KRAZY_BOOL_True;
}
//////
KrazyBool KrazySherrifMovingPolicy_01Phase::onExecuteMoveState_in_Going_to_SherrifBay(KrazyVehical* ptr_vehical)
{
	KrazyTrafficIndicatorList* ptr_car_route = ptr_vehical->get_TrafficIndicatorMap();
	int count = ptr_car_route->size();
	if(!(count > 0))
		return KRAZY_BOOL_False;
	//
	KrazyLaneNode* _destination_lane = ptr_car_route->at(0)->PTRParentLaneNode();
	KrazyLaneNode* _assigned_lane    = ptr_vehical->PTRCurrentAssignedSingleLane();
	//
	if(_assigned_lane == _destination_lane)
	{
		ptr_vehical->on_setDestinationLane(0);
		ptr_vehical->on_setNextTurnIndex(0);
		_reset_sherrif_on_the_bay(ptr_vehical);
#ifdef _KRAZY_ALGO_TRACE_ENBALED
		printf("\n sherrif going to park");
#endif	
		return KRAZY_BOOL_False;
	}
	//
	return KRAZY_BOOL_True;
}
////
KrazyBool KrazySherrifMovingPolicy_01Phase::_violator_in_rear(KrazyVehical* ptr_vehical , KrazyVehical* _violated_vehical)
{
	KrazyVehical* _rear_vehical = ptr_vehical->PTRRearVehical();
	for(;_rear_vehical;){
		if(_rear_vehical == _violated_vehical){
			return KRAZY_BOOL_True;
		}
		_rear_vehical = _rear_vehical->PTRRearVehical();
	}
	return KRAZY_BOOL_False;
}