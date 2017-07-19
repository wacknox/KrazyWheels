#include "StdAfx.h"
#include "KrazyUnaaFamily.h"
#include "KrazyLevelThriver.h"
//
KrazyUnaaFamilyMovingPolicy_01Phase::KrazyUnaaFamilyMovingPolicy_01Phase(void)
{
}
//
KrazyUnaaFamilyMovingPolicy_01Phase::~KrazyUnaaFamilyMovingPolicy_01Phase(void)
{
}
void KrazyUnaaFamilyMovingPolicy_01Phase::onExecute(KrazyVehical* _vehical , KrazyALGODiagnostic** diagnostic)
{
	ON_EXECUTE_DEFAULT_GAMEPLAYSTATE();
	*diagnostic = &_AlgoDiagnostic;
}
//
void KrazyUnaaFamilyMovingPolicy_01Phase::reset()
{
}
//
void KrazyUnaaFamilyMovingPolicy_01Phase::onExecutePostMatchDistance(KrazyVehical* _vehical)
{
	KrazyVehicalGameplayStateMovingPolicy_01Phase::onExecutePostMatchDistance(_vehical);
}
//
void KrazyUnaaFamilyMovingPolicy_01Phase::onExecuteDefaultMove(KrazyVehical* _vehical)
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
		ON_CREATE_ROUTE_BYVEHICAL(_vehical);
	}
	//
	KrazyVehicalGameplayStateMovingPolicy_01Phase::onExecuteDefaultMove(_vehical);
}
//
KrazyBool KrazyUnaaFamilyMovingPolicy_01Phase::onExecuteGamePlayMovingState_Busted(KrazyVehical* _vehical)
{
	KrazyBool _busted = KrazyVehicalGameplayStateMovingPolicy_01Phase::onExecuteGamePlayMovingState_Busted(_vehical);
	return _busted;
}
//
KrazyBool KrazyUnaaFamilyMovingPolicy_01Phase::onExecuteGamePlayMovingState_Waiting_at_Block(KrazyVehical* _vehical)
{
	return KrazyVehicalGameplayStateMovingPolicy_01Phase::onExecuteGamePlayMovingState_Waiting_at_Block(_vehical);
}
//
KrazyBool KrazyUnaaFamilyMovingPolicy_01Phase::onExecuteGamePlayMovingState_Waiting_at_NoEntry(KrazyVehical* _vehical)
{
	return KrazyVehicalGameplayStateMovingPolicy_01Phase::onExecuteGamePlayMovingState_Waiting_at_NoEntry(_vehical);
}
//
KrazyBool KrazyUnaaFamilyMovingPolicy_01Phase::onExecuteGamePlayMovingState_Waiting_after_Touch(KrazyVehical* _vehical)
{
	return KRAZY_BOOL_False;
}
//
KrazyBool KrazyUnaaFamilyMovingPolicy_01Phase::onExecuteGamePlayMovingState_Moving_after_Violation(KrazyVehical* ptr_vehical)
{	
	return KRAZY_BOOL_True;
}
//................................................
