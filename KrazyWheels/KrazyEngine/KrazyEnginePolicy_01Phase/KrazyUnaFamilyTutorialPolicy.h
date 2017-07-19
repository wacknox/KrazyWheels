#include "KrazyUnaaFamily.h"
#include "KrazyGanguniMovingPolicy_01Phase.h"
#include "KrazyUnaBrothersMovingPolicy_01Phase.h"
//..,
class KrazyUnaaFamilyTutorialPolicy : public KrazyUnaaFamilyMovingPolicy_01Phase
{
public:
	KrazyUnaaFamilyTutorialPolicy();
	~KrazyUnaaFamilyTutorialPolicy();
	//..,
	void onExecuteDefaultMove(KrazyVehical* ptr_vehical);
	KrazyBool onExecuteTouch(KrazyVehical* _vehical);
	//..,
	void onExecute(KrazyVehical* vehical);
	KrazyBool onExecuteGamePlayMovingState_Waiting_at_NoEntry(KrazyVehical* ptr_vehical);
};
//..,
class KrazyGanguniMovingTutorialPolicy : public KrazyGanguniMovingPolicy_01Phase
{
public:
	KrazyGanguniMovingTutorialPolicy();
	~KrazyGanguniMovingTutorialPolicy();
	//..,
	void reset();
	//..,
	void onExecuteDefaultMove(KrazyVehical* ptr_vehical);
	KrazyBool onExecuteTouch(KrazyVehical* _vehical);
	//..,
	void onExecute(KrazyVehical* vehical);
	KrazyBool onExecuteGamePlayMovingState_Waiting_at_NoEntry(KrazyVehical* ptr_vehical);

private:
	KrazyBool _Was_waiting_at_NEntry;

};
//..,
class KrazyUnaBrothersMovingTutorialPolicy : public KrazyUnaBrothersMovingPolicy_01Phase
{
public:
	KrazyUnaBrothersMovingTutorialPolicy();
	~KrazyUnaBrothersMovingTutorialPolicy();
	//..,
	void reset();
	//..,
	void onExecuteDefaultMove(KrazyVehical* ptr_vehical);
	KrazyBool onExecuteTouch(KrazyVehical* _vehical);
	//..,
	void onExecute(KrazyVehical* vehical);
	KrazyBool onExecuteGamePlayMovingState_Waiting_at_NoEntry(KrazyVehical* ptr_vehical);
	//..,
private:
	KrazyBool _Was_waiting_at_NEntry;
};
//..,

