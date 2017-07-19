#include "KrazyToonaBrothersMovingPolicy_01Phase.h"
//..,
class KrazyToonaBrotherTutorialPolicy : public KrazyToonaBrothersMovingPolicy_01Phase
{
public :
	
	KrazyToonaBrotherTutorialPolicy();
	virtual ~KrazyToonaBrotherTutorialPolicy();
	//..,
	void reset();
	KrazyBool onExecuteTouch(KrazyVehical* _vehical);
	KrazyBool onExecuteReflectionTouch(KrazyVehical* _vehical);
	//..,
	void onExecuteDefaultMove(KrazyVehical* ptr_vehical);
	void onExecute(KrazyVehical* vehical);
	//..,
	KrazyBool onExecuteGamePlayMovingState_Waiting_at_NoEntry(KrazyVehical* _vehical);
	void onExecutePostMatchDistance(KrazyVehical* ptr_vehical);
	//..,
private:
	KrazyBool _Was_in_NEntry;
};