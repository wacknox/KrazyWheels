#pragma once
#include <queue>
#include "KrazyEnums.h"
using namespace std;
class KrazyDuty
{
public:
	//..,
	KrazyDuty(unsigned short _id) : 
	  _duty_id(_id)
	{
	}
	//..,
	virtual ~KrazyDuty(){}
	unsigned short duty_id(){ return _duty_id; }
	//..,
protected:
	unsigned short _duty_id;
};
//..,
class KrazyDutyList : public queue<KrazyDuty*>{};
//..,
class KrazyDutyKueue
{
public:
	//..,
	static KrazyDutyKueue* getDefault();
	static void releaseDefault();
	KrazyDuty* getNextDuty();
	void appendNewDuty(KrazyDuty* _duty);
	//..,
	KrazyBool& enable();
private:
	void cleanUp();
	KrazyDutyKueue(void);
	virtual ~KrazyDutyKueue(void);
	//..,
	static KrazyDutyKueue* _Me;
	KrazyDutyList _DutyQ;
	KrazyBool     _Enable;
};

