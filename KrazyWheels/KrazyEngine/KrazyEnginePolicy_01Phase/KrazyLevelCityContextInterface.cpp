#include "stdafx.h"
#include "KrazyLevelCityContextInterface.h"
#include "KrazyLevelCityNetwork.h"
#include "KrazyLevelThriver.h"
#include "KrazyPlayToolRect.h"
#include "KrazyLevelRectsPlayTool.h"
#include "KrazyTutorialThriver.h"
#include "../KrazyLevelXML/KrazyLevelXML_Demo.h"
#include "../KrazyLevelXML/KrazyLevelXML_Tutorial_1.h"
#include "../KrazyLevelXML/KrazyLevelXML_1_and_2_Tutorial.h"
#include "../KrazyLevelXML/KrazyLevelXML_3_Tutorial.h"
#include "../KrazyLevelXML/KrazyLevelXML_4_Tutorial.h"
#include "../KrazyLevelXML/KrazyLevelXML_5_Tutorial.h"
#include "../KrazyLevelXML/KrazyLevel_1_XML.h"
#include "../KrazyLevelXML/KrazyLevel_2_XML.h"
#include "../KrazyLevelXML/KrazyLevel_3_XML.h"
#include "../KrazyLevelXML/KrazyLevel_4_XML.h"
#include "../KrazyLevelXML/KrazyLevel_5_XML.h"
#include "../KrazyLevelXML/KrazyLevel_6_XML.h"
#include "../KrazyLevelXML/KrazyLevel_7_XML.h"
//..,
#if 0
#ifndef _KrazyTutorials
#define _KrazyTutorials
#endif
#endif
//..,
#define _KRAZY_EXIT_SEQUENCE_1     string("Rangtoona")
#define _KRAZY_EXIT_SEQUENCE_2     string("Bangtoona , Rangtoona")
#define _KRAZY_EXIT_SEQUENCE_3     string("Ganguni")
#define _KRAZY_EXIT_SEQUENCE_4     string("Ganguni , Bangtoona , Rangtoona")
#define _KRAZY_EXIT_SEQUENCE_5     string("Ganguni , Bangtoona")
//..,
KrazyLevelCityContext::KrazyLevelCityContext() 
{

}
//
KrazyLevelCityContext::~KrazyLevelCityContext()
{

}
//
void KrazyLevelCityContext::clean_up()
{
	_HorzsRoadBuffer.clear();
	_HorzsBorderBuffer.clear();
	_VertsRoadBuffer.clear();
	_VertsBorderBuffer.clear();
	_BlockingTI.clear();
	_XML.clear();
}
//..,
void KrazyLevelCityContext::on_refresh_level_onli(int level)
{
	clean_up();
	KrazyLevelThriver::get_default()->reset();
	KrazyPlayToolRect::get_default()->reset();
	switch(level)
	{
		case 0		: 
			{
#if 0
			_XML = gKrazyLevelXML_Demo;
#endif
	//..,
#if 0
			 KrazyFloatList _vectors;
		    _KRAZY_COPY_LIST_PTR(_play_tool_vectors_1 , _vectors , 4);
	         KrazyPlayToolRect::get_default()->onSetVectors(_vectors);
			_XML = gKrazy_Test_2_c;
#endif
	//..,
#if 0
			 KrazyFloatList _vectors;
		    _KRAZY_COPY_LIST_PTR(_play_tool_vectors_1 , _vectors , 4);
	         KrazyPlayToolRect::get_default()->onSetVectors(_vectors);
			_XML = gKrazy_Test_3_c;
#endif
	//..,
#if 0
			KrazyFloatList _vectors;
		    _KRAZY_COPY_LIST_PTR(_play_tool_vectors_1 , _vectors , 4);
	         KrazyPlayToolRect::get_default()->onSetVectors(_vectors);
			_XML = gKrazy_Test_4_c;
#endif
	//..,
#if 1
			KrazyFloatList _vectors;
		    _KRAZY_COPY_LIST_PTR(_play_tool_vectors_1 , _vectors , 4);
	         KrazyPlayToolRect::get_default()->onSetVectors(_vectors);
			_XML = gKrazy_Test_8_c;
#endif

			}
			break;
#if 0
		case 1		:  onLevel_1();		break;
		case 2		:  onLevel_2();		break;
		case 3		:  onLevel_3();		break;
		case 4		:  onLevel_4();		break;
		case 5		:  onLevel_5();		break;
		case 6		:  onLevel_6();		break;
		case 7		:  onLevel_7();		break;
		case 8		:  onLevel_8();		break;
		case 9		:  onLevel_9();		break;
		case 10		:  onLevel_10();	break;
		case 11		:  onLevel_11();	break;
		case 12		:  onLevel_12();	break;
		case 13		:  onLevel_13();	break;
		case 14		:  onLevel_14();	break;
		case 15		:  onLevel_15();	break;
#endif 
	}
	//..,
	if(!_XML.empty()){
		KrazyNetworkCity_01Phase::loadNetworkFromXML<KrazyXML_to_KrazyNetwork>(_XML , this);
		setLayoutVertices();
	}
}
//..,
void KrazyLevelCityContext::on_refresh(int level)
{
	clean_up();
	KrazyLevelThriver::get_default()->reset();
	KrazyPlayToolRect::get_default()->reset();
	switch(level)
	{
		case 0		: 
			{
#if 0
			_XML = gKrazyLevelXML_Demo;
#endif
	//..,
#if 0
			 KrazyFloatList _vectors;
		    _KRAZY_COPY_LIST_PTR(_play_tool_vectors_1 , _vectors , 4);
	         KrazyPlayToolRect::get_default()->onSetVectors(_vectors);
			_XML = gKrazy_Test_2_c;
#endif
	//..,
#if 0
			 KrazyFloatList _vectors;
		    _KRAZY_COPY_LIST_PTR(_play_tool_vectors_1 , _vectors , 4);
	         KrazyPlayToolRect::get_default()->onSetVectors(_vectors);
			_XML = gKrazy_Test_3_c;
#endif
	//..,
#if 0
			KrazyFloatList _vectors;
		    _KRAZY_COPY_LIST_PTR(_play_tool_vectors_1 , _vectors , 4);
	         KrazyPlayToolRect::get_default()->onSetVectors(_vectors);
			_XML = gKrazy_Test_4_c;
#endif
	//..,
#if 1
			KrazyFloatList _vectors;
		    _KRAZY_COPY_LIST_PTR(_play_tool_vectors_1 , _vectors , 4);
	         KrazyPlayToolRect::get_default()->onSetVectors(_vectors);
			_XML = gKrazy_Test_8_c;
#endif

			}
			break;

		case 1		:  onLevel_1();		break;
		case 2		:  onLevel_2();		break;
		case 3		:  onLevel_3();		break;
		case 4		:  onLevel_4();		break;
		case 5		:  onLevel_5();		break;
		case 6		:  onLevel_6();		break;
		case 7		:  onLevel_7();		break;
#if 0
		case 8		:  onLevel_8();		break;
		case 9		:  onLevel_9();		break;
		case 10		:  onLevel_10();	break;
		case 11		:  onLevel_11();	break;
		case 12		:  onLevel_12();	break;
		case 13		:  onLevel_13();	break;
		case 14		:  onLevel_14();	break;
		case 15		:  onLevel_15();	break;
		case 16     :  onLevel_16();    break;
#endif
	}
	//..,
	if(!_XML.empty()){
		KrazyNetworkCity_01Phase::loadNetworkFromXML<KrazyXML_to_KrazyNetwork>(_XML , this);
		setLayoutVertices();
		KrazyNetworkCity_01Phase::onRefreshLayoutRendrer<KrazyLevelCityContext>(this);
	}
}
//
 #ifdef _ENABLE_SPLIT_SCALED_LANE
