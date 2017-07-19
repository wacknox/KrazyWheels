#pragma once
#include "KrazyVehicalGameplayStateMovingPolicy_01Phase.h"
class KrazyUnaaFamilyMovingPolicy_01Phase :
	public KrazyVehicalGameplayStateMovingPolicy_01Phase
{
public:
	KrazyUnaaFamilyMovingPolicy_01Phase(void);
	virtual ~KrazyUnaaFamilyMovingPolicy_01Phase(void);

	void onExecute(KrazyVehical* vehical , KrazyALGODiagnostic** diagnostic);
	void reset();
protected:
	void onExecutePostMatchDistance(KrazyVehical* ptr_vehical);
	void onExecuteDefaultMove(KrazyVehical* ptr_vehical);
	//
	KrazyBool onExecuteGamePlayMovingState_Busted(KrazyVehical* ptr_vehical);
	KrazyBool onExecuteGamePlayMovingState_Waiting_at_Block(KrazyVehical* ptr_vehical);
	KrazyBool onExecuteGamePlayMovingState_Waiting_at_NoEntry(KrazyVehical* ptr_vehical);
	KrazyBool onExecuteGamePlayMovingState_Waiting_after_Touch(KrazyVehical* ptr_vehical);
	KrazyBool onExecuteGamePlayMovingState_Moving_after_Violation(KrazyVehical* ptr_vehical);
};

