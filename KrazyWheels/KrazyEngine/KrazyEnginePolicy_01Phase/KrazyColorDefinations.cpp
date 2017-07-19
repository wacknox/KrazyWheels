#include "stdafx.h"
#include "KrazyColorDefinations.h"
//..,
KrazyFloatListColorDispensor* KrazyFloatListColorDispensor::_Me = 0;
KrazyFloatListColorDispensor* KrazyFloatListColorDispensor::getDefault()
{
	if(!_Me){
		_Me = new KrazyFloatListColorDispensor;
	}
	//..,
	return _Me;
}
//..,
void KrazyFloatListColorDispensor::releaseDefault()
{
	if(_Me){
		delete _Me;_Me = 0;
	}
}
//..,