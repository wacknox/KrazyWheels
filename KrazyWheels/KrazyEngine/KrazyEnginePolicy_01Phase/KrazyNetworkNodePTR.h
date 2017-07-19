#pragma once
#include <vector>

using namespace std;
template<typename KRAZY_NET_NODE_PTR>
class KrazyNetworkNodePTR
{
public:
	vector<KrazyNetworkNodePTR*>* getNodeCache()
	{
		return &_NodeCache;
	}
	//
protected:
	KrazyNetworkNodePTR() :
	_PTROwner(new KRAZY_NET_NODE_PTR)
	{
		
	}
	
	virtual ~KrazyNetworkNodePTR(void)
	{
		cleanUp();
	}
	
	
	void cleanUp()
	{
		if(_PTROwner)	{ delete _PTROwner;_PTROwner = 0;}
		int count = _NodeCache.size();for(int i = 0; i < count; i++){	delete _NodeCache[i]; }
		_NodeCache.clear();
	}
	//
	KRAZY_NET_NODE_PTR*			    _PTROwner;
	vector<KRAZY_NET_NODE_PTR*>	    _PTRConnectedNodes;
	vector<KrazyNetworkNodePTR*>    _NodeCache;
	int							    _Index;
};

#if 0
template<typename KRAZY_NET_NODE_PTR>
class KrazyNetworkNodePTR
{
public:
	vector<KrazyNetworkNodePTR*>* getNodeCache()
	{
		return &_NodeCache;
	}
	//
protected:
	KrazyNetworkNodePTR() :
	_PTROwner(new KRAZY_NET_NODE_PTR)
	{
		
	}
	
	virtual ~KrazyNetworkNodePTR(void)
	{
		cleanUp();
	}
	
	
	void cleanUp()
	{
		if(_PTROwner)	{ delete _PTROwner;_PTROwner = 0;}
		int count = _NodeCache.size();for(int i = 0; i < count; i++){	delete _NodeCache[i]; }
		_NodeCache.clear();
	}
	//
	KRAZY_NET_NODE_PTR*			    _PTROwner;
	vector<KRAZY_NET_NODE_PTR*>	    _PTRConnectedNodes;
	vector<KrazyNetworkNodePTR*>    _NodeCache;
	int							    _Index;
};
#endif



