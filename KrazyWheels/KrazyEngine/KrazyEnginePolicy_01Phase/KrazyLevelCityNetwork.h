#pragma once
#ifndef _KRAZYLEVELCITYNETWORK_H_
#define _KRAZYLEVELCITYNETWORK_H_
//
#include "KrazyNetworkNodePTR.h"
#include "KrazyLaneNode.h"
#include "KrazyNodeTraverserIndexers.h"
#include "KrazyDoubleLaneRoadRendrer.h"
#include "KrazyXMLNode.h"
#include "KrazyEntryIndicatorExtractionPolicy_01Phase.h"
#include "KrazyGameRenderingPolicy_01Phase.h"
#include "KrazyPrimitiveDataCollections.h"
#include "KrazyLevelCityContextInterface.h"
//
#if 1
typedef KrazyNetworkNodePTR<KrazyLaneNode>  KrazyWheelNetwork_01Phase;
template<typename KRAZY_RENDRER>
class KrazyNetworkCity : public  KrazyWheelNetwork_01Phase
{
	
public:
	/// creations..,
	static KrazyNetworkCity* getNetwork()
	{
		if(!_PtrMe){
			_PtrMe = new KrazyNetworkCity();
		//	_PtrMe->intialize_rendrer();			
			KrazyXMLNode::getXMLNodeParent();
		}

		return _PtrMe;
	}
	// network operation..,
	template <typename KRAZY_EXTRACTION_XML_TO_KRAZY_NET>
	static void loadNetworkFromXML(string xml , KrazyLevelCityContext* city_context)
	{
		if(!_PtrMe)
			return ;
	
		KRAZY_EXTRACTION_XML_TO_KRAZY_NET extract(city_context);
		extract.registerOnNewNetworkNodeCallback(KrazyNetworkCity::onGetNewNode);
		KrazyXMLNode::loadXML(xml);
		extract.onExecute(KrazyXMLNode::getXMLNodeParent());
	}
	//
	static  string getNetworkXML()
	{
		return KrazyXMLNode::getXML();
	}
	// manager
	static  void manageNetwork()
	{
		if(_PtrMe) {
			_PtrMe->manage();
		}
	}
	//
	//static  void renderNetwork()
	//{
	//	if(_PtrMe){
	//		_PtrMe->render();
	//	}
	//}
	
	static void resizeNetwork()
	{
		if(_PtrMe){
			_PtrMe->on_resize();
		}
	}
	// static cleanups..,
	static  void cleanNetworkData()
	{
		if(_PtrMe){
			_PtrMe->cleanUp();
			KrazyXMLNode::cleanUpData();
		}
	}
	//
	static  void destroyNetwork()
	{
		if(_PtrMe){ 
			delete _PtrMe;_PtrMe = 0; 
			KrazyXMLNode::releaseParent();
		}
	}
	//refreshing level context data;
	template<typename KRAZY_LEVEL_CONTEXT>
	static void onRefreshLayoutRendrer(KRAZY_LEVEL_CONTEXT* ptr_level_context)
	{
		if(_PtrMe && _PtrMe->_Rendrer){
			_PtrMe->_Rendrer->onRefresh(ptr_level_context);
		}
	}
	// operations on data..,
	template<typename DATA_PACKING_POLICY>
	static void onOperateOnNodeCacheDataByPolicy(DATA_PACKING_POLICY* policy)
	{
		/* wish to have some kind of approval policies , for these policies;., some kind of policy table..,
		   for right now using protected / private inner classes to make it more strict..,
		*/
		if(_PtrMe){ 
			policy->onExecute((vector<KrazyNetworkCity*>*)_PtrMe->getNodeCache());
		}
	}
	//
	template<typename _KRAZY_GAMEPLAY_POLICY>
	static void onSetNodeCacheToPolicy(_KRAZY_GAMEPLAY_POLICY* policy)
	{
		if(_PtrMe){ 
			policy->onSetNodeCache((vector<KrazyNetworkCity*>*)_PtrMe->getNodeCache());
		}
	}
	// appending..,
	void appendConnectedNode(KrazyLaneNode* connected_node);
	// properties..,

