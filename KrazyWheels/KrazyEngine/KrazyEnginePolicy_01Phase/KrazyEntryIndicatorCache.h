#pragma once
#include "KrazyCache.h"
#include "KrazyTrafficIndicator.h"
#include "KrazyRedundentClassDeclarationMacros.h"
///
class KrazyEntryIndicatorCache : public KrazyCache<KrazyTrafficIndicator>
{
public :
	static KrazyEntryIndicatorCache* getCache();
	static void destroyCache();
	static void cleanData();
	static void on_resize();
	static  KrazyTrafficIndicator* allocate();
	/////
	vector<KrazyTrafficIndicator*>* get_EntryIndicators(){ return &_DataPTRListCache; }
protected:
	KrazyEntryIndicatorCache(void);
	virtual ~KrazyEntryIndicatorCache(void);
	///
private:
	 static   KrazyEntryIndicatorCache*		_PTRMe;
};

