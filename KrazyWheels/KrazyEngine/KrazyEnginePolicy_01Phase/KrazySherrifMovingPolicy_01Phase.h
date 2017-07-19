#pragma once
#include "krazydefaultalgomovingvehical_01phase.h"
#include "KrazyVehical.h"
//
class KrazySherrifMovingPolicy_01Phase :
	public KrazyDefaultAlgoMovingVehical_01Phase
{
public:
	KrazySherrifMovingPolicy_01Phase(void);
	virtual ~KrazySherrifMovingPolicy_01Phase(void);

	void onExecute(KrazyVehical* vehical , KrazyALGODiagnostic** diagnostic);
	void reset();

protected:
	KrazyBool _violator_in_rear(KrazyVehical* ptr_vehical , KrazyVehical* _violated_vehical);
	KrazyBool _check_opposite_lane_violator(KrazyVehical* ptr_vehical);
	KrazyVehicalStateInSiblingLanes _check_vehical_state_in_opposite_lane(KrazyVehical* ptr_vehical);
	void  _reset_sherrif_on_the_bay(KrazyVehical* ptr_vehical);

	void onExecutePostMatchDistance(KrazyVehical* ptr_vehical);
	void onExecuteDefaultMove(KrazyVehical* ptr_vehical);
	// executing moving state ....,
	KrazyBool onExecuteMoveState_in_Waiting(KrazyVehical* ptr_vehical);
	KrazyBool onExecuteMoveState_in_to_SherrifBay(KrazyVehical* ptr_vehical);
	KrazyBool onExecuteMoveState_NabViolator_in_OppositeLane(KrazyVehical* ptr_vehical);
	KrazyBool onExecuteMoveState_in_Chasing(KrazyVehical* ptr_vehical);
	KrazyBool onExecuteMoveState_in_DetectPath(KrazyVehical* ptr_vehical);
	KrazyBool onExecuteMoveState_in_DetectSherrifBay(KrazyVehical* ptr_vehical);
	KrazyBool onExecuteMoveState_in_Going_to_SherrifBay(KrazyVehical* ptr_vehical);
	// ...............................................................,
};

