#include "stdafx.h"
#include "KrazyUnaFamilyTutorialPolicy.h"
#include "KrazyLevelThriver.h"
#include "KrazyTutorialThriver.h"
//..,
KrazyUnaaFamilyTutorialPolicy::KrazyUnaaFamilyTutorialPolicy()
{
}
//..,
KrazyUnaaFamilyTutorialPolicy::~KrazyUnaaFamilyTutorialPolicy()
{
}
//..,
void KrazyUnaaFamilyTutorialPolicy::onExecute(KrazyVehical* _vehical)
{
	ON_EXECUTE_DEFAULT_GAMEPLAYSTATE();
}
//..,
KrazyBool KrazyUnaaFamilyTutorialPolicy::onExecuteGamePlayMovingState_Waiting_at_NoEntry(KrazyVehical* ptr_vehical)
{
	return KRAZY_BOOL_False;
}
//..,
void KrazyUnaaFamilyTutorialPolicy::onExecuteDefaultMove(KrazyVehical* ptr_vehical)
{
}
//..,
KrazyBool KrazyUnaaFamilyTutorialPolicy::onExecuteTouch(KrazyVehical* _vehical)
{
	return KRAZY_BOOL_False;
}
//..,
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
KrazyGanguniMovingTutorialPolicy::KrazyGanguniMovingTutorialPolicy() :
_Was_waiting_at_NEntry(KRAZY_BOOL_False)
{
}
//..,
KrazyGanguniMovingTutorialPolicy::~KrazyGanguniMovingTutorialPolicy()
{

}
//..,
void KrazyGanguniMovingTutorialPolicy::reset()
{
	 _Was_waiting_at_NEntry = KRAZY_BOOL_False;
}
//..,
void KrazyGanguniMovingTutorialPolicy::onExecuteDefaultMove(KrazyVehical* _vehical)
{
	switch(KrazyTutorialThriver::get_default()->all_vehical_stopped())
	{
	case KRAZY_BOOL_False : 
		KrazyGanguniMovingPolicy_01Phase::onExecuteDefaultMove(_vehical); 
		switch(_vehical->gameplay_movingstate()) 
		{
			case KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_Before_Exit : KrazyTutorialThriver::get_default()->on_exit(_vehical);break;
		}
		break;
	}
	//..,
	KrazyTutorialThriver::get_default()->on_moved(_vehical);
	KrazyTutorialThriver::get_default()->on_waiting_before_point(_vehical);
	KrazyTutorialThriver::get_default()->on_waiting_after_vehicle_start(_vehical);
	//..,
}
//..,
KrazyBool KrazyGanguniMovingTutorialPolicy::onExecuteTouch(KrazyVehical* _vehical)
{
	if(KrazyTutorialThriver::get_default()->on_touched(_vehical))
	{
		switch(_Was_waiting_at_NEntry)
		{
		case KRAZY_BOOL_True  : 
			 _Was_waiting_at_NEntry = KRAZY_BOOL_False;
			 KrazyGanguniMovingPolicy_01Phase::onExecuteTouch(_vehical);
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
void KrazyGanguniMovingTutorialPolicy::onExecute(KrazyVehical* _vehical)
{
	ON_EXECUTE_DEFAULT_GAMEPLAYSTATE();
}
//..,
KrazyBool KrazyGanguniMovingTutorialPolicy::onExecuteGamePlayMovingState_Waiting_at_NoEntry(KrazyVehical* _vehical)
{
	_Was_waiting_at_NEntry = KRAZY_BOOL_True;
	KrazyTutorialThriver::get_default()->on_point(_vehical , _vehical->get_blocking_indicator());
	return KRAZY_BOOL_False;
}
//..,
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
KrazyUnaBrothersMovingTutorialPolicy::KrazyUnaBrothersMovingTutorialPolicy() : 
_Was_waiting_at_NEntry(KRAZY_BOOL_False)
{
}
//..,
KrazyUnaBrothersMovingTutorialPolicy::~KrazyUnaBrothersMovingTutorialPolicy()
{
}
//..,
//..,
void KrazyUnaBrothersMovingTutorialPolicy::reset()
{
	 _Was_waiting_at_NEntry = KRAZY_BOOL_False;
}
//..,
void KrazyUnaBrothersMovingTutorialPolicy::onExecuteDefaultMove(KrazyVehical* _vehical)
{
	switch(KrazyTutorialThriver::get_default()->all_vehical_stopped())
	{
	case KRAZY_BOOL_False : 
		KrazyUnaBrothersMovingPolicy_01Phase::onExecuteDefaultMove(_vehical); 
		switch(_vehical->gameplay_movingstate()) 
		{
			case KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_Before_Exit : KrazyTutorialThriver::get_default()->on_exit(_vehical);break;
		}
		break;
	}
	//..,
	KrazyTutorialThriver::get_default()->on_moved(_vehical);
	KrazyTutorialThriver::get_default()->on_waiting_before_point(_vehical);
	KrazyTutorialThriver::get_default()->on_waiting_after_vehicle_start(_vehical);
	//..,
}
//..,
KrazyBool KrazyUnaBrothersMovingTutorialPolicy::onExecuteTouch(KrazyVehical* _vehical)
{
	if(KrazyTutorialThriver::get_default()->on_touched(_vehical))
	{
		switch(_Was_waiting_at_NEntry)
		{
		case KRAZY_BOOL_True  : 
			 _Was_waiting_at_NEntry = KRAZY_BOOL_False;
			 KrazyUnaBrothersMovingPolicy_01Phase::onExecuteTouch(_vehical);
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
void KrazyUnaBrothersMovingTutorialPolicy::onExecute(KrazyVehical* _vehical)
{
	ON_EXECUTE_DEFAULT_GAMEPLAYSTATE();
}
//..,
KrazyBool KrazyUnaBrothersMovingTutorialPolicy::onExecuteGamePlayMovingState_Waiting_at_NoEntry(KrazyVehical* _vehical)
{
	_Was_waiting_at_NEntry = KRAZY_BOOL_True;
	KrazyTutorialThriver::get_default()->on_point(_vehical , _vehical->get_blocking_indicator());
	return KRAZY_BOOL_False;
}
//..,