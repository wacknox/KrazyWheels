#pragma once 
#include "KrazySherrifMovingPolicy_01Phase.h"
class KrazySherrifTutorialMovingPolicy : public KrazySherrifMovingPolicy_01Phase
{
public :
	void onExecute(KrazyVehical* vehical , KrazyALGODiagnostic** diagnostic);
	//..,
	void onExecutePostMatchDistance(KrazyVehical* ptr_vehical);
	void onExecuteDefaultMove(KrazyVehical* ptr_vehical);
};