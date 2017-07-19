#include "StdAfx.h"
#include "KrazyToonaBrothersMovingPolicy_01Phase.h"
#include "KrazyLaneNode.h"
#include "KrazyUNTanglers.h"
#include "KrazyLevelThriver.h"
//
KrazyToonaBrothersMovingPolicy_01Phase::KrazyToonaBrothersMovingPolicy_01Phase(void)
{
}
//
KrazyToonaBrothersMovingPolicy_01Phase::~KrazyToonaBrothersMovingPolicy_01Phase(void)
{
}
//
void KrazyToonaBrothersMovingPolicy_01Phase::onExecute(KrazyVehical* _vehical , KrazyALGODiagnostic** diagnostic)
{
	ON_EXECUTE_DEFAULT_GAMEPLAYSTATE();
	*diagnostic = &_AlgoDiagnostic;
}
//
void KrazyToonaBrothersMovingPolicy_01Phase::reset()
{
}
//
void KrazyToonaBrothersMovingPolicy_01Phase::onExecuteDefaultMove(KrazyVehical* _vehical)
{
	switch(_vehical->disablemove()){
	case KRAZY_BOOL_True:_vehical->disablemove() = KRAZY_BOOL_False;break;
	}
	//
	switch(_vehical->visibilitystate()){
	case KRAZY_BOOL_False:_vehical->visibilitystate() = KRAZY_BOOL_True;break;
	} 
	
	KrazyTrafficIndicatorList* ptr_car_route = _vehical->get_TrafficIndicatorMap();
	if(!(ptr_car_route->size() > 0)){
		switch(_vehical->route_mode())
		{
		case KRAZY_ROUTEMODE_ByVehical : ON_CREATE_ROUTE_BYVEHICAL(_vehical);			break;
		case KRAZY_ROUTEMODE_Normal    : ON_CREATE_ROUTE_for_GAMPLAY_VEHICAL(_vehical); break;
		}
		
	}
    //
	KrazyVehicalGameplayStateMovingPolicy_01Phase::onExecuteDefaultMove(_vehical);
}
//
KrazyBool KrazyToonaBrothersMovingPolicy_01Phase::onExecuteGamePlayMovingState_Waiting_at_NoEntry(KrazyVehical* _vehical)
{	
	return KRAZY_BOOL_False;
}
//
KrazyBool KrazyToonaBrothersMovingPolicy_01Phase::onExecuteGamePlayMovingState_Waiting_after_Touch(KrazyVehical* _vehical)
{
	return KRAZY_BOOL_False;
}
//
KrazyBool KrazyToonaBrothersMovingPolicy_01Phase::onExecuteGamePlayMovingState_Moving_after_Violation(KrazyVehical* ptr_vehical)
{
	
	return KRAZY_BOOL_True;
}
//
KrazyBool KrazyToonaBrothersMovingPolicy_01Phase::onExecuteGamePlayMovingState_Waiting_after_TurnisOver(KrazyVehical* ptr_vehical)
{
	ON_CREATE_ROUTE_for_GAMPLAY_VEHICAL(ptr_vehical);
	ptr_vehical->gameplay_movingstate() = KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Default;
	return KRAZY_BOOL_True;
}
//
KrazyBool KrazyToonaBrothersMovingPolicy_01Phase::onExecuteGamePlayMovingState_Waiting_After_OnSpotFlip(KrazyVehical* ptr_vehical)
{
	onExecuteGamePlayMovingState_Waiting_after_TurnisOver(ptr_vehical);
	KrazyTrafficIndicatorList* _car_route = ptr_vehical->get_TrafficIndicatorMap();
	if(!(_car_route->size() > 0)/* || (_car_route->size() == 1 && _car_route->at(0)->Type() == KRAZY_TRAFFIC_INDC_FlipPoint)*/){
		ptr_vehical->on_turning(ptr_vehical->PTRCurrentAssignedSingleLane()->getFlipTrafficIndicator());
	}
	return KRAZY_BOOL_True;
}
//
KrazyBool KrazyToonaBrothersMovingPolicy_01Phase::onExecuteTouch(KrazyVehical* _vehical)
{
	//
	switch(_vehical->gameplay_movingstate())
	{
		case KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_at_Block    :  break;
		case KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_at_NoEntry  :  return onExecuteTouch_for_NEntryCrossing(_vehical);
		case KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_after_Touch :  return KrazyVehicalGameplayStateMovingPolicy_01Phase::onExecuteGamePlayMovingState_Waiting_after_Touch(_vehical);
		case KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_Before_Exit :  return onExecuteTouch_for_Exiting(_vehical);
		default:_vehical->gameplay_movingstate() = KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_after_Touch;break;
	}
	//
	return KRAZY_BOOL_True;
}
//
KrazyBool KrazyToonaBrothersMovingPolicy_01Phase::onExecuteReflectionTouch(KrazyVehical* _vehical)
{
	_vehical->gameplay_movingstate() = KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_Before_OnSpotFlip; 
	return KRAZY_BOOL_True;
}
