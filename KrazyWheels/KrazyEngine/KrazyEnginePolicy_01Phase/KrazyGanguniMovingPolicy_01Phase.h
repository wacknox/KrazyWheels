#pragma once
#include "KrazyUnaaFamily.h"
#include "KrazyVehical.h"
class KrazyGanguniMovingPolicy_01Phase : public KrazyUnaaFamilyMovingPolicy_01Phase
{
public:
	KrazyGanguniMovingPolicy_01Phase(void);
	virtual ~KrazyGanguniMovingPolicy_01Phase(void);
	void onExecute(KrazyVehical* vehical , KrazyALGODiagnostic** diagnostic);
	void reset();
protected:
	
};

