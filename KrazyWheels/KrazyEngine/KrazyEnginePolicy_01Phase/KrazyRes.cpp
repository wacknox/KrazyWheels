#include "stdafx.h"
#include "KrazyRes.h"
//..,
KrazyRes* KrazyRes::_Me	= 0;
KrazyRes::KrazyRes(void)
{
	reset();
}
//..,
KrazyRes::~KrazyRes(void)
{
}
//..,
KrazyRes* KrazyRes::getDefault()
{
	if(!_Me){
		_Me = new KrazyRes;
	}
	//..,
	return _Me;
}
//..,
void KrazyRes::releaseDefault()
{
	if(_Me){
		delete _Me;_Me = 0;
	}
}
//....................................................................................................................
string& KrazyRes::apk_path(){ return _APKPath; }
//....................................................................................................................
void KrazyRes::reset()
{
	_APKPath.clear();
}
//..,