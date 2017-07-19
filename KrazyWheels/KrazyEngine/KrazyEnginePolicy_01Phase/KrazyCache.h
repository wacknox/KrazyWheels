#pragma once
#include "KrazyRedundentClassDeclarationMacros.h"
#include "KrazyRedundentCallHelperMacros.h"
#include "KrazyPrimitiveDataCollections.h"
///////////////////////////////////////////////////////////
template<typename DATA_OBJECT>
class KrazyCache
{
protected:
	KRAZY_DATA_ENCAPSULATION(KrazyCacheObject , DATA_OBJECT)
	//
	KrazyCache(){}	
	virtual ~KrazyCache(){
		cleanUp();
	}
	//
	 DATA_OBJECT* get_newDataObject()
	{
		int count = _DataPTRListCache.size();
		_DataPTRListCache.push_back(new KrazyCacheObject);
		return _DataPTRListCache[count];
	}
    //
	void cleanUp()
	{
		int count = _DataPTRListCache.size();
		for(int i = 0; i < count ; i++){
			KrazyCacheObject* data_object = (KrazyCacheObject*)_DataPTRListCache[i];
			delete data_object;data_object = 0;
		}
		_DataPTRListCache.clear();
		_DataPTRExpenseList.clear();
		_DataPTRInTheNetList.clear();
	}
	//
	
	DATA_OBJECT* get_Allocated(bool recycle = true)
	{
		int count = _DataPTRListCache.size();
		KrazyCacheObject* data_object = new KrazyCacheObject;
		_DataPTRListCache.push_back(data_object);
		if(recycle){
			_DataPTRInTheNetList.push_back(data_object);
		}
	   return data_object;
	}
	//
	void return_for_Deallocation(DATA_OBJECT* data_object)
	{
		_KRAZY_REMOVE_ITEM_FROM_VECTOR_BY_ITEM_PTR(_DataPTRListCache , DATA_OBJECT , data_object)
	}
	//	
	
	vector<DATA_OBJECT*>	_DataPTRListCache;
	vector<DATA_OBJECT*>	_DataPTRExpenseList;
	vector<DATA_OBJECT*>	_DataPTRInTheNetList;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////