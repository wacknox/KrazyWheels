#include "stdafx.h"
#include "KrazyVehicalCache.h"
#include "KrazyRedundentCallHelperMacros.h"
KrazyVehicalCache* KrazyVehicalCache::_PTRMe = 0;
//////////////////////////////////////////////////////// Static Methods /////////////////////////////////////////////////////
KrazyVehicalCache* KrazyVehicalCache::getCache()
{
	if(!_PTRMe){
		_PTRMe = new KrazyVehicalCache;
	}
	return _PTRMe;
}
//
void KrazyVehicalCache::destroyCache()
{
	if(_PTRMe){
		delete _PTRMe;_PTRMe = 0;
	}
}
//
void KrazyVehicalCache::cleanData()
{
	if(_PTRMe)
		_PTRMe->cleanUp();

}
//
KrazyVehical* KrazyVehicalCache::allocate()
{
	if(!_PTRMe)
		return 0;

	return _PTRMe->get_Allocated();
}
//
void KrazyVehicalCache::deallocate(KrazyVehical* vehical)
{
	if(_PTRMe){
		_PTRMe->return_for_Deallocation(vehical);
	}	
}
///////////////////////////////////////////////////////////// Class Methods //////////////////////////////////////////////////////////////////
KrazyVehical* KrazyVehicalCache::get_Allocated()
{
	KrazyVehical* vehical = 0;
	int expense_count = _DataPTRExpenseList.size();
	if(expense_count > 0){
		vehical = _DataPTRExpenseList[expense_count-1];
		_DataPTRExpenseList.pop_back();
		_DataPTRInTheNetList.push_back((KrazyCacheObject*)vehical);
	}
	else{
		vehical = KrazyCache_KrazyVehical::get_Allocated();
	}

	return vehical;
}
//
void KrazyVehicalCache::return_for_Deallocation(KrazyVehical* data_object)
{
	KrazyCacheObject* ptr_real_object = (KrazyCacheObject*)data_object;
	ptr_real_object->reset();		
	_DataPTRExpenseList.push_back(ptr_real_object);
	_KRAZY_REMOVE_ITEM_FROM_VECTOR_BY_ITEM_PTR(_DataPTRInTheNetList,KrazyVehical,data_object);
}
//
//void KrazyVehicalCache::cleanUp()
//{
//	int count = _DataPTRListCache.size();
//		for(int i = 0; i < count ; i++){
//			KrazyVehical* data_object = _DataPTRListCache[i];
//			delete data_object;data_object = 0;
//		}
//		_DataPTRListCache.clear();
//		_DataPTRExpenseList.clear();
//		_DataPTRInTheNetList.clear();
//}