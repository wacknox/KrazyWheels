#include "StdAfx.h"
#include "KrazyLevelCityNetwork.h"
#include "KrazyEntryIndicatorCache.h"
#include "KrazyRedundentCallHelperMacros.h"
#ifdef _IS_FOR_SIMONE
#include "GLeish.h"
#include "KrazyColorDefinations.h"
#endif
template<>
KrazyNetworkCity_01Phase* KrazyNetworkCity_01Phase::_PtrMe = 0;
template<>
int KrazyNetworkCity_01Phase::_IndexGenrator = 0;
/////////////////////////////////////////////////////////////////////// KrazyXML_to_KrazyNetwork /////////////////////////////////////////////////////////////////////////////////////////////////////
#define LANE_NODE_NAME						string("KrazyLaneNode")
#define TRAFFICINDICATOR_NODE_NAME			string("KrazyTrafficIndicator")
#define TYPE_ATTRIB							string("Type")
#define DIRECTION_ATTRIB					string("Direction")
#define CONNECTED_LANES_ATTRIB				string("ConnectedLanes")
#define POINT_ATTRIB						string("Point")
#define RECT_ATTRIB							string("Rect")
#define ROTATION_ATTRIB                     string("RotationAngle")
#define PARENTINDEX_ATTRIB					string("ParentIndex")
#define CHILDINDEX_ATTRIB					string("ChildIndex")
#define PARENTINDEXCONTEXT_ATTRIB			string("ParentIndexContext")
#define SELFINDEXCONTEXT_ATTRIB				string("SelfIndexContext")
#define FLIPPOINT_ATTRIB                    string("FlipPoint")
#define VEHICALCOLORLIST_ATTRIB				string("VehicalColorList")
#define VEHICALTYPELIST_ATTRIB              string("VehicalTypeList")

#if 0
#define LANE_NODE_BORDER_COORDS				string("BorderCoords")
#endif
#define VEHICALNAMELIST_ATTRIB				string("VehicalNameList")
#define VEHICALSHADERLIST_ATTRIB			string("VehicalShaderList")
#define VEHICALSHADERTYPE_ATTRIB			string("VehicalShaderType")
#define ASSIGNEDLANE_FOR_ATTRIB				string("AssignedLanes_for_Sherrif")
////////////////////////////////////////////////////  KrazyNetworkCity ///////////////////////////////////////////////////////////////////////////////////
// appending..,
template<>
void KrazyNetworkCity_01Phase::appendConnectedNode(KrazyLaneNode* connected_node)
{
   _PTRConnectedNodes.push_back(connected_node);//remove this later..,
   _PTROwner->appendConnectedNode(connected_node);
}
/////////////////////////////////////////////////////////////////////////// KrazyXML_to_KrazyNetwork /////////////////////////////////////////////////////////////////////////////////////////////
//Later change into one file...,

//
//#define _KRAZY_GET_LIST_TO_FLOATLIST(list , float_list , ptr)\
//	if(ptr){\
//	string  str = ptr->get_Data();\
//	int count = str.size();\
//	string aux;\
//	KrazyFloatList floats;\
//	for(int i = 0; i < count; i++){\
//		char ch = str.at[i];\
//		switch(ch){\
//		case '[':break;\
//		case ']':\
//		{\
//			int index = list.size();\
//			list.push_back(KrazyFloatList());\
//			KrazyFloatList* ptr_floats = &list[index];\
//			int floatc = floats.size();\
//			for(int j = 0; j < floatc; j++){\
//				ptr_floats->push_back(floats[j]);\
//			}\
//			floats.clear();\
//		}/*ending case ']':*/\
//		break;\
//		case ',':\
//			floats.push_back(atof(aux.c_str()));\
//			aux.clear();\
//			break;\
//		default:aux.push_back(ch);break;\
//	}/*ending switch*/\
//	}/*ending for*/\
//	}/*ending if(ptr) */\
////

