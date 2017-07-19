#include "stdafx.h"
#include "KrazyToonaBrotherTutorialPolicy.h"
#include "KrazyLevelThriver.h"
#include "KrazyTutorialThriver.h"
//..,
KrazyToonaBrotherTutorialPolicy::KrazyToonaBrotherTutorialPolicy() : 
_Was_in_NEntry(KRAZY_BOOL_False)
{
}
//..,
KrazyToonaBrotherTutorialPolicy::~KrazyToonaBrotherTutorialPolicy()
{
}
//..,
void KrazyToonaBrotherTutorialPolicy::reset()
{
	_Was_in_NEntry = KRAZY_BOOL_False;
}
//..,
void KrazyToonaBrotherTutorialPolicy::onExecutePostMatchDistance(KrazyVehical* ptr_vehical)
{
	switch(ptr_vehical->waitstate())
	{
		case KRAZY_VEHICAL_WAIT_STATE_Turned : 
			KrazyTutorialThriver::get_default()->on_turned(ptr_vehical);
			break;
	}
	//..,
	KrazyVehicalGameplayStateMovingPolicy_01Phase::onExecutePostMatchDistance(ptr_vehical);
}
//..,
KrazyBool KrazyToonaBrotherTutorialPolicy::onExecuteTouch(KrazyVehical* _vehical)
{
	if(KrazyTutorialThriver::get_default()->on_touched(_vehical))
	{
		switch(_Was_in_NEntry)
		{
		case KRAZY_BOOL_True  :
			_Was_in_NEntry = KRAZY_BOOL_False;
			KrazyToonaBrothersMovingPolicy_01Phase::onExecuteTouch(_vehical);
			break;
		case KRAZY_BOOL_False : 
			 KrazyVehicalGameplayStateMovingPolicy_01Phase::onExecuteGamePlayMovingState_Waiting_after_Touch(_vehical);
			 break;
		}
	}
	//..,
	return KRAZY_BOOL_True; 
}
//..,
KrazyBool KrazyToonaBrotherTutorialPolicy::onExecuteReflectionTouch(KrazyVehical* _vehical)
{
	if(!KrazyTutorialThriver::get_default()->on_reflection_touched(_vehical)){
		return KRAZY_BOOL_True;
	}
	return KrazyToonaBrothersMovingPolicy_01Phase::onExecuteReflectionTouch(_vehical);
}
//..,
void KrazyToonaBrotherTutorialPolicy::onExecuteDefaultMove(KrazyVehical* ptr_vehical)
{
	
	switch(KrazyTutorialThriver::get_default()->all_vehical_stopped())
	{
	case KRAZY_BOOL_False : 
		if(!KrazyTutorialThriver::get_default()->should_wait(ptr_vehical)){
			KrazyToonaBrothersMovingPolicy_01Phase::onExecuteDefaultMove(ptr_vehical); 
			switch(ptr_vehical->gameplay_movingstate()) 
			{
			case KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_Before_Exit : KrazyTutorialThriver::get_default()->on_exit(ptr_vehical);break;
			}
		}
		break;
	}
	//..,
	KrazyTutorialThriver::get_default()->on_moved(ptr_vehical);
	KrazyTutorialThriver::get_default()->on_waiting_before_point(ptr_vehical);
}
//..,
KrazyBool KrazyToonaBrotherTutorialPolicy::onExecuteGamePlayMovingState_Waiting_at_NoEntry(KrazyVehical* _vehical)
{
	_Was_in_NEntry = KRAZY_BOOL_True;
	KrazyTutorialThriver::get_default()->on_point(_vehical , _vehical->get_blocking_indicator());
	return KRAZY_BOOL_False;
}
//..,
void KrazyToonaBrotherTutorialPolicy::onExecute(KrazyVehical* _vehical)
{
	ON_EXECUTE_DEFAULT_GAMEPLAYSTATE();
}
//..,