	/*void reset_rendrer()
	{
		_Rendrer->reset();
	}*/
	//
	/*void create_rendrer( KrazyLevelCityContext* city_context)
	{
		_Rendrer->onCreateProgramme();
		_Rendrer->onRefresh(city_context);
	}*/
	//
	/*void intialize_rendrer()
	{
		if(this != KrazyNetworkCity::_PtrMe)
			return ;

		_Rendrer.initialize();
	}*/
//need to check
bool			  hasConnectedNodes()			{ return (_PTRConnectedNodes.size() > 0);}
KrazyLaneNode*    getOwner()					{ return _PTROwner;}
int				  getIndex()					{ return _Index; }
int				  getConnectedNodesCount()		{ return _PTRConnectedNodes.size();}
KrazyLaneNode*	  getConnectedNode(int index)	{ return _PTRConnectedNodes[index];}
KrazyNetworkCity* getOwnerNodeContainerNode(KrazyLaneNode* node)
				{
					if(this != KrazyNetworkCity::_PtrMe)
						return 0;
	
					KrazyNetworkCity* network_city_node = 0;
					int count = _NodeCache.size();
					for(int i = 0; i < count; i++){
						network_city_node = _NodeCache[i];
						if(network_city_node->getOwner() == node){
							return network_city_node;
						}
					}

					return network_city_node;
				}
//..,
			
		void on_set_rendrer(KRAZY_RENDRER* _rendrer)
		{
			_Rendrer = _rendrer;
		}
		//..
private:
	

		void manage()
		{
			 if(this != KrazyNetworkCity::_PtrMe)
				return ;

			// _ManagementPolicy.onExecute();
		}
//		void on_resize()
//		{
//			if(this != KrazyNetworkCity::_PtrMe)
//				return;
//#if 0
//			vector<KrazyNetworkNodePTR*>* node_cache = getNodeCache();
//			int count = node_cache->size();
//			for(int i = 0; i < count; i++)
//			{
//				((KrazyNetworkCity_01Phase*)node_cache->at(i))->getOwner()->on_resize();
//			}
//#endif
//			//
//			_Rendrer->on_resize();
//		}
		//void render()
		//{
		//   if(this != KrazyNetworkCity::_PtrMe)
		//	 return ;

		//	_Rendrer.render();
		//  //Render All the cars later..,
		//}
		////
		//KRAZY_RENDRER* getRendrer()
		//{
		//	if(this != KrazyNetworkCity::_PtrMe)
		//		return 0;

		//	return &_Rendrer;
		//}
        //	
		KrazyNetworkCity* getNewNode()
		{
			if(this != KrazyNetworkCity::_PtrMe)
				return 0;

			KrazyNetworkCity* ptr_newNode = new KrazyNetworkCity;
			_NodeCache.push_back(ptr_newNode);
			return ptr_newNode;

		}
		//
		KrazyLaneNodeList* getConnectedNodes()
		{
			return &_PTRConnectedNodes;
		}
		//
		static KrazyNetworkCity* onGetNewNode()
		{
			if(!_PtrMe)
				return 0;
			return _PtrMe->getNewNode();
		}
		//