static void __getListOfFloatList(KrazyListToFloatList* ptr_list , KrazyXMLAttribute* ptr_attrib)
{
	if(ptr_attrib){
		string  str = ptr_attrib->get_Data();
		int count = str.size();
		string aux;
		KrazyFloatList floats;
		for(int i = 0; i < count; i++){
			char ch = str.at(i);
			switch(ch){
			case '[':break;
			case ']':
				{
					floats.push_back(atof(aux.c_str()));aux.clear();
					int index = ptr_list->size();
					ptr_list->push_back(KrazyFloatList());
					KrazyFloatList* ptr_floats = &ptr_list->at(index);
					int floatc = floats.size();
					for(int j = 0; j < floatc; j++){
						ptr_floats->push_back(floats[j]);
					}
					floats.clear();
				}/*ending case ']':*/
				break;
			case ',':
				floats.push_back(atof(aux.c_str()));aux.clear();
				break;
			default:aux.push_back(ch);break;
			}/*ending switch*/
		}/*ending for*/
}/*ending if(ptr) */

}
KrazyXML_to_KrazyNetwork::KrazyXML_to_KrazyNetwork(KrazyLevelCityContext* level_city_context) :
_PTRLevelCityContext(level_city_context)
{

}
//
KrazyXML_to_KrazyNetwork::~KrazyXML_to_KrazyNetwork()
{
	
}
//
void KrazyXML_to_KrazyNetwork::onExecute(KrazyXMLNode* ptr_xmlNode)
{
	KrazyNetworkDepthNode_Packing					_indexer_map;
	KrazyNetworkDepthIntToIndexer_Packing			_connected_indices; 
	KrazyMapLaneIndextoDestinationIndicator        _destinations_without_single_lane_parent;
	KrazyXMLNode* ptr_currentParent = ptr_xmlNode;
	int double_lane_road_count = 0;
	while(ptr_currentParent){
		if(!_indexer_map.find(ptr_currentParent)){
			_helper_indexer_network_* ptr_current_indexer = _indexer_map[ptr_currentParent];
			string name = ptr_currentParent->get_NodeName();
			int attrib_count = ptr_currentParent->get_Attributes_Count();
			if(attrib_count > 0){
				KrazyXMLAttribute* ptr_type  = ptr_currentParent->get_AttributeByTypeIfAny(TYPE_ATTRIB);
				KrazyXMLAttribute* ptr_dir   = ptr_currentParent->get_AttributeByTypeIfAny(DIRECTION_ATTRIB);
				KrazyXMLAttribute* ptr_conn  = ptr_currentParent->get_AttributeByTypeIfAny(CONNECTED_LANES_ATTRIB);
				KrazyXMLAttribute* ptr_point = ptr_currentParent->get_AttributeByTypeIfAny(POINT_ATTRIB);
				KrazyXMLAttribute* ptr_rect  = ptr_currentParent->get_AttributeByTypeIfAny(RECT_ATTRIB);
				int type = 0;_KRAZY_GET_INT(type,ptr_type); /*revisit:- May Be If time permits remove these macro funtionality into a class*/
				int dir  = 0;_KRAZY_GET_INT(dir,ptr_dir);
				KrazyFloatList point_indices_f;	_KRAZY_GET_FLOAT_INDICES_FROM_STRING(point_indices_f , ptr_point);
				KrazyFloatList rect_indices_f;	_KRAZY_GET_FLOAT_INDICES_FROM_STRING(rect_indices_f , ptr_rect);
				if( !name.compare(LANE_NODE_NAME) ){
					KrazyIntList   conn_indices_i;	_KRAZY_GET_INT_INDICES_FROM_STRING(conn_indices_i , ptr_conn);
					if(type > 0 && dir > 0){
						KrazyLaneProperties lane_propes;
						lane_propes.set_LaneType((KrazyLaneType)type);
						lane_propes.set_LaneDirection((KrazyLaneDirection)dir);
						lane_propes.set_RectByIndices(rect_indices_f);
						lane_propes.set_PointByIndices(point_indices_f);
						switch(type){
						case KRAZY_LANE_Type_Single:
							{
							KrazyXMLNode* ptr_parent = ptr_currentParent->get_PTRParent();
							if(_indexer_map.find(ptr_parent)){
								_helper_indexer_network_* p_indexer = _indexer_map[ptr_parent];
								KrazyLaneNode* lane_node = p_indexer->_PTRNetworkCityNode->getOwner();
								KrazyXMLAttribute* ptr_flip_points  = ptr_currentParent->get_AttributeByTypeIfAny(FLIPPOINT_ATTRIB);
								KrazyFloatList flip_points;_KRAZY_GET_FLOAT_INDICES_FROM_STRING(flip_points , ptr_flip_points);
								KrazyLaneNode* single_lane = lane_node->createKid();
								single_lane->setProperties(lane_propes);
								single_lane->setFlipTrafficIndicator(flip_points);
								ptr_current_indexer->_PTRNetworkCityNode = p_indexer->_PTRNetworkCityNode;
							}
							}
							break;
						case KRAZY_LANE_Type_Double:
							{
#if 0
							KrazyXMLAttribute* ptr_border_coords  = ptr_currentParent->get_AttributeByTypeIfAny(LANE_NODE_BORDER_COORDS);
							KrazyFloatList border_coords;_KRAZY_GET_FLOAT_INDICES_FROM_STRING(border_coords , ptr_border_coords);
							lane_propes.set_BorderCoords(border_coords);																
#endif
							KrazyNetworkCity_01Phase* network_city_node = _on_getNewNetworkNode();
							network_city_node->getOwner()->setProperties(lane_propes);
							ptr_current_indexer->_PTRNetworkCityNode = network_city_node;
							int count = conn_indices_i.size();
							bool push_conn(false);
							if(_connected_indices.find(double_lane_road_count) == _connected_indices.end()){
								push_conn = true;
								_connected_indices[double_lane_road_count] = ptr_current_indexer;
							}
							for(int i = 0; i < count;i++){
								int index = conn_indices_i[i];
								if(push_conn)
									ptr_current_indexer->_List.push_back(index);
								if(_connected_indices.find(index) != _connected_indices.end()){
									_helper_indexer_network_* already_connected_indexer = _connected_indices[index];
									int connected_count = already_connected_indexer->_List.size();
									for(int j = 0; j < connected_count;j++){
										if(already_connected_indexer->_List[j] == double_lane_road_count ){
											already_connected_indexer->_PTRNetworkCityNode->appendConnectedNode(ptr_current_indexer->_PTRNetworkCityNode->getOwner());
											ptr_current_indexer->_PTRNetworkCityNode->appendConnectedNode(already_connected_indexer->_PTRNetworkCityNode->getOwner());
										}
									}
								}
							}
							double_lane_road_count++;
							}
							break;
						}
					}
				}
				else if( !name.compare(TRAFFICINDICATOR_NODE_NAME) ){					
					KrazyXMLAttribute* ptr_rotation     = ptr_currentParent->get_AttributeByTypeIfAny(ROTATION_ATTRIB);
					float rotation   = 0.0f; _KRAZY_GET_FLOAT(rotation , ptr_rotation);
					if(type > 0){
						switch(type){						
						case KRAZY_TRAFFIC_INDC_Turn:
							{
							KrazyXMLNode* parent_node = ptr_currentParent->get_PTRParent();
							if(_indexer_map.find(parent_node)){
								_helper_indexer_network_* ptr_parent_node_indexer = _indexer_map[parent_node];
								KrazyLaneNode* double_node_lane = ptr_parent_node_indexer->_PTRNetworkCityNode->getOwner();
								if(double_node_lane->getProperties()->get_LaneType() == KRAZY_LANE_Type_Double){
									int single_lane_count = double_node_lane->getKidCount();
									if(single_lane_count > 0){
										KrazyLaneNode* single_node = double_node_lane->getLane(single_lane_count - 1);
										if(single_node){
											KrazyTrafficIndicator* traffic_indc = single_node->createTrafficIndicator();
											if(traffic_indc){
												traffic_indc->Type() = type;
												traffic_indc->RotationAngle() = rotation;
												traffic_indc->set_VectorByIndices(point_indices_f);
												traffic_indc->set_RectByIndices(rect_indices_f);
											}
										}
									}
								}
							}
							}
							break;
						case KRAZY_TRAFFIC_INDC_Dest:
						case KRAZY_TRAFFIC_INDC_Entry:
						case KRAZY_TRAFFIC_INDC_NoEntry:
						case KRAZY_TRAFFIC_INDC_Wait:
						case KRAZY_TRAFFIC_INDC_SherrifBay:
						case KRAZY_TRAFFIC_INDC_Exit:
							{
								int parent_index             = -1;
								int parent_single_lane_index = -1;
								int self_parent_index        = -1;
								int self_single_lane_index   = -1;
								int self_source_traffic_indc_index = -1;
							
								KrazyXMLAttribute* ptr_parent_index_context = ptr_currentParent->get_AttributeByTypeIfAny(PARENTINDEXCONTEXT_ATTRIB);
								KrazyXMLAttribute* ptr_self_index_context   = ptr_currentParent->get_AttributeByTypeIfAny(SELFINDEXCONTEXT_ATTRIB);
								KrazyXMLAttribute* ptr_vehical_color_list = ptr_currentParent->get_AttributeByTypeIfAny(VEHICALCOLORLIST_ATTRIB);
								KrazyXMLAttribute* ptr_vehical_name_list = ptr_currentParent->get_AttributeByTypeIfAny(VEHICALNAMELIST_ATTRIB);
								KrazyXMLAttribute* ptr_vehical_type_list  = ptr_currentParent->get_AttributeByTypeIfAny(VEHICALTYPELIST_ATTRIB);
								KrazyXMLAttribute* ptr_vehical_shader  = ptr_currentParent->get_AttributeByTypeIfAny(VEHICALSHADERLIST_ATTRIB);
								KrazyXMLAttribute* ptr_vehical_shadertype = ptr_currentParent->get_AttributeByTypeIfAny(VEHICALSHADERTYPE_ATTRIB);	
								KrazyXMLAttribute* ptr_assigned_lane_for_sherrif = ptr_currentParent->get_AttributeByTypeIfAny(ASSIGNEDLANE_FOR_ATTRIB);
								KrazyIntList vehical_shade_list;_KRAZY_GET_INT_INDICES_FROM_STRING(vehical_shade_list,ptr_vehical_shader);
								KrazyIntList vehical_shadetype_list;_KRAZY_GET_INT_INDICES_FROM_STRING(vehical_shadetype_list,ptr_vehical_shadertype);
								KrazyIntList parent_index_context_list;_KRAZY_GET_INT_INDICES_FROM_STRING(parent_index_context_list,ptr_parent_index_context);
								KrazyIntList self_index_context_list;_KRAZY_GET_INT_INDICES_FROM_STRING(self_index_context_list,ptr_self_index_context);
								KrazyIntList vehical_name_list;_KRAZY_GET_INT_INDICES_FROM_STRING(vehical_name_list,ptr_vehical_name_list);
								KrazyIntList assignedlane_for_sherrif_list;_KRAZY_GET_INT_INDICES_FROM_STRING(assignedlane_for_sherrif_list,ptr_assigned_lane_for_sherrif);
							
								KrazyListToFloatList vehical_color_list; __getListOfFloatList(&vehical_color_list,ptr_vehical_color_list);
								KrazyIntList vehical_type_list         ;_KRAZY_GET_INT_INDICES_FROM_STRING(vehical_type_list,ptr_vehical_type_list);
							
								if(parent_index_context_list.size() == 2){
									parent_index = parent_index_context_list[0];
									parent_single_lane_index = parent_index_context_list[1];
								}
								if(self_index_context_list.size() == 3){
									self_parent_index        = self_index_context_list[0];
									self_single_lane_index   = self_index_context_list[1];
									self_source_traffic_indc_index = self_index_context_list[2];
								}
								if(parent_index > -1){
									_unlinked_traffic_indc_memory_slot* ptr_mem_slot = new _unlinked_traffic_indc_memory_slot( 
																															rotation , type,
																															parent_index , parent_single_lane_index,
																															self_parent_index , self_single_lane_index,
																															self_source_traffic_indc_index , point_indices_f,			
																															rect_indices_f);

									KrazyTrafficIndcMemorySlotList* _mem_slot_list = 0;
									if(_destinations_without_single_lane_parent.find(parent_index) == _destinations_without_single_lane_parent.end()){
										_mem_slot_list = new KrazyTrafficIndcMemorySlotList;
										_destinations_without_single_lane_parent[parent_index] = _mem_slot_list;
									}
									else{
										_mem_slot_list = _destinations_without_single_lane_parent[parent_index];
									}
									//..,
									_mem_slot_list->push_back(ptr_mem_slot);
									ptr_mem_slot->on_setFloatArray(vehical_color_list);
									ptr_mem_slot->on_setIntList(vehical_type_list);
									ptr_mem_slot->on_setVehicalNameList(vehical_name_list);
									ptr_mem_slot->on_setVehicalShadeList(vehical_shade_list);
									ptr_mem_slot->on_setVehicalShadeTypeList(vehical_shadetype_list);
									ptr_mem_slot->on_setAssignedLaneList_for_Sherrif(assignedlane_for_sherrif_list);
								}
							}
							break;
						}				
					}
				}
			}
			ptr_current_indexer->_Size = ptr_currentParent->get_Kids_Count();
			if(ptr_current_indexer->_Size == 0){
				ptr_currentParent = ptr_currentParent->get_PTRParent();
				continue;
			}
			ptr_currentParent = ptr_currentParent->get_KidsNode(ptr_current_indexer->_Cursor++);
			continue;		   
		}
		// simulating recursion for whatever current parent is in existance..,
		_helper_indexer* ptr_indexer = _indexer_map[ptr_currentParent];
		if(ptr_indexer->_Size > 0 && ptr_indexer->_Cursor < ptr_indexer->_Size){
			ptr_currentParent = ptr_currentParent->get_KidsNode(ptr_indexer->_Cursor++);
		}
		else if(ptr_indexer->_Size > 0 && ptr_indexer->_Size == ptr_indexer->_Cursor){
			ptr_currentParent = ptr_currentParent->get_PTRParent();
		}
	}
	KrazyParentFinderPolicyForKrazyTrafficIndicator _dest_indc_operation_policy(&_destinations_without_single_lane_parent,_PTRLevelCityContext);
	KrazyNetworkCity_01Phase::onOperateOnNodeCacheDataByPolicy<KrazyParentFinderPolicyForKrazyTrafficIndicator>(&_dest_indc_operation_policy);	
}
/////////////////////////////////////////////////////////////////////////// KrazyParentFinderPolicy_DestinationIndc /////////////////////////////////////////////////////////////////////////////////////////////////
void KrazyParentFinderPolicyForKrazyTrafficIndicator::onExecute(vector<KrazyNetworkCity_01Phase*>* network)
{
	int count = network->size();
	for(int i = 0; i < count; i++){
		KrazyLaneNode* ptr_parent_node_double = network->at(i)->getOwner();
		KrazyLaneProperties* ptr_parent_prop  = ptr_parent_node_double->getProperties();
		KrazyLaneType laneType = ptr_parent_prop->get_LaneType();
		if(_PTRIndc_map->find(i) != _PTRIndc_map->end()){
			KrazyTrafficIndcMemorySlotList* _ptr_slot_list = (*_PTRIndc_map)[i];//->at(i);
			int slots = _ptr_slot_list->size();
			for(int slot = 0; slot < slots; slot++){
				_unlinked_traffic_indc_memory_slot* ptr_dest_mem_slot = _ptr_slot_list->at(slot);
				KrazyLaneNode* ptr_parent_node_double = network->at(i)->getOwner();
				KrazyLaneProperties* ptr_parent_prop  = ptr_parent_node_double->getProperties();
				KrazyLaneType laneType = ptr_parent_prop->get_LaneType();
				switch(laneType){
				case KRAZY_LANE_Type_Double:{
					KrazyTrafficIndicator* ptr_new_indc = 0;
					switch(ptr_dest_mem_slot->_type){
					case KRAZY_TRAFFIC_INDC_Entry:
						{
						    int child_index = ptr_dest_mem_slot->_parent_single_lane_index;
							int child_count = ptr_parent_node_double->getKidCount();
							if(child_index > -1 && child_count == 2 && child_index < child_count){
								ptr_new_indc =  KrazyEntryIndicatorCache::allocate();
								KrazyEntranceContext* ptr_ent_ctxt = ptr_new_indc->get_EntranceContext();
								ptr_ent_ctxt->on_setShadeList(ptr_dest_mem_slot->_vehical_shade_list);
								ptr_ent_ctxt->on_setShadeTypeList(ptr_dest_mem_slot->_vehical_shadetype_list);
								ptr_ent_ctxt->on_setTypeList(ptr_dest_mem_slot->_int_list);
								ptr_ent_ctxt->on_setNameList(ptr_dest_mem_slot->_vehical_name_list);
								KrazyLaneNode* ptr_single_lane = ptr_parent_node_double->getLane(child_index);
								ptr_new_indc->set_ParentLaneNode(ptr_single_lane);
							}
						}
						break;
					case KRAZY_TRAFFIC_INDC_Dest:
						{
							int parent_index = ptr_dest_mem_slot->_parent_index;
							if(parent_index > -1 && _PTRIndc_map->find(parent_index) != _PTRIndc_map->end()){
								KrazyLaneNode* ptr_parent_double_lane = network->at(parent_index)->getOwner();
								KrazyLaneNode* ptr_single_lane_node  = ptr_parent_double_lane->getLane(ptr_dest_mem_slot->_parent_single_lane_index);
								ptr_new_indc = ptr_single_lane_node->createDestinationIndicator();
								ptr_new_indc->set_ParentLaneNode(ptr_single_lane_node);
							}
							if(ptr_new_indc){
								int self_parent_index = ptr_dest_mem_slot->_self_parent_index;
								if(self_parent_index > -1 && _PTRIndc_map->find(self_parent_index) != _PTRIndc_map->end()){
									KrazyLaneNode* ptr_parent_double_lane = network->at(self_parent_index)->getOwner();
								    KrazyLaneNode* ptr_single_lane_node  = ptr_parent_double_lane->getLane(ptr_dest_mem_slot->_self_single_lane_index);
									KrazyTrafficIndicator* ptr_traffic_indc = ptr_single_lane_node->get_TrafficIndicator(ptr_dest_mem_slot->_self_source_traffic_indc_index);
									ptr_traffic_indc->add_DestinationIndc(ptr_new_indc);
									ptr_new_indc->set_Source(ptr_traffic_indc);
								}
							}
						}
						break;
					case KRAZY_TRAFFIC_INDC_NoEntry:
					case KRAZY_TRAFFIC_INDC_Wait:
					case KRAZY_TRAFFIC_INDC_Exit:
						{
							int child_index = ptr_dest_mem_slot->_parent_single_lane_index;
							int child_count = ptr_parent_node_double->getKidCount();
							if(child_index > -1 && child_count == 2 && child_index < child_count){
								KrazyLaneNode* ptr_single_lane = ptr_parent_node_double->getLane(child_index);
								ptr_new_indc = ptr_single_lane->createTrafficIndicator();
								ptr_new_indc->set_ParentLaneNode(ptr_single_lane);
							}
						}
						break;
					case KRAZY_TRAFFIC_INDC_SherrifBay:
						{
							int child_index = ptr_dest_mem_slot->_parent_single_lane_index;
							int child_count = ptr_parent_node_double->getKidCount();
							if(child_index > -1 && child_count == 2 && child_index < child_count){
								KrazyLaneNode* ptr_single_lane = ptr_parent_node_double->getLane(child_index);
								ptr_single_lane->createSherrifBay();
								ptr_single_lane->get_sherrifBay()->set_assignedLanes_for_Sherrif(ptr_dest_mem_slot->_assigned_lane_for_sherrif);
							}
						}
						break;
					}
					if(ptr_new_indc){
						ptr_new_indc->Type() = ptr_dest_mem_slot->_type;
						ptr_new_indc->RotationAngle() = ptr_dest_mem_slot->_rotation_angle;
						ptr_new_indc->set_VectorByIndices(ptr_dest_mem_slot->_point);
						ptr_new_indc->set_RectByIndices(ptr_dest_mem_slot->_rect);
					}
				}break;
				}
		}
		}
	}

	for(int i = 0; i < count; i++){
		KrazyLaneNode* ptr_node_double_lane = network->at(i)->getOwner();
		int single_lane_count = ptr_node_double_lane->getKidCount();
		for(int i = 0; i < single_lane_count; i++){
			ptr_node_double_lane->getLane(i)->sort_allTrafficIndicators();
		}
	}
}
/////////////////////////////////////////////////////////////////// KrazyDisplayTrafficIndicators //////////////////////////////////////////////////
#ifdef _IS_FOR_SIMONE
KrazyDisplayTrafficIndicators::KrazyDisplayTrafficIndicators(unsigned int position , unsigned int color) :
 _Position(position)
