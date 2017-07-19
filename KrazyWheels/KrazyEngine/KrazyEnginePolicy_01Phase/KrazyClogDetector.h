#pragma once
#include "KrazyPrimitiveDataCollections.h"
class KrazyVehical;
//..,
class KrazyMapVehical_to_Vehical : public KrazyMap<KrazyVehical* , KrazyVehical*>{};
//.,
class KrazyWheelsRendrer;
class KrazyClogDetector
{
public:
	//..,
	static KrazyClogDetector* getDefault();
	static void releaseDefault();
	//..,
private:
	KrazyClogDetector();
	virtual ~KrazyClogDetector();
	//..,
public:
	void attach_rendrer(KrazyWheelsRendrer* _rendrer);
	bool append_waitingvehicals(KrazyVehical* _waiting_ , KrazyVehical* _waiting_for_);
	void clear(KrazyVehical* _waiting_);
	KrazyVehical* waiting_for(KrazyVehical* _waiting);
	//..,
private:
	bool _clog_detected(KrazyVehical* _waiting_ , KrazyVehical* _waiting_for_);
	KrazyMapVehical_to_Vehical    _MapVehicals;
	static KrazyClogDetector*     _Me;
	bool						  _ClogDetected;
	KrazyWheelsRendrer*			  _Rendrer;
};

