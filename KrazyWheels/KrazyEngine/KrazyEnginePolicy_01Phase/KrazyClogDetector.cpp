#include "stdafx.h"
#include "KrazyClogDetector.h"
#include "KrazyWheelsRendrer.h"
#include "KrazyVehical.h"
//..,
KrazyClogDetector* KrazyClogDetector::_Me = 0;
//..,
KrazyClogDetector* KrazyClogDetector::getDefault()
{
	if(!_Me)
	{
		_Me = new KrazyClogDetector;
	}
	//..,
	return _Me;

}
//..,
void KrazyClogDetector::releaseDefault()
{
	if(_Me){
		delete _Me;_Me = 0;
	}
}
//..,
KrazyClogDetector::KrazyClogDetector() :
_ClogDetected(false)
{
}
//..,
KrazyClogDetector::~KrazyClogDetector()
{
}
//..,
void KrazyClogDetector::attach_rendrer(KrazyWheelsRendrer* _rendrer)
{
	_Rendrer = _rendrer;
}
//..,
bool KrazyClogDetector::append_waitingvehicals(KrazyVehical* _waiting_ , KrazyVehical* _waiting_for_)
{
	if(_ClogDetected)
		return false;
	//..,
	if(_MapVehicals.find(_waiting_)){
		if(_MapVehicals[_waiting_] != _waiting_for_){
			_MapVehicals.replaceat(_waiting_ , _waiting_for_);
		}
	}
	else{
		_MapVehicals.insert(_waiting_ , _waiting_for_);
	}
	//..,
	return _clog_detected(_waiting_ , _waiting_for_);
}
//..,
void KrazyClogDetector::clear(KrazyVehical* _waiting_)
{
	if(_MapVehicals.find(_waiting_)){
		_MapVehicals.erase(_waiting_);
	}
}
//..,
KrazyVehical* KrazyClogDetector::waiting_for(KrazyVehical* _waiting)
{
	if(_MapVehicals.find(_waiting)){
		return _MapVehicals[_waiting];
	}
	//..,
	return 0;
	
}
//..,
bool KrazyClogDetector::_clog_detected(KrazyVehical* _waiting_ , KrazyVehical* _waiting_for_)
{
	KrazyVehical* _a_waiting = _waiting_;
	KrazyVehical* _a_waiting_for = _waiting_for_;
	__KrazyVehicalList _waiting_list;_waiting_list.push_back(_a_waiting);
	for(;_a_waiting_for;){
		if(_MapVehicals.find(_a_waiting_for)){
			KrazyVehical* _c_a_waiting_for = _MapVehicals[_a_waiting_for];
			int _count = _waiting_list.size();
			for(int i = 0; i < _count; i++){
				if(_waiting_list[i] == _c_a_waiting_for){
					//clog detected..,
					_ClogDetected = true;
					_waiting_list.push_back(_a_waiting_for);
					_Rendrer->on_clogged(_waiting_list);
					return true;
				}
			}
			//..
			_waiting_list.push_back(_a_waiting_for);
			_a_waiting_for = _c_a_waiting_for;
		}
		else{
			break;
		}
	}
	return false;
}
//..,
 