,_Color(color)
{
}
////////////////////////////////////////////////////
void KrazyDisplayTrafficIndicators::onExecute(vector<KrazyNetworkCity_01Phase*>* ptr_network)
{
#if 0
	int count = ptr_network->size();
	for(int i = 0; i < count; i++){	
		KrazyNetworkCity_01Phase* ptr_network_node = ptr_network->at(i);
		KrazyLaneNode* ptr_lane_node = ptr_network_node->getOwner();
		int kid_count = ptr_lane_node->getKidCount();
		for(int j = 0; j < kid_count; j++){
			KrazyLaneNode* ptr_single_lane = ptr_lane_node->getLane(j);
			KrazyTrafficIndicatorList* ptr_traffic_indc_list = ptr_single_lane->get_TrafficIndicators();
			int traffic_indc_count = ptr_traffic_indc_list->size();
			for(int traffic_indc = 0; traffic_indc < traffic_indc_count; traffic_indc++){
				KrazyTrafficIndicator* ptr_traffic_indc =  ptr_traffic_indc_list->at(traffic_indc);
				if(ptr_traffic_indc->Type() > 1) continue;
				float* rendering_triangles = ptr_traffic_indc->get_RenderingTriangle()->data();
				if(rendering_triangles){
					glVertexAttribPointer(_Position, 4, GL_FLOAT, GL_FALSE, 0, rendering_triangles);
					glEnableVertexAttribArray(_Position);
					glUniform4fv(_Color, 1 , COLOR_BLUE);
					glDrawArrays(GL_TRIANGLES, 0, 6);
					glDisableVertexAttribArray(_Position);
				}
				int dest_indc_count = ptr_traffic_indc->get_DestinationIndcCount();
				for(int dest_indc = 0; dest_indc < dest_indc_count; dest_indc++){
					KrazyTrafficIndicator* ptr_dest_indc = ptr_traffic_indc->get_DestinationIndc(dest_indc);
					rendering_triangles = ptr_dest_indc->get_RenderingTriangle()->data();
					if(rendering_triangles){
						glVertexAttribPointer(_Position, 4, GL_FLOAT, GL_FALSE, 0, rendering_triangles);
						glEnableVertexAttribArray(_Position);
						glUniform4fv(_Color, 1 , COLOR_RED);
						glDrawArrays(GL_TRIANGLES, 0, 6);
						glDisableVertexAttribArray(_Position);
					}
				}
			}
		}
	}
#endif
}
/////////////////////////////////////////////////////////////////////////////////////
#endif

