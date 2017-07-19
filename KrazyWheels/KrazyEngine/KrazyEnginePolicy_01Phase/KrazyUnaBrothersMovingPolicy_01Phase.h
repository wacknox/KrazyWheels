#pragma once
#include "krazyunaafamily.h"
//..,
#define _ENABLE_WAITING_AFTER_TOUCH
//..,
class KrazyUnaBrothersMovingPolicy_01Phase :
	public KrazyUnaaFamilyMovingPolicy_01Phase
{
public:
	KrazyUnaBrothersMovingPolicy_01Phase(void);
	virtual ~KrazyUnaBrothersMovingPolicy_01Phase(void);
	void onExecute(KrazyVehical* vehical);
	void reset();
protected:
	//..,
#ifdef _ENABLE_WAITING_AFTER_TOUCH
	KrazyBool onExecuteTouch(KrazyVehical* _vehical);
#endif
	//..,
#if 0
	void onExecutePostMatchDistance(KrazyVehical* ptr_vehical);
#endif
	//..,
#ifdef _ENABLE_WAITING_AFTER_TOUCH
	void onExecuteDefaultMove(KrazyVehical* ptr_vehical);
	KrazyBool onExecuteGamePlayMovingState_Waiting_after_Touch(KrazyVehical* _vehical);
#endif
	//..,
	KrazyBool onExecuteGamePlayMovingState_Waiting_at_Block(KrazyVehical* ptr_vehical);
	//...,
	KrazyBool onExecutePreBlockFliping(KrazyVehical* _vehical);

	unsigned int _Wait_after_Touch;
};

