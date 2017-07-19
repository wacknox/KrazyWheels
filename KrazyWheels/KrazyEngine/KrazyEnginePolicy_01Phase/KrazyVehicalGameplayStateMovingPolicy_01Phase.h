#pragma once
#include "KrazyDefaultAlgoMovingVehical_01Phase.h"
#include "KrazyVehical.h"
#include "KrazyClogDetector.h"

#define ON_EXECUTE_DEFAULT_GAMEPLAYSTATE() \
	switch(KrazyLevelThriver::get_default()->onExecute(_vehical)) \
	{ \
	case KRAZY_BOOL_False:return; \
	} \
	switch(_vehical->touched()) \
	{ \
	case KRAZY_BOOL_True: \
         onExecuteTouch(_vehical); \
		 _vehical->touched() = KRAZY_BOOL_False;\
		 break; \
	} \
	switch(_vehical->reflection_touched()) \
	{ \
	case KRAZY_BOOL_True: \
		 onExecuteReflectionTouch(_vehical); \
		 _vehical->reflection_touched() = KRAZY_BOOL_False; \
		 break; \
	} \
switch(_vehical->gameplay_movingstate()) \
	{ \
	case KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Moving_Busted : switch(onExecuteGamePlayMovingState_Busted(_vehical)){case KRAZY_BOOL_False:return;}	 break; \
	case KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_at_Block:switch(onExecuteGamePlayMovingState_Waiting_at_Block(_vehical)){case KRAZY_BOOL_False:return;} break; \
	case KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_at_NoEntry:switch(onExecuteGamePlayMovingState_Waiting_at_NoEntry(_vehical)){case KRAZY_BOOL_False:return;} break; \
	case KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_after_Touch:switch(onExecuteGamePlayMovingState_Waiting_after_Touch(_vehical)){case KRAZY_BOOL_False:return;} break; \
	case KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Moving_after_Violation:switch(onExecuteGamePlayMovingState_Moving_after_Violation(_vehical)){case KRAZY_BOOL_False:return;} break; \
	case KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_after_TurnsOver:switch(onExecuteGamePlayMovingState_Waiting_after_TurnisOver(_vehical)){case KRAZY_BOOL_False:return;} break; \
	case KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Moving_after_Touch:switch(onExecuteGamePlayMovingState_Moving_after_Touch(_vehical)){case KRAZY_BOOL_False:return;} break; \
	case KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_Before_Exit:switch(onExecuteGamePlayMovingState_Waiting_Before_Exit(_vehical)){case KRAZY_BOOL_False:return;} break; \
	case KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_Before_OnSpotFlip:switch(onExecuteGamePlayMovingState_Waiting_Before_OnSpotFlip(_vehical)){case KRAZY_BOOL_False:return;} break; \
	case KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_for_OnSpotFlip:switch(onExecuteGamePlayMovingState_Waiting_for_OnSpotFlip(_vehical)){case KRAZY_BOOL_False:return;} break; \
	case KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_After_OnSpotFlip:switch(onExecuteGamePlayMovingState_Waiting_After_OnSpotFlip(_vehical)){case KRAZY_BOOL_False:return;} break; \
	} \
	if(!KrazyDefaultAlgoMovingVehical_01Phase::matchDistance(_vehical)){ \
		onExecutePostMatchDistance(_vehical); \
		return;\
	}\
	KrazyClogDetector::getDefault()->clear(_vehical);\
	onExecuteDefaultMove(_vehical);\


#define ON_CREATE_ROUTE_BYVEHICAL(_vehical)  \
		createRouteMap(_vehical); \
		_vehical->route_mode() = KRAZY_ROUTEMODE_ByVehical; \
		_vehical->on_setNextTurnIndex(0); \

#define ON_CREATE_GAMEPLAYROUTE(_vehical) \
	      create_gameplay_route(_vehical); \
		   _vehical->gameplay_turns() = 0; \
		   _vehical->route_mode() = KRAZY_ROUTEMODE_Gameplay \
		   
#define ON_CREATE_EXITROUTE(_vehical) \
	    create_exit_route(_vehical); \

#define ON_CREATE_ROUTE_for_GAMPLAY_VEHICAL(_vehical) \
	    createRouteMap(_vehical); \
		_vehical->route_mode() = KRAZY_ROUTEMODE_Normal; \
		_vehical->on_setNextTurnIndex(0); \

#define ON_CLEAR_ROUTE_MAP(_vehical) \
	     _vehical->get_TrafficIndicatorMap()->clear(); 	\
		 _vehical->on_setNextTurnIndex(0); \
		
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class KrazyVehicalGameplayStateMovingPolicy_01Phase :
	public KrazyDefaultAlgoMovingVehical_01Phase
{
public:
	void onExecute(KrazyVehical* vehical , KrazyALGODiagnostic** diagnostic);
	void reset();
	KrazyVehicalGameplayStateMovingPolicy_01Phase(void);
	virtual ~KrazyVehicalGameplayStateMovingPolicy_01Phase(void);
protected:

	void onExecutePostMatchDistance(KrazyVehical* ptr_vehical);
	void onExecuteDefaultMove(KrazyVehical* ptr_vehical);

	KrazyBool onExecuteTouch(KrazyVehical* _vehical);
	KrazyBool onExecuteReflectionTouch(KrazyVehical* _vehical);
	//
	KrazyBool onExecuteTouch_for_NEntryCrossing(KrazyVehical* _vehical);
	KrazyBool onExecuteTouch_for_Exiting(KrazyVehical* _vehical);
	KrazyBool onExecuteCrossingBlock(KrazyVehical* _vehical);
	//
	KrazyBool onExecuteGamePlayMovingState_Busted(KrazyVehical* ptr_vehical);
	KrazyBool onExecuteGamePlayMovingState_Waiting_at_Block(KrazyVehical* ptr_vehical);
	KrazyBool onExecuteGamePlayMovingState_Waiting_at_NoEntry(KrazyVehical* ptr_vehical);
	KrazyBool onExecuteGamePlayMovingState_Waiting_after_Touch(KrazyVehical* ptr_vehical);
	KrazyBool onExecuteGamePlayMovingState_Moving_after_Violation(KrazyVehical* ptr_vehical);
	KrazyBool onExecuteGamePlayMovingState_Waiting_after_TurnisOver(KrazyVehical* ptr_vehical);
	KrazyBool onExecuteGamePlayMovingState_Moving_after_Touch(KrazyVehical* ptr_vehical);
	KrazyBool onExecuteGamePlayMovingState_Waiting_Before_Exit(KrazyVehical* ptr_vehical);
	KrazyBool onExecuteGamePlayMovingState_Waiting_Before_OnSpotFlip(KrazyVehical* ptr_vehical);
	KrazyBool onExecuteGamePlayMovingState_Waiting_for_OnSpotFlip(KrazyVehical* ptr_vehical);
	KrazyBool onExecuteGamePlayMovingState_Waiting_After_OnSpotFlip(KrazyVehical* ptr_vehical);
	//	
public:
	void create_gameplay_route(KrazyVehical* _vehical);
	void create_exit_route(KrazyVehical* _vehical);
	void adjust_blocks_in_the_reckoning(KrazyVehical* _vehical);
	void _update_waitat_blocks_after_touch_state(KrazyVehical* _vehical , KrazyBool _skip_first_indicator = KRAZY_BOOL_False);
	void on_move_if_not_intrupted(KrazyVehical* _vehical);
};