void KrazyLevelCityContext::setLayoutVertices()
 {
	KrazyNetworkCity_01Phase* ptr_parent_network = KrazyNetworkCity_01Phase::getNetwork();
	vector<KrazyWheelNetwork_01Phase*>* ptr_network_nodes = ptr_parent_network->getNodeCache();
	int count = ptr_network_nodes->size();
	for(int i = 0; i < count; i++){
		KrazyLaneNode* lane = ((KrazyNetworkCity_01Phase*)ptr_network_nodes->at(i))->getOwner();
		KrazyLaneProperties* _lane_props = lane->getProperties();
		KrazyLaneDirection _lane_direction = _lane_props->get_LaneDirection();
		KrazyFloatList& vertex_coords = _lane_props->get_VertexCoords();
		int verts = vertex_coords.size();
		for(int vert = 0; vert < verts; vert++){
			switch(_lane_direction){
			case KRAZY_LANE_DIRECTION_HORZS	 : _HorzsRoadBuffer.push_back(vertex_coords[vert]); break;
			case KRAZY_LANE_DIRECTION_VERTS	 : _VertsRoadBuffer.push_back(vertex_coords[vert]); break;
			}
			
		}
		KrazyFloatList& border_coords = _lane_props->get_BorderCoords();
		int borders = border_coords.size();
		for(int border = 0; border < borders; border++){
			switch(_lane_direction){
			case KRAZY_LANE_DIRECTION_HORZS	 : _HorzsBorderBuffer.push_back(border_coords[border]); break;
			case KRAZY_LANE_DIRECTION_VERTS	 : _VertsBorderBuffer.push_back(border_coords[border]); break;
			}
			
		}
		int kids = 2;
		for(int kid = 0; kid < kids; kid++){
			KrazyLaneNode* lane_kid = lane->getLane(kid);
			int indicators = lane_kid->get_TrafficIndicatorCount();
#if 0
			KrazyTrafficIndicator* _sherrif_bay = lane_kid->get_sherrifBay();
			if(_sherrif_bay)
				_BlockingTI.push_back(_sherrif_bay);
#endif

			for(int indicator = 0; indicator < indicators; indicator++)
			{
				KrazyTrafficIndicator* _indicator = lane_kid->get_TrafficIndicator(indicator);
				switch(_indicator->Type())
				{
				case KRAZY_TRAFFIC_INDC_NoEntry:
				case KRAZY_TRAFFIC_INDC_Wait:
				case KRAZY_TRAFFIC_INDC_Exit:
					{
						KrazyLaneProperties* lane_kid_property = lane_kid->getProperties();
						glm::vec2* vec = _indicator->Vector();
						float x1(0),y1(0);float x2(0),y2(0);
						lane_kid_property->get_Rect( x1 , y1 , x2 , y2 );
						switch(lane_kid_property->get_LaneDirection())
						{
						case KRAZY_LANE_DIRECTION_UP_HORZS    : 
						case KRAZY_LANE_DIRECTION_DOWN_HORZS  : 
							{
								glm::vec2 vec_1(x1,y1);	glm::vec2 vec_2(x1,y2);
								float distance_p = abs(glm::distance(vec_1 , vec_2)/2.0f);
								vec->y = y2 + distance_p;
							}
							break;
						case KRAZY_LANE_DIRECTION_UP_VERTS	  : 
						case KRAZY_LANE_DIRECTION_DOWN_VERTS  : 
							{
								glm::vec2 vec_1(x1,y1);	glm::vec2 vec_2(x2,y1);
								float distance_p = abs(glm::distance(vec_1 , vec_2)/2.0f);
								vec->x = x1 + distance_p;
							}
							break;
						}
						_BlockingTI.push_back(_indicator);
					}
					 break;
				}
			}
		}
	}
 }
