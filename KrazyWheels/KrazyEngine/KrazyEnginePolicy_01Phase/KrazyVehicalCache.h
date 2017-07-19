#pragma once
#include "KrazyCache.h"
#include "KrazyVehical.h"
typedef KrazyCache<KrazyVehical> KrazyCache_KrazyVehical;
class KrazyVehicalCache : public KrazyCache_KrazyVehical
{
public:
	static KrazyVehicalCache* getCache();
	static void destroyCache();
	static void cleanData();
	static  KrazyVehical* allocate();
	static  void deallocate(KrazyVehical* vehical);
	//
	template<typename POLICY_OPERATING_ON_VEHICAL_CACHE>
	static void onSetVehicalsInNetwork(POLICY_OPERATING_ON_VEHICAL_CACHE* policy)
	{
		policy->onSetNodeVehical((vector<KrazyVehical*>*)_PTRMe->_DataPTRInTheNetList);
	}
	// Make  it more strict by checking the _PTRMe agains the this..,
	vector<KrazyVehical*>* get_VehicalsInTheNetwork()    { return &_DataPTRInTheNetList;}
	int				get_VehicalsInTheNetworkCount()		 { return _DataPTRInTheNetList.size();}
	KrazyVehical*	get_VehicalFromTheNetwork(int index) { return _DataPTRInTheNetList[index];}
	/*void			cleanUp();*/
	//
private:

	
	KrazyVehicalCache(){}
   ~KrazyVehicalCache(){}
	KrazyVehical* get_Allocated();
	void return_for_Deallocation(KrazyVehical* data_object);
	//
	static KrazyVehicalCache* _PTRMe;
};