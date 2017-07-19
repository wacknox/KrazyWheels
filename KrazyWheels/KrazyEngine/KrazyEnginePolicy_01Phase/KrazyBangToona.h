#pragma once
#include "KrazyToonaBrothersMovingPolicy_01Phase.h"
#include "KrazyVehical.h"
class KrazyBangToona :
	public KrazyToonaBrothersMovingPolicy_01Phase
{
public:
	KrazyBangToona(void);
	virtual ~KrazyBangToona(void);
	//.......
	void onExecute(KrazyVehical* vehical );
	void reset();
	//...............
protected:
	KrazyBool onExecuteReflectionTouch(KrazyVehical* _vehical);
	KrazyBool onExecuteCrossingBlock(KrazyVehical* _vehical);
};

