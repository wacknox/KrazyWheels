#include "stdafx.h"
#include "KrazySherrifTutorialMovingPolicy.h"
#include "KrazyTutorialThriver.h"
#include "KrazyClogDetector.h"
//..,
void KrazySherrifTutorialMovingPolicy::onExecute(KrazyVehical* ptr_vehical , KrazyALGODiagnostic** diagnostic)
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
//..,
void KrazySherrifTutorialMovingPolicy::onExecutePostMatchDistance(KrazyVehical* ptr_vehical)
{
	switch(ptr_vehical->waitstate())
	{
		case KRAZY_VEHICAL_WAIT_STATE_Turned : 
			KrazyTutorialThriver::get_default()->on_turned(ptr_vehical);
			break;
	}
	//..,
	KrazySherrifMovingPolicy_01Phase::onExecutePostMatchDistance(ptr_vehical);
}
//..,
void KrazySherrifTutorialMovingPolicy::onExecuteDefaultMove(KrazyVehical* _vehical)
{
	switch(KrazyTutorialThriver::get_default()->all_vehical_stopped())
	{
	case KRAZY_BOOL_False : KrazySherrifMovingPolicy_01Phase::onExecuteDefaultMove(_vehical);break;
	}
}
//..,