#include "StdAfx.h"
#include "KrazyEntryIndicatorCache.h"
#include "KrazyRedundentCallHelperMacros.h"
//
KrazyEntryIndicatorCache* KrazyEntryIndicatorCache::_PTRMe = 0;
KrazyEntryIndicatorCache::KrazyEntryIndicatorCache(void)
{
}
//
KrazyEntryIndicatorCache::~KrazyEntryIndicatorCache(void)
{
}
//
KrazyEntryIndicatorCache* KrazyEntryIndicatorCache::getCache()
{
	if(!_PTRMe){
		_PTRMe = new KrazyEntryIndicatorCache;
	}

	return _PTRMe;
}
//
void KrazyEntryIndicatorCache::destroyCache()
{
	if(_PTRMe){
		delete _PTRMe;_PTRMe = 0;
	}
}
//
void KrazyEntryIndicatorCache::cleanData()
{
	if(_PTRMe){
		_PTRMe->cleanUp();
	}
}
//
void KrazyEntryIndicatorCache::on_resize()
{
	if(_PTRMe)
	{
		vector<KrazyTrafficIndicator*>* indc = _PTRMe->get_EntryIndicators();
		int count = indc->size();
		for(int i = 0; i < count; i++)
		{
			indc->at(i)->on_resize();
		}
	}
}
//
KrazyTrafficIndicator* KrazyEntryIndicatorCache::allocate()
{
	if(!_PTRMe)
		return 0;
	
	return _PTRMe->get_Allocated(false);
}
//