    //
	KrazyNetworkCity() : _Rendrer(0) {}
	~KrazyNetworkCity(){}
	//
	KRAZY_RENDRER*							       _Rendrer;
	static KrazyNetworkCity*				       _PtrMe;
	static int									   _IndexGenrator;
};
#else
//
class KrazyNetworkCity : public  KrazyNetworkNodePTR
{
	
public:
	/// creations..,
	static KrazyNetworkCity* KrazyNetworkCity::getNetwork()
	{
		if(!_PtrMe){
			_PtrMe = new KrazyNetworkCity();
			//_PtrMe->intialize_rendrer();			
			KrazyXMLNode::getXMLNodeParent();
		}

		return _PtrMe;
	}
	// network operation..,
	template <typename KRAZY_EXTRACTION_XML_TO_KRAZY_NET>
	static void loadNetworkFromXML(string xml)
	{
		if(!_PtrMe)
			return ;
	
		KRAZY_EXTRACTION_XML_TO_KRAZY_NET extract;
		extract.registerOnNewNetworkNodeCallback(KrazyNetworkCity::onGetNewNode);
		KrazyXMLNode::loadXML(xml);
		extract.onExecute(KrazyXMLNode::getXMLNodeParent());
	}
	//
	static  string KrazyNetworkCity::getNetworkXML()
	{
		return KrazyXMLNode::getXML();
	}
	// manager
	static  void KrazyNetworkCity::manageNetwork()
	{
		if(_PtrMe) {
			_PtrMe->manage();
		}
	}
	//
	static  void KrazyNetworkCity::renderNetwork()
	{
		if(_PtrMe){
			_PtrMe->render();
		}
	}
	// static cleanups..,
	static  void KrazyNetworkCity::cleanNetworkData()
	{
		if(_PtrMe){
			_PtrMe->cleanUp();
			KrazyXMLNode::cleanUpData();
		}
	}
	//
	static  void KrazyNetworkCity::destroyNetwork()
	{
		if(_PtrMe){ 
			delete _PtrMe;_PtrMe = 0; 
			KrazyXMLNode::releaseParent();
		}
	}
	//refreshing level context data;
	template<typename KRAZY_LEVEL_CONTEXT>
	static void onRefreshLayoutRendrer(KRAZY_LEVEL_CONTEXT* ptr_level_context)
	{
		if(_PtrMe){
			_PtrMe->_Rendrer.onRefresh(ptr_level_context);
		}
	}
	// operations on data..,
	template<typename DATA_PACKING_POLICY>
	static void onOperateOnNodeCacheDataByPolicy(DATA_PACKING_POLICY* policy)
	{
		/* wish to have some kind of approval policies , for these policies;., some kind of policy table..,
		   for right now using protected / private inner classes to make it more strict..,
		*/
		if(_PtrMe){ 
			policy->onExecute((vector<KrazyNetworkCity*>*)_PtrMe->getNodeCache());
		}
	}
	//
	template<typename _KRAZY_GAMEPLAY_POLICY>
	static void onSetNodeCacheToPolicy(_KRAZY_GAMEPLAY_POLICY* policy)
	{
		if(_PtrMe){ 
			policy->onSetNodeCache((vector<KrazyNetworkCity*>*)_PtrMe->getNodeCache());
		}
	}
	// appending..,
	void appendConnectedNode(KrazyLaneNode* connected_node);
	// properties..,

//need to check
bool			  hasConnectedNodes()			{ return (_PTRConnectedNodes.size() > 0);}
KrazyLaneNode*    getOwner()					{ return _PTROwner;}
int				  getIndex()					{ return _Index; }
int				  getConnectedNodesCount()		{ return _PTRConnectedNodes.size();}
KrazyLaneNode*	  getConnectedNode(int index)	{ return _PTRConnectedNodes[index];}
KrazyNetworkCity* getOwnerNodeContainerNode(KrazyLaneNode* node)
				{
					if(this != KrazyNetworkCity::_PtrMe)
						return 0;
	
					KrazyNetworkCity* network_city_node = 0;
					int count = _NodeCache.size();
					for(int i = 0; i < count; i++){
						network_city_node = _NodeCache[i];
						if(network_city_node->getOwner() == node){
							return network_city_node;
						}
					}

					return network_city_node;
				}
private:
		void intialize_rendrer()
		{
			if(this != KrazyNetworkCity::_PtrMe)
				 return ;

			_Rendrer.initialize();
		}

		void manage()
		{
			 if(this != KrazyNetworkCity::_PtrMe)
				return ;

			// _ManagementPolicy.onExecute();
		}

		void render()
		{
		   if(this != KrazyNetworkCity::_PtrMe)
			 return ;

			_Rendrer.render();
		  //Render All the cars later..,
		}

		/*KRAZY_RENDRER* getRendrer()
		{
			if(this != KrazyNetworkCity::_PtrMe)
				return 0;

			return &_Rendrer;
		}*/
        //	
		KrazyNetworkCity* getNewNode()
		{
			if(this != KrazyNetworkCity::_PtrMe)
				return 0;

			KrazyNetworkCity* ptr_newNode = new KrazyNetworkCity;
			_NodeCache.push_back(ptr_newNode);
			return ptr_newNode;

		}
		//
		KrazyLaneNodeList* getConnectedNodes()
		{
			return &_PTRConnectedNodes;
		}
		//
		static KrazyNetworkCity* onGetNewNode()
		{
			if(!_PtrMe)
				return 0;
			return _PtrMe->getNewNode();
		}
    //
	KrazyNetworkCity() {}
	~KrazyNetworkCity(){}
	//
	KRAZY_RENDRER							       _Rendrer;
	static KrazyNetworkCity*				       _PtrMe;
	static int									   _IndexGenrator;
};
#endif
//
typedef KrazyNetworkCity<KrazyDoubleLaneRendrer> KrazyNetworkCity_01Phase;
//
struct _helper_indexer_network_ : public _helper_indexer
{
	_helper_indexer_network_() :
	_PTRNetworkCityNode(0)
	{}

