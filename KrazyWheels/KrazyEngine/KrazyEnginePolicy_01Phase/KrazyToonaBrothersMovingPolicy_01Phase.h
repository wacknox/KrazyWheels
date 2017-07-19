#pragma once
#include "krazyvehicalgameplaystatemovingpolicy_01phase.h"
class KrazyToonaBrothersMovingPolicy_01Phase :
	public KrazyVehicalGameplayStateMovingPolicy_01Phase
{
public:
	KrazyToonaBrothersMovingPolicy_01Phase(void);
	virtual ~KrazyToonaBrothersMovingPolicy_01Phase(void);
		
	void onExecute(KrazyVehical* vehical , KrazyALGODiagnostic** diagnostic);
	void reset();

protected:

	KrazyBool onExecuteTouch(KrazyVehical* _vehical);
	KrazyBool onExecuteReflectionTouch(KrazyVehical* _vehical);

	void onExecuteDefaultMove(KrazyVehical* ptr_vehical);
	//
	KrazyBool onExecuteGamePlayMovingState_Waiting_at_NoEntry(KrazyVehical* _vehical);
	KrazyBool onExecuteGamePlayMovingState_Waiting_after_Touch(KrazyVehical* _vehical);
	KrazyBool onExecuteGamePlayMovingState_Moving_after_Violation(KrazyVehical* ptr_vehical);
	KrazyBool onExecuteGamePlayMovingState_Waiting_after_TurnisOver(KrazyVehical* ptr_vehical);
	KrazyBool onExecuteGamePlayMovingState_Waiting_After_OnSpotFlip(KrazyVehical* ptr_vehical);
	//..,
	KrazyBool onExecuteCrossingBlock(KrazyVehical* _vehical);

};

