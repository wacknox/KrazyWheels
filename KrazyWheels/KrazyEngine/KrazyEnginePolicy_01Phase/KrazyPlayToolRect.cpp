#include "stdafx.h"
#include "KrazyPlayToolRect.h"
#include "KrazyColorDefinations.h"
//..,
KrazyPlayToolRect* KrazyPlayToolRect::_Me = 0;
//..,
KrazyPlayToolRect* KrazyPlayToolRect::get_default()
{
	if(!_Me){
		_Me = new KrazyPlayToolRect;
	}
	//..,
	return _Me;
}
//..,
void KrazyPlayToolRect::release_default()
{
	if(_Me){
		delete _Me;_Me = 0;
	}
}
//..,
void KrazyPlayToolRect::reset()
{
	_Vectors.clear();
}
//..,
void KrazyPlayToolRect::onSetVectors(KrazyFloatList& _vectors)
{
	_KRAZY_COPY_LIST(_vectors , _Vectors);
}
//..,
KrazyPlayToolRect::KrazyPlayToolRect()
{
}
//..,
KrazyPlayToolRect::~KrazyPlayToolRect()
{
}
//
KrazyFloatList* KrazyPlayToolRect::get_vectors()  { return &_Vectors;  }
//..
bool KrazyPlayToolRect::empty()
{
	return (_Vectors.size() == 0);
}
//........................................................................................................................................
