#include "StdAfx.h"
#include "KrazyUnaBrothersMovingPolicy_01Phase.h"
#include "KrazyLevelThriver.h"
#include "KrazyRouteThriver.h"
#define _KRAZY_UNABROTHERS_WAITING  200
//..,
KrazyUnaBrothersMovingPolicy_01Phase::KrazyUnaBrothersMovingPolicy_01Phase(void) :
_Wait_after_Touch(0)
{
}
//..,
KrazyUnaBrothersMovingPolicy_01Phase::~KrazyUnaBrothersMovingPolicy_01Phase(void)
{
}
//..,
void KrazyUnaBrothersMovingPolicy_01Phase::onExecute(KrazyVehical* _vehical)
{
	ON_EXECUTE_DEFAULT_GAMEPLAYSTATE();
}
//..,
void KrazyUnaBrothersMovingPolicy_01Phase::reset()
{
	_Wait_after_Touch = 0;
}
//..,
#ifdef _ENABLE_WAITING_AFTER_TOUCH
KrazyBool KrazyUnaBrothersMovingPolicy_01Phase::onExecuteTouch(KrazyVehical* _vehical)
{
	switch(_vehical->gameplay_movingstate())
	{
	case KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_at_Block    : return onExecutePreBlockFliping(_vehical);
	case KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_after_Touch :
		 _Wait_after_Touch = 0;
		 return KrazyUnaaFamilyMovingPolicy_01Phase::onExecuteTouch(_vehical);
	default : 
		switch(_vehical->route_mode())
		{
			case KRAZY_ROUTEMODE_Reckoning	:	return KRAZY_BOOL_True;
			default : return KrazyUnaaFamilyMovingPolicy_01Phase::onExecuteTouch(_vehical);
		}
	}
	//..,
	return KRAZY_BOOL_True;
}
#endif
//.., 
#if 0
void KrazyUnaBrothersMovingPolicy_01Phase::onExecutePostMatchDistance(KrazyVehical* ptr_vehical)
{
	KrazyVehicalGameplayStateMovingPolicy_01Phase::onExecutePostMatchDistance(ptr_vehical);
	
}
#endif
//..,
#ifdef _ENABLE_WAITING_AFTER_TOUCH
KrazyBool KrazyUnaBrothersMovingPolicy_01Phase::onExecuteGamePlayMovingState_Waiting_after_Touch(KrazyVehical* _vehical)
{
	if(++_Wait_after_Touch == _KRAZY_UNABROTHERS_WAITING){
		_Wait_after_Touch = 0;
		return KrazyVehicalGameplayStateMovingPolicy_01Phase::onExecuteGamePlayMovingState_Waiting_after_Touch(_vehical);
	}
	//..,
	return KRAZY_BOOL_False;
}
#endif
//...,
KrazyBool KrazyUnaBrothersMovingPolicy_01Phase::onExecuteGamePlayMovingState_Waiting_at_Block(KrazyVehical* _vehical)
{
	return KRAZY_BOOL_False;
}
//..,
#ifdef _ENABLE_WAITING_AFTER_TOUCH
void KrazyUnaBrothersMovingPolicy_01Phase::onExecuteDefaultMove(KrazyVehical* _vehical)
{
	KrazyUnaBrotherRouteThriver* _thriver = (KrazyUnaBrotherRouteThriver*)_vehical->get_route_thriver();
	switch(_thriver->execute(this))
	{
	case KRAZY_BOOL_False : KrazyUnaaFamilyMovingPolicy_01Phase::onExecuteDefaultMove(_vehical);break;
	}
}
#endif
//..,
KrazyBool KrazyUnaBrothersMovingPolicy_01Phase::onExecutePreBlockFliping(KrazyVehical* _vehical)
{
	KrazyTrafficIndicator* _blocking = _vehical->get_blocking_indicator();
	if(_blocking){
		switch(_blocking->is_green()){
			case KRAZY_BOOL_True : _blocking->swap_lights(); break;
		}
		//..,
		KrazyVehicalGameplayStateMovingPolicy_01Phase::onExecuteCrossingBlock(_vehical);
		KrazyUnaBrotherRouteThriver* _thriver = (KrazyUnaBrotherRouteThriver*)_vehical->get_route_thriver();
		_thriver->reset_for_gameplay();
	}
	//..,
	return KRAZY_BOOL_True;
}
//..,