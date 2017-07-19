#pragma once
#include "KrazyToonaBrothersMovingPolicy_01Phase.h"
#include "KrazyVehical.h"
class KrazyRangToonaMovingPolicy_01Phase :
	public KrazyToonaBrothersMovingPolicy_01Phase
{
public:
	void onExecute(KrazyVehical* vehical , KrazyALGODiagnostic** diagnostic);
	void reset();
	KrazyRangToonaMovingPolicy_01Phase(void);
	virtual ~KrazyRangToonaMovingPolicy_01Phase(void);
    //
protected:
	
};

