#include "StdAfx.h"
#include "KrazyBangToona.h"
#include "KrazyLevelThriver.h"
#include "KrazyTrafficIndicator.h"
#include "KrazyRouteThriver.h"
//#define _ENABLE_TRACE_ON_FOR_BANGOO
//..,
KrazyBangToona::KrazyBangToona(void)
{
}
//
KrazyBangToona::~KrazyBangToona(void)
{
}
//
void KrazyBangToona::onExecute(KrazyVehical* _vehical)
{
	ON_EXECUTE_DEFAULT_GAMEPLAYSTATE();
}
//
void KrazyBangToona::reset()
{
}
//
KrazyBool KrazyBangToona::onExecuteReflectionTouch(KrazyVehical* _vehical)
{
	switch(_vehical->gameplay_movingstate())
	{
	case KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_at_Block	:  return onExecuteCrossingBlock(_vehical);
	default														:  return KrazyToonaBrothersMovingPolicy_01Phase::onExecuteReflectionTouch(_vehical);
	}
	
}
//
KrazyBool KrazyBangToona::onExecuteCrossingBlock(KrazyVehical* _vehical)
{

	KrazyTrafficIndicator* _blocking = _vehical->get_blocking_indicator();
	if(_blocking){
		switch(_blocking->is_green()){
		case KRAZY_BOOL_False : 
			{
				KrazyDutyKueue* _dutyQ = KrazyDutyKueue::getDefault();
				switch(_dutyQ->enable())
				{
				case KRAZY_BOOL_True : 
					{
						KrazyUnaBrotherDuty_for_SearchingBlocks* _duty = new KrazyUnaBrotherDuty_for_SearchingBlocks(_blocking);
						_dutyQ->appendNewDuty(_duty);
					}
					break;
				}
				//..,
				_blocking->swap_lights();
			}
			break;
		}
	}
	//..,
	_vehical->gameplay_movingstate() = KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_Before_OnSpotFlip; 
	return KRAZY_BOOL_True;
}
//..,