#else
 void KrazyLevelCityContext::setLayoutVertices()
 {
	KrazyNetworkCity_01Phase* ptr_parent_network = KrazyNetworkCity_01Phase::getNetwork();
	vector<KrazyWheelNetwork_01Phase*>* ptr_network_nodes = ptr_parent_network->getNodeCache();
	int count = ptr_network_nodes->size();
	for(int i = 0; i < count; i++)
	{
		KrazyLaneNode* lane = ((KrazyNetworkCity_01Phase*)ptr_network_nodes->at(i))->getOwner();
		KrazyFloatList& vertex_coords = lane->getProperties()->get_VertexCoords();
		int verts = vertex_coords.size();
		for(int vert = 0; vert < verts; vert++)
		{
			_RoadBuffer.push_back(vertex_coords[vert]);
		}
		KrazyFloatList& border_coords = lane->getProperties()->get_BorderCoords();
		int borders = border_coords.size();
		for(int border = 0; border < borders; border++)
		{
			_BorderBuffer.push_back(border_coords[border]);
		}
		int kids = 2;
		for(int kid = 0; kid < kids; kid++){
			KrazyLaneNode* lane_kid = lane->getLane(kid);
			int indicators = lane_kid->get_TrafficIndicatorCount();
			KrazyTrafficIndicator* _sherrif_bay = lane_kid->get_sherrifBay();
			if(_sherrif_bay)
				_BlockingTI.push_back(_sherrif_bay);

			for(int indicator = 0; indicator < indicators; indicator++)
			{
				KrazyTrafficIndicator* _indicator = lane_kid->get_TrafficIndicator(indicator);
				switch(_indicator->Type())
				{
				case KRAZY_TRAFFIC_INDC_NoEntry:
				case KRAZY_TRAFFIC_INDC_Wait:
				case KRAZY_TRAFFIC_INDC_Exit:
					{
						KrazyLaneProperties* lane_kid_property = lane_kid->getProperties();
						glm::vec2* vec = _indicator->Vector();
						float x1(0),y1(0);float x2(0),y2(0);
						lane_kid_property->get_Rect( x1 , y1 , x2 , y2 );
						switch(lane_kid_property->get_LaneDirection())
						{
						case KRAZY_LANE_DIRECTION_UP_HORZS    : 
						case KRAZY_LANE_DIRECTION_DOWN_HORZS  : 
							{
								glm::vec2 vec_1(x1,y1);	glm::vec2 vec_2(x1,y2);
								float distance_p = abs(glm::distance(vec_1 , vec_2)/2.0f);
								vec->y = y2 + distance_p;
							}
							break;
						case KRAZY_LANE_DIRECTION_UP_VERTS	  : 
						case KRAZY_LANE_DIRECTION_DOWN_VERTS  : 
							{
								glm::vec2 vec_1(x1,y1);	glm::vec2 vec_2(x2,y1);
								float distance_p = abs(glm::distance(vec_1 , vec_2)/2.0f);
								vec->x = x1 + distance_p;
							}
							break;
						}
						_BlockingTI.push_back(_indicator);
					}
					 break;
				}
			}
		}
	}
 }