	KrazyNetworkCity_01Phase*    _PTRNetworkCityNode;
};
//
struct _unlinked_traffic_indc_memory_slot
	{

	 _unlinked_traffic_indc_memory_slot(float angle , int type , int parent_index , int parent_single_lane_index , int self_parent_index , int self_single_lane_index , int self_source_traffic_indc_index , KrazyFloatList& point , KrazyFloatList& rect)
	  {
		 set_Data(angle , type , parent_index , parent_single_lane_index , self_parent_index , self_single_lane_index , self_source_traffic_indc_index , point , rect);
	  }
	  _unlinked_traffic_indc_memory_slot()
		   :
	    _rotation_angle(0.0f)
	   ,_type(0)
	   ,_parent_index(-1)
	   ,_parent_single_lane_index(-1)
	   ,_self_parent_index(-1)
	   ,_self_single_lane_index(-1)
	   ,_self_source_traffic_indc_index(-1)
	    {}

		void set_Data(float angle , int type , int parent_index , int parent_single_lane_index , int self_parent_index , int self_single_lane_index ,int self_source_traffic_indc_index , KrazyFloatList& point , KrazyFloatList& rect)
		{
			_rotation_angle = angle;
			_type = type;
			_parent_index = parent_index;
			_parent_single_lane_index = parent_single_lane_index;
			_self_parent_index = self_parent_index;
			_self_single_lane_index = self_single_lane_index;
			_self_source_traffic_indc_index = self_source_traffic_indc_index;
		
			int count = point.size();
			for(int i = 0; i < count && count == 2; i++){
				_point.push_back(point[i]);
			}

			count = rect.size();
			for(int i = 0; i < count && count == 4; i++){
				_rect.push_back(rect[i]);
			}
		}
		void  on_setFloatArray(KrazyListToFloatList& list)
		{  
			int count = list.size();
			for(int i = 0; i < count; i++){
				int index = _array_float_array.size();
				_array_float_array.push_back(KrazyFloatList());
				KrazyFloatList* ptr_float_list_d = &_array_float_array[index];
				KrazyFloatList* ptr_float_list_s = &list[i];
				int count_float_list = ptr_float_list_s->size();
				for(int elem = 0;elem < count_float_list;elem++){
					ptr_float_list_d->push_back(ptr_float_list_s->at(elem));
				}
			}
		}
		void on_setIntList(KrazyIntList& list)
		{
			int count = list.size();
			for(int i = 0; i < count; i++){
				_int_list.push_back(list[i]);
			}
		}
		void on_setVehicalNameList(KrazyIntList& list)
		{
			int count = list.size();
			for(int i = 0; i < count; i++)
			{
				_vehical_name_list.push_back(list[i]);
			}
		}
		void on_setVehicalShadeList(KrazyIntList& list)
		{
			int count = list.size();
			for(int i = 0; i < count; i++)
			{
				_vehical_shade_list.push_back(list[i]);
			}
			
		}
		void on_setVehicalShadeTypeList(KrazyIntList& list)
		{
			int count = list.size();
			for(int i = 0; i < count; i++)
			{
				_vehical_shadetype_list.push_back(list[i]);
			}
		}
		void on_setAssignedLaneList_for_Sherrif(KrazyIntList& list)
		{
			int count = list.size();
			for(int i = 0; i < count; i++)
			{
				_assigned_lane_for_sherrif.push_back(list[i]);
			}
		}

	   float					_rotation_angle;
	   int						_type;
	   int						_parent_index;
	   int                      _parent_single_lane_index;
	   int						_self_parent_index;
	   int                      _self_single_lane_index;
	   int                      _self_source_traffic_indc_index;
	   KrazyFloatList			_point;
	   KrazyFloatList			_rect;
	   KrazyListToFloatList     _array_float_array;
	   KrazyIntList             _int_list;
	   KrazyIntList				_vehical_name_list;
	   KrazyIntList				_vehical_shade_list;
	   KrazyIntList				_vehical_shadetype_list;
	   KrazyIntList				_assigned_lane_for_sherrif;
	  };
