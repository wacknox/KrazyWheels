#include "StdAfx.h"
#include "KrazyDutyKueue.h"
KrazyDutyKueue* KrazyDutyKueue::_Me = 0;
//..,
KrazyDutyKueue::KrazyDutyKueue(void) :
_Enable(KRAZY_BOOL_True)
{
}
//..,
KrazyDutyKueue::~KrazyDutyKueue(void)
{
	cleanUp();
}
//..,
void KrazyDutyKueue::cleanUp()
{
	for(;_DutyQ.size() > 0;){
		KrazyDuty* _duty = _DutyQ.front();
		delete _duty;_duty = 0;_DutyQ.pop();
	}
}
//..,
KrazyDutyKueue* KrazyDutyKueue::getDefault()
{
	if(!_Me)
	{
		_Me = new KrazyDutyKueue;
	}
	//..,
	return _Me;
}
//..,
void KrazyDutyKueue::releaseDefault()
{
	if(_Me){
		delete _Me;_Me = 0;
	}
}
//..,
KrazyBool& KrazyDutyKueue::enable() { return _Enable; }
//..,
void KrazyDutyKueue::appendNewDuty(KrazyDuty* _duty)
{
	_DutyQ.push(_duty);
}
//..,
KrazyDuty* KrazyDutyKueue::getNextDuty()
{
	KrazyDuty* _duty = 0;
	if(_DutyQ.size() > 0)
	{
		_duty = _DutyQ.front();
		_DutyQ.pop();
	}
	//..,
	return _duty;
}
//..,