#endif
 //
 void KrazyLevelCityContext::onSetHorzsVertexList(KrazyFloatList* _list)
 {
	_list->clear(); int _count = _HorzsRoadBuffer.size();
	for(int i = 0; i < _count; i++){
		_list->push_back(_HorzsRoadBuffer[i]);
	}
 }
 //
 void KrazyLevelCityContext::onSetHorzsBorderList(KrazyFloatList* _list)
 {
	 _list->clear();int _count = _HorzsBorderBuffer.size();
	 for(int i = 0; i < _count; i++){
		 _list->push_back(_HorzsBorderBuffer[i]);
	}
 }
 //
void KrazyLevelCityContext::onSetVertsVertexList(KrazyFloatList* _list) 
{
	_list->clear();int _count = _VertsRoadBuffer.size();
	for(int i = 0; i < _count; i++){
		 _list->push_back(_VertsRoadBuffer[i]);
	}
}
//
void KrazyLevelCityContext::onSetVertsBorderList(KrazyFloatList* _list) 
{
	_list->clear();int _count = _VertsBorderBuffer.size();
	for(int i = 0; i < _count; i++){
		 _list->push_back(_VertsBorderBuffer[i]);
	}
}
//
 void KrazyLevelCityContext::on_set_blockingTI(KrazyTrafficIndicatorList* _list)
 {
	 _list->clear();int blocks = _BlockingTI.size();
	 for(int block = 0; block < blocks; block++){
		 _list->push_back(_BlockingTI[block]);
	 }
 }
 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _KrazyTutorials
 void KrazyLevelCityContext::onLevel_1()
 {
	 // Tutorial Mode ...........,
	 KrazyFloatList _vectors;
	 _KRAZY_COPY_LIST_PTR(_play_tool_vectors_2 , _vectors , 4);
	 KrazyPlayToolRect::get_default()->onSetVectors(_vectors);
	 //..,
	_XML = gKrazyLevelXML_1_Tutorial;//gKrazyLevelTut_1_XML;
	 KrazyThriving* _thriving = new KrazyThriving;
	 _thriving->vehical_name() = KRAZY_VEHICAL_in_GAMEPLAY_RangToona;
	 KrazyLevelThriver::get_default()->on_appendThriving(_thriving);
	 KrazyLevelThriver::get_default()->exit_sequencetext() = _KRAZY_EXIT_SEQUENCE_1;
	 KrazyTutorialThriver::get_default()->level() = 1;
 }
 //..,
 void KrazyLevelCityContext::onLevel_2()
 {
	 // Tutorial Mode ...........,
	 KrazyFloatList _vectors;
	 _KRAZY_COPY_LIST_PTR(_play_tool_vectors_2 , _vectors , 4);
	 KrazyPlayToolRect::get_default()->onSetVectors(_vectors);
	 //..,
	_XML = gKrazyLevelXML_2_Tutorial;//gKrazyLevelTut_1_XML;
	 KrazyThriving* _thriving = new KrazyThriving;
	 _thriving->vehical_name() = KRAZY_VEHICAL_in_GAMEPLAY_BangToona;
	 KrazyLevelThriver::get_default()->on_appendThriving(_thriving);
	 KrazyLevelThriver::get_default()->exit_sequencetext() = _KRAZY_EXIT_SEQUENCE_2;
	 KrazyTutorialThriver::get_default()->level() = 2;
 }
 //..,
 void KrazyLevelCityContext::onLevel_3()
 {
	 KrazyFloatList _vectors;
	 _KRAZY_COPY_LIST_PTR(_play_tool_vectors_1 , _vectors , 4);
	 KrazyPlayToolRect::get_default()->onSetVectors(_vectors);
	 //..,
	 _XML = gKrazyLevel_1_XML;
	 KrazyThriving* _thriving = new KrazyThriving;
	 _thriving->vehical_name() = KRAZY_VEHICAL_in_GAMEPLAY_RangToona;
	 KrazyLevelThriver::get_default()->on_appendThriving(_thriving);
	 KrazyLevelThriver::get_default()->exit_sequencetext() = _KRAZY_EXIT_SEQUENCE_1;
 }
 //
 void KrazyLevelCityContext::onLevel_4()
 {
	  KrazyFloatList _vectors;
	 _KRAZY_COPY_LIST_PTR(_play_tool_vectors_2 , _vectors , 4);
	 KrazyPlayToolRect::get_default()->onSetVectors(_vectors);
	 //..,
	 _XML = gKrazyLevel_2_XML;
	 KrazyThriving* _thriving = new KrazyThriving;
	 _thriving->vehical_name() = KRAZY_VEHICAL_in_GAMEPLAY_BangToona;
	 KrazyLevelThriver::get_default()->on_appendThriving(_thriving);
	  KrazyLevelThriver::get_default()->exit_sequencetext() = _KRAZY_EXIT_SEQUENCE_2;
 }
 //
 void KrazyLevelCityContext::onLevel_5()
 {
	 KrazyFloatList _vectors;
	 _KRAZY_COPY_LIST_PTR(_play_tool_vectors_2 , _vectors , 4);
	 KrazyPlayToolRect::get_default()->onSetVectors(_vectors);
	 //..,
	 _XML = gKrazyLevel_3_XML;
	 KrazyThriving* _thriving = new KrazyThriving;
	 _thriving->vehical_name() = KRAZY_VEHICAL_in_GAMEPLAY_BangToona;
	 KrazyLevelThriver::get_default()->on_appendThriving(_thriving);
	 KrazyLevelThriver::get_default()->exit_sequencetext() = _KRAZY_EXIT_SEQUENCE_2;
 }
 //
 void KrazyLevelCityContext::onLevel_6()
 {
	  // Tutorial Mode ...........,
	 KrazyFloatList _vectors;
	 _KRAZY_COPY_LIST_PTR(_play_tool_vectors_3 , _vectors , 4);
	 KrazyPlayToolRect::get_default()->onSetVectors(_vectors);
	 //..,
	 _XML = gKrazyLevelXML_3_Tutorial;
	 KrazyThriving* _thriving = new KrazyThriving;
	 _thriving->vehical_name() = KRAZY_VEHICAL_in_GAMEPLAY_Ganungi;
	 _thriving->wait_further() = KRAZY_BOOL_False;
	 KrazyLevelThriver::get_default()->on_appendThriving(_thriving);
	 KrazyLevelThriver::get_default()->exit_sequencetext() = _KRAZY_EXIT_SEQUENCE_3;
	 KrazyTutorialThriver::get_default()->level() = 6;
 }
 //
 void KrazyLevelCityContext::onLevel_7()
 {
	  KrazyFloatList _vectors;
	 _KRAZY_COPY_LIST_PTR(_play_tool_vectors_3 , _vectors , 4);
	 KrazyPlayToolRect::get_default()->onSetVectors(_vectors);
	 //..,
	 _XML = gKrazyLevel_4_XML;
	 KrazyThriving* _thriving = new KrazyThriving;
	 _thriving->vehical_name() = KRAZY_VEHICAL_in_GAMEPLAY_Ganungi;
	 _thriving->wait_further() = KRAZY_BOOL_False;
	 KrazyLevelThriver::get_default()->on_appendThriving(_thriving);
	 KrazyLevelThriver::get_default()->exit_sequencetext() = _KRAZY_EXIT_SEQUENCE_3;
 }
 //
  void KrazyLevelCityContext::onLevel_8()
 {
	  // Tutorial Mode..,
	  KrazyFloatList _vectors;
	 _KRAZY_COPY_LIST_PTR(_play_tool_vectors_3 , _vectors , 4);
	 KrazyPlayToolRect::get_default()->onSetVectors(_vectors);
	 //..,
	 _XML = gKrazyLevelXML_4_Tutorial;
	 KrazyThriving* _thriving = new KrazyThriving;
	 _thriving->vehical_name() = KRAZY_VEHICAL_in_GAMEPLAY_Ganungi;
	 _thriving->wait_further_for_Rangtoona() = KRAZY_BOOL_False;
	 KrazyLevelThriver::get_default()->on_appendThriving(_thriving);
	 KrazyLevelThriver::get_default()->exit_sequencetext() = _KRAZY_EXIT_SEQUENCE_5;
	 KrazyTutorialThriver::get_default()->level() = 8;
 }
 //
 void KrazyLevelCityContext::onLevel_9()
 {
	 // Tutorial Mode..,
	  KrazyFloatList _vectors;
	 _KRAZY_COPY_LIST_PTR(_play_tool_vectors_3 , _vectors , 4);
	 KrazyPlayToolRect::get_default()->onSetVectors(_vectors);
	 //..,
	 _XML = gKrazyLevelXML_4_Tutorial;
	 KrazyThriving* _thriving = new KrazyThriving;
	 _thriving->vehical_name() = KRAZY_VEHICAL_in_GAMEPLAY_Ganungi;
	 _thriving->wait_further_for_Rangtoona() = KRAZY_BOOL_False;
	 KrazyLevelThriver::get_default()->on_appendThriving(_thriving);
	 KrazyLevelThriver::get_default()->exit_sequencetext() = _KRAZY_EXIT_SEQUENCE_5;
	 KrazyTutorialThriver::get_default()->level() = 9;
 }
 //
 void KrazyLevelCityContext::onLevel_10()
 {
	  KrazyFloatList _vectors;
	 _KRAZY_COPY_LIST_PTR(_play_tool_vectors_4 , _vectors , 4);
	 KrazyPlayToolRect::get_default()->onSetVectors(_vectors);
	 //..,
	 _XML = gKrazyLevel_5_XML;
	 KrazyThriving* _thriving = new KrazyThriving;
	 _thriving->vehical_name() = KRAZY_VEHICAL_in_GAMEPLAY_BangToona;
	 KrazyLevelThriver::get_default()->on_appendThriving(_thriving);
	 KrazyLevelThriver::get_default()->exit_sequencetext() = _KRAZY_EXIT_SEQUENCE_2;
 }
 //
  void KrazyLevelCityContext::onLevel_11()
 {
	 //Tutorial Mode..,
	  KrazyFloatList _vectors;
	 _KRAZY_COPY_LIST_PTR(_play_tool_vectors_3 , _vectors , 4);
	 KrazyPlayToolRect::get_default()->onSetVectors(_vectors);
	 //..,
	 _XML = gKrazyLevelXML_4_Tutorial_c;
	 KrazyThriving* _thriving = new KrazyThriving;
	 _thriving->vehical_name() = KRAZY_VEHICAL_in_GAMEPLAY_Ganungi;
	 KrazyLevelThriver::get_default()->on_appendThriving(_thriving);
	 KrazyLevelThriver::get_default()->exit_sequencetext() = _KRAZY_EXIT_SEQUENCE_4;
	 KrazyTutorialThriver::get_default()->level() = 11;
 }
  //
 void KrazyLevelCityContext::onLevel_12()
 {
	 KrazyFloatList _vectors;
	 _KRAZY_COPY_LIST_PTR(_play_tool_vectors_4 , _vectors , 4);
	 KrazyPlayToolRect::get_default()->onSetVectors(_vectors);
	 //..,
	 _XML = gKrazyLevelXML_4_Tutorial_b;
	 KrazyThriving* _thriving = new KrazyThriving;
	 _thriving->vehical_name() = KRAZY_VEHICAL_in_GAMEPLAY_Ganungi;
	 KrazyLevelThriver::get_default()->on_appendThriving(_thriving);
	 KrazyLevelThriver::get_default()->exit_sequencetext() = _KRAZY_EXIT_SEQUENCE_4;
 }
 //
 void KrazyLevelCityContext::onLevel_13()
 {
	 KrazyFloatList _vectors;
	 _KRAZY_COPY_LIST_PTR(_play_tool_vectors_4 , _vectors , 4);
	 KrazyPlayToolRect::get_default()->onSetVectors(_vectors);
	 //..,
	 _XML = gKrazyLevelXML_5_Tutorial;
	 KrazyThriving* _thriving = new KrazyThriving;
	 _thriving->vehical_name() = KRAZY_VEHICAL_in_GAMEPLAY_Ganungi;
	 KrazyLevelThriver::get_default()->on_appendThriving(_thriving);
	 KrazyLevelThriver::get_default()->exit_sequencetext() = _KRAZY_EXIT_SEQUENCE_4;
 }
 //
 void KrazyLevelCityContext::onLevel_14()
 {
	 KrazyFloatList _vectors;
	 _KRAZY_COPY_LIST_PTR(_play_tool_vectors_4 , _vectors , 4);
	 KrazyPlayToolRect::get_default()->onSetVectors(_vectors);
	 //..,
	 _XML = gKrazyLevel_6_XML;
	 KrazyThriving* _thriving = new KrazyThriving;
	 _thriving->vehical_name() = KRAZY_VEHICAL_in_GAMEPLAY_Ganungi;
	 KrazyLevelThriver::get_default()->on_appendThriving(_thriving);
	 KrazyLevelThriver::get_default()->exit_sequencetext() = _KRAZY_EXIT_SEQUENCE_4;
 }
 //
 void KrazyLevelCityContext::onLevel_15()
 {
	 //Tutorial Mode..,
	 KrazyFloatList _vectors;
	 _KRAZY_COPY_LIST_PTR(_play_tool_vectors_1 , _vectors , 4);
	 KrazyPlayToolRect::get_default()->onSetVectors(_vectors);
	 //..,
	 _XML = gKrazyLevelXML_5_Tutorial_a;
	 KrazyThriving* _thriving = new KrazyThriving;
	 _thriving->vehical_name() = KRAZY_VEHICAL_in_GAMEPLAY_BangToona;
	 KrazyLevelThriver::get_default()->on_appendThriving(_thriving);
	 KrazyLevelThriver::get_default()->exit_sequencetext() = _KRAZY_EXIT_SEQUENCE_2;
	 KrazyTutorialThriver::get_default()->level() = 15;
 }
 //
  void KrazyLevelCityContext::onLevel_16()
 {
	 KrazyFloatList _vectors;
	 _KRAZY_COPY_LIST_PTR(_play_tool_vectors_1 , _vectors , 4);
	 KrazyPlayToolRect::get_default()->onSetVectors(_vectors);
	 //..,
	 _XML = gKrazyLevel_7_XML;
	 KrazyThriving* _thriving = new KrazyThriving;
	 _thriving->vehical_name() = KRAZY_VEHICAL_in_GAMEPLAY_BangToona;
	 KrazyLevelThriver::get_default()->on_appendThriving(_thriving);
	 KrazyLevelThriver::get_default()->exit_sequencetext() = _KRAZY_EXIT_SEQUENCE_2;
 }
  //
 #else
  void KrazyLevelCityContext::onLevel_1()
 {
	 KrazyFloatList _vectors;
	 _KRAZY_COPY_LIST_PTR(_play_tool_vectors_1 , _vectors , 4);
	 KrazyPlayToolRect::get_default()->onSetVectors(_vectors);
	 //..,
	 _XML = gKrazyLevel_1_XML;
	 KrazyThriving* _thriving = new KrazyThriving;
	 _thriving->vehical_name() = KRAZY_VEHICAL_in_GAMEPLAY_RangToona;
	 KrazyLevelThriver::get_default()->on_appendThriving(_thriving);
	 KrazyLevelThriver::get_default()->exit_sequencetext() = _KRAZY_EXIT_SEQUENCE_1;
 }
 //
 void KrazyLevelCityContext::onLevel_2()
 {
	  KrazyFloatList _vectors;
	 _KRAZY_COPY_LIST_PTR(_play_tool_vectors_2 , _vectors , 4);
	 KrazyPlayToolRect::get_default()->onSetVectors(_vectors);
	 //..,
	 _XML = gKrazyLevel_2_XML;
	 KrazyThriving* _thriving = new KrazyThriving;
	 _thriving->vehical_name() = KRAZY_VEHICAL_in_GAMEPLAY_BangToona;
	 KrazyLevelThriver::get_default()->on_appendThriving(_thriving);
	  KrazyLevelThriver::get_default()->exit_sequencetext() = _KRAZY_EXIT_SEQUENCE_2;
 }
 //
 void KrazyLevelCityContext::onLevel_3()
 {
	 KrazyFloatList _vectors;
	 _KRAZY_COPY_LIST_PTR(_play_tool_vectors_2 , _vectors , 4);
	 KrazyPlayToolRect::get_default()->onSetVectors(_vectors);
	 //..,
	 _XML = gKrazyLevel_3_XML;
	 KrazyThriving* _thriving = new KrazyThriving;
	 _thriving->vehical_name() = KRAZY_VEHICAL_in_GAMEPLAY_BangToona;
	 KrazyLevelThriver::get_default()->on_appendThriving(_thriving);
	 KrazyLevelThriver::get_default()->exit_sequencetext() = _KRAZY_EXIT_SEQUENCE_2;
 }
 //
 void KrazyLevelCityContext::onLevel_4()
 {
	  KrazyFloatList _vectors;
	 _KRAZY_COPY_LIST_PTR(_play_tool_vectors_3 , _vectors , 4);
	 KrazyPlayToolRect::get_default()->onSetVectors(_vectors);
	 //..,
	 _XML = gKrazyLevel_4_XML;
	 KrazyThriving* _thriving = new KrazyThriving;
	 _thriving->vehical_name() = KRAZY_VEHICAL_in_GAMEPLAY_Ganungi;
	 _thriving->wait_further() = KRAZY_BOOL_False;
	 KrazyLevelThriver::get_default()->on_appendThriving(_thriving);
	 KrazyLevelThriver::get_default()->exit_sequencetext() = _KRAZY_EXIT_SEQUENCE_3;
 }
 //
 void KrazyLevelCityContext::onLevel_5()
 {
	  KrazyFloatList _vectors;
	 _KRAZY_COPY_LIST_PTR(_play_tool_vectors_4 , _vectors , 4);
	 KrazyPlayToolRect::get_default()->onSetVectors(_vectors);
	 //..,
	 _XML = gKrazyLevel_5_XML;
	 KrazyThriving* _thriving = new KrazyThriving;
	 _thriving->vehical_name() = KRAZY_VEHICAL_in_GAMEPLAY_BangToona;
	 KrazyLevelThriver::get_default()->on_appendThriving(_thriving);
	 KrazyLevelThriver::get_default()->exit_sequencetext() = _KRAZY_EXIT_SEQUENCE_2;
 }
 //
 void KrazyLevelCityContext::onLevel_6()
 {
	 KrazyFloatList _vectors;
	 _KRAZY_COPY_LIST_PTR(_play_tool_vectors_4 , _vectors , 4);
	 KrazyPlayToolRect::get_default()->onSetVectors(_vectors);
	 //..,
	 _XML = gKrazyLevel_6_XML;
	 KrazyThriving* _thriving = new KrazyThriving;
	 _thriving->vehical_name() = KRAZY_VEHICAL_in_GAMEPLAY_Ganungi;
	 KrazyLevelThriver::get_default()->on_appendThriving(_thriving);
	 KrazyLevelThriver::get_default()->exit_sequencetext() = _KRAZY_EXIT_SEQUENCE_4;
 }
 //
 void KrazyLevelCityContext::onLevel_7()
 {
	 KrazyFloatList _vectors;
	 _KRAZY_COPY_LIST_PTR(_play_tool_vectors_1 , _vectors , 4);
	 KrazyPlayToolRect::get_default()->onSetVectors(_vectors);
	 //..,
	 _XML = gKrazyLevel_7_XML;
	 KrazyThriving* _thriving = new KrazyThriving;
	 _thriving->vehical_name() = KRAZY_VEHICAL_in_GAMEPLAY_BangToona;
	 KrazyLevelThriver::get_default()->on_appendThriving(_thriving);
	 KrazyLevelThriver::get_default()->exit_sequencetext() = _KRAZY_EXIT_SEQUENCE_2;
 }

 //
#endif