//
class KrazyTrafficIndcMemorySlotList            : public KrazyDynamicallyAllocatedPtrList<_unlinked_traffic_indc_memory_slot>{};
class KrazyMapLaneIndextoDestinationIndicator	: public hash_map<int , KrazyTrafficIndcMemorySlotList*>   
{
public:
	~KrazyMapLaneIndextoDestinationIndicator()
	{
		cleanUp();
	}
	//..,
private:
	void cleanUp()
	{
		KrazyMapLaneIndextoDestinationIndicator::iterator _Iter = begin();
		for(;_Iter != end();_Iter++){
			delete _Iter->second;
		}
		clear();
	}
};
class KrazyNetworkDepthIntToIndexer_Packing		: public hash_map<int , _helper_indexer_network_*>{};
class KrazyNetworkDepthNode_Packing				//: public hash_map<KrazyXMLNode* , _helper_indexer_network_>
{
public:
	~KrazyNetworkDepthNode_Packing()
	{
		int indexers = _Indexers.size();
		for(int i = 0; i < indexers; i++){
			_helper_indexer_network_* _indexer = _Indexers[i];
			delete _indexer;_indexer = 0;
		}
	}
	////
	bool find(KrazyXMLNode* ptr_xml_node)
	{
		int count = _XMLNodes.size();
		for(int i = 0; i < count; i++)
		{
			if(_XMLNodes[i] == ptr_xml_node)
			{
				return true;
			}
		}
		
		return false;
	}

	_helper_indexer_network_* operator[](KrazyXMLNode* ptr_xml_node)
	{
		if(_XMLNodes.size() != _Indexers.size())
			return 0;
		int count = _XMLNodes.size();
		for(int i = 0; i < count; i++)
		{
			if(_XMLNodes[i] == ptr_xml_node)
			{
				return _Indexers[i];
			}
		}

		int index = _XMLNodes.size();
		_XMLNodes.push_back(ptr_xml_node);
		_Indexers.push_back(new _helper_indexer_network_());

		return _Indexers[index];
	}

private:
	vector<KrazyXMLNode*> _XMLNodes;
	vector<_helper_indexer_network_*> _Indexers;

};
//
typedef	KrazyNetworkCity_01Phase*(*on_getNewKrazyNetworkNode)();
//
class KrazyXML_to_KrazyNetwork
{
public :
	KrazyXML_to_KrazyNetwork(KrazyLevelCityContext* level_city_context);
	~KrazyXML_to_KrazyNetwork();
	void registerOnNewNetworkNodeCallback(on_getNewKrazyNetworkNode ptr_CB){_on_getNewNetworkNode = ptr_CB;}
	void onExecute(KrazyXMLNode* ptr_xmlNode);
private:
	on_getNewKrazyNetworkNode  _on_getNewNetworkNode;
	KrazyLevelCityContext*         _PTRLevelCityContext;

};
//
class KrazyParentFinderPolicyForKrazyTrafficIndicator
{
	
public :
	KrazyParentFinderPolicyForKrazyTrafficIndicator(KrazyMapLaneIndextoDestinationIndicator* indc_map , KrazyLevelCityContext* level_city_context) :
	   _PTRIndc_map(indc_map),
	   _PTRLevelCityContext(level_city_context)
	   {}
	void onExecute(vector<KrazyNetworkCity_01Phase*>* network);
private:
	KrazyMapLaneIndextoDestinationIndicator* _PTRIndc_map;
	KrazyLevelCityContext*         _PTRLevelCityContext;
};
///
#ifdef _IS_FOR_SIMONE
class KrazyDisplayTrafficIndicators
{
public:
	KrazyDisplayTrafficIndicators(unsigned int postion , unsigned int color);
	void onExecute(vector<KrazyNetworkCity_01Phase*>* network);
protected:
	unsigned int _Position;
	unsigned int _Color;
};
#endif
#endif






