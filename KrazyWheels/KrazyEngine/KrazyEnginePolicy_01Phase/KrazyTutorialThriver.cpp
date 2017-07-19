#include "stdafx.h"
#include "krazytutorialthriver.h"
#include "KrazyDoubleLaneRoadRendrer.h"
#include "KrazyGameRenderingPolicy_01Phase.h"
#include "KrazyTextXML.h"
#include "KrazyLevelCityNetwork.h"
#include "KrazyVehicalCache.h"
#include "../KrazyLevelXML/KrazyLevelXML_Tutorial_1.h"
#include "../KrazyLevelXML/KrazyLevelXML_1_and_2_Tutorial.h"
#include "../KrazyLevelXML/KrazyLevelXML_3_Tutorial.h"
#include "../KrazyLevelXML/KrazyLevelXML_4_Tutorial.h"
#include "../KrazyLevelXML/KrazyLevelXML_5_Tutorial.h"
//..,
#define ATTR_TYPE      string("Type")
#define ATTR_POINT     string("PointIndex")
#define ATTR_VEHICLE   string("Vehicle") 
#define ATTR_LANE      string("LaneIndex")
#define ATTR_DISTANCE  string("Distance")
#define ATTR_ALLVEHICLESTOP string("AllVehicleStop")
#define ATTR_ADDTOWAITLIST  string("AddToWaitList")
#define ATTR_CLEARWAITLIST  string("ClearFromWaitList")
//..,
KrazyTutorialThriver*	KrazyTutorialThriver::_Me = 0;
KrazyTutorialThriver::KrazyTutorialThriver() :
_CurrentSequence(0)
{
	reset();
}
//..,
KrazyTutorialThriver::~KrazyTutorialThriver()
{
}
//..,
void KrazyTutorialThriver::init()
{
}
//..,
void KrazyTutorialThriver::destroy()
{
	clean_seq_kueue();
	clean_seq_list();
}
//..,
KrazyTutorialThriver* KrazyTutorialThriver::get_default()
{
	if(!_Me){
		_Me = new KrazyTutorialThriver;
		_Me->init();
	}
	//..,
	return _Me;
}
//..,
void KrazyTutorialThriver::release_default()
{
	if(_Me){
		_Me->destroy();
		delete _Me;_Me = 0;
	}
}
//..,
void KrazyTutorialThriver::on_attach_doublelanerendrer(KrazyDoubleLaneRendrer* _double_lane_rendrer)
{
	_DoubleLaneRendrer = _double_lane_rendrer;

}
//..,
void KrazyTutorialThriver::on_attach_gamerendrer(KrazyGameRenderingPolicy_01Phase* _game_rendrer)
{
	_GameRendrer = _game_rendrer;
}
//..,
KrazyBool& KrazyTutorialThriver::mode()  { return _Mode;  }
int&       KrazyTutorialThriver::level() { return _Level; }
KrazyBool  KrazyTutorialThriver::all_vehical_stopped() { return _AllVehicalStopped; }
//..,
bool KrazyTutorialThriver::should_wait(KrazyVehical* _vehical)
{
	int _count = _VehicalWaitList.size();
	for(int i = 0; i < _count; i++)
	{
		if(_VehicalWaitList[i] == _vehical){
			return true;
		}
	}
	//..,
	return false;
}
//..,
void KrazyTutorialThriver::reset()
{
	_CurrentSequence = 0;
	_Level = 0;
	_Mode = KRAZY_BOOL_False;
	_AllVehicalStopped = KRAZY_BOOL_False;
	clean_seq_kueue();
	clean_seq_list();
}
//..,
void KrazyTutorialThriver::clean_for_thriving()
{
	_XML.empty();
	clean_seq_kueue();
	clean_seq_list();
}
//..,
void KrazyTutorialThriver::clean_seq_kueue()
{
	for(;_SeqKueue.size() > 0;){
		KrazySequence* _seq = _SeqKueue.front();
		delete _seq;_seq = 0;_SeqKueue.pop();
	}
}
//..,
void KrazyTutorialThriver::clean_seq_list()
{
	int _count = _SeqList.size();
	for(int i = 0; i < _count; i++)
	{
		KrazySequence* _sequence = _SeqList[i];
		delete _sequence;_sequence = 0;
	}
	//..,
	_SeqList.clear();
}
//..
void KrazyTutorialThriver::erase_from_seq_list(KrazySequence* _seq)
{
	int _count = _SeqList.size();
	int _index_to_delete = -1;
	for(int i = 0; i < _count; i++)
	{
		KrazySequence* _sequence = _SeqList[i];
		if( _seq == _sequence){
			_index_to_delete = i;
			break;
		}
		
	}
	//..,
	if(_index_to_delete > -1){
		KrazySequence* _sequence = _SeqList[_index_to_delete];
		delete _sequence;_sequence = 0;
		_SeqList.erase(_SeqList.begin() + _index_to_delete);
	}
	//..,
}
//..,
void KrazyTutorialThriver::on_set_level()
{
	switch(_Mode)
	{
	case KRAZY_BOOL_False : return;
	}
	//..,
	clean_for_thriving();
	switch(_Level)
	{
	case 1  : on_level_1();		break;
	case 2  : on_level_2();		break;
	case 6  : on_level_6();		break;
	case 8  : on_level_8();		break;
	case 9  : on_level_9();		break;
	case 11 : on_level_11();	break;
	case 15 : on_level_15();    break;
	}
}
//..,
void KrazyTutorialThriver::on_level_1()
{
	_XML = gKrazyTTDataLevel_1;
	on_set_data_for_level();
}
//..,
void KrazyTutorialThriver::on_level_2()
{
	_XML = gKrazyTTDataLevel_2;
	on_set_data_for_level();
}
//..,
void KrazyTutorialThriver::on_level_6()
{
	_XML = gKrazyTTDataLevel_3;
	on_set_data_for_level();
}
//..,
void KrazyTutorialThriver::on_level_8()
{
	_XML = gKrazyTTDataLevel_4;
	on_set_data_for_level();
}
//..,
void KrazyTutorialThriver::on_level_9()
{
	_XML = gKrazyTTDataLevel_5;
	on_set_data_for_level();
}
//..,
void KrazyTutorialThriver::on_level_11()
{
	_XML = gKrazyTTDataLevel_6;
	on_set_data_for_level();
}
//..,
void KrazyTutorialThriver::on_level_15()
{
	_XML = gKrazyTTDataLevel_7;
	on_set_data_for_level();
}
//..,
void KrazyTutorialThriver::on_set_currentseq()
{
	if(!_CurrentSequence){
		if(_SeqKueue.size() > 0){
			_CurrentSequence = _SeqKueue.front();
			_SeqKueue.pop();
			_SeqList.push_back(_CurrentSequence);
		}
	}
}
//..,
void KrazyTutorialThriver::on_add_to_waitlist(KrazySequence* _seq)
{
	KrazyIntList* _add = _seq->get_addwaitlist();
	int _count = _add->size();
	for(int i = 0; i < _count; i++){
		KrazyVehicalName _vehicle = (KrazyVehicalName)_add->at(i);
		KrazyVehical* __vehicle = get_Vehicle(_vehicle);
		_VehicalWaitList.push_back(__vehicle);
	}
}
//..,
void KrazyTutorialThriver::on_clear_waitlist(KrazySequence* _seq)
{
	KrazyIntList* _clear = _seq->get_clearwaitlist();
	int _count = _clear->size();
	KrazyIntList _index_to_delete;
	for(int i = 0; i < _count; i++){
		KrazyVehicalName _vehicle_name = (KrazyVehicalName)_clear->at(i);
		for(int j = 0; j < _VehicalWaitList.size();j++){
			KrazyVehical* _vehicle = _VehicalWaitList[j];
			if(_vehicle->vehical_name() == _vehicle_name){
				_index_to_delete.push_back(j);
			}
		}
	}
	//..,
	for(int i = 0; i < _index_to_delete.size(); i++){
		_VehicalWaitList.erase(_VehicalWaitList.begin() + _index_to_delete[i]);
	}
}
//..,
void KrazyTutorialThriver::on_set_data_for_level()
{
	KrazyTextXML _xml_;_xml_.loadText(_XML);
	int _kids = _xml_.get_Kids_Count();
	for(int kid = 0; kid < _kids; kid++){
		KrazyXMLNode* _kid_node = _xml_.get_KidsNode(kid);
		KrazyXMLAttribute* _type = _kid_node->get_AttributeByTypeIfAny(ATTR_TYPE);
		KrazyXMLAttribute* _all_vehicle_stop = _kid_node->get_AttributeByTypeIfAny(ATTR_ALLVEHICLESTOP);
		KrazyXMLAttribute* _add_to_waitmap   = _kid_node->get_AttributeByTypeIfAny(ATTR_ADDTOWAITLIST);
		KrazyXMLAttribute* _add_to_clearmap  = _kid_node->get_AttributeByTypeIfAny(ATTR_CLEARWAITLIST);
		//..,		
		KrazyTTSequenceId _seq_id = (KrazyTTSequenceId)atoi(_type->get_Data().c_str());
		KrazyBool __all_vehicle_stop = (atoi(_all_vehicle_stop->get_Data().c_str()) == 1) ? KRAZY_BOOL_True : KRAZY_BOOL_False;
		//..,
		KrazyIntList _add_wait_list;_KRAZY_GET_INT_INDICES_FROM_STRING(_add_wait_list , _add_to_waitmap);
		KrazyIntList _clear_wait_list;_KRAZY_GET_INT_INDICES_FROM_STRING(_clear_wait_list , _add_to_clearmap);
		//..,
		switch(_seq_id)
		{
		case KRAZY_TTSEQ_Waiting_for_Point     : 
			{
				KrazyIntList _point_data;KrazyXMLAttribute* _point = _kid_node->get_AttributeByTypeIfAny(ATTR_POINT);
				_KRAZY_GET_INT_INDICES_FROM_STRING(_point_data , _point);
				KrazyXMLAttribute* _attr_vehicle = _kid_node->get_AttributeByTypeIfAny(ATTR_VEHICLE);
				KrazyVehicalName _vehicle = (KrazyVehicalName)atoi(_attr_vehicle->get_Data().c_str());
			    //..,
				int _lane_parent_index = _point_data[0];
				int _lane_index        = _point_data[1];
				int _indc_index        = _point_data[2];
				//..,
				KrazyLaneNode* _lane = get_Lane(_lane_parent_index , _lane_index);
				KrazyTrafficIndicator* _indc_point = get_Point(_indc_index , _lane);
				KrazyVehical* __vehicle = get_Vehicle(_vehicle);
				KrazyPointData* __point_data = new KrazyPointData;
				__point_data->on_set_point(_indc_point);
				__point_data->on_set_vehical(__vehicle);
				KrazyTTSequencePoint* _seq_point = new KrazyTTSequencePoint(__all_vehicle_stop);
				_seq_point->on_attach_pointdata(__point_data);
				_seq_point->add_to_addwaitlist(_add_wait_list);
				_seq_point->add_to_clearwaitlist(_clear_wait_list);
				_SeqKueue.push(_seq_point);
			}
			break;
			//..,
		case KRAZY_TTSEQ_Waiting_for_Touch     : 
			{
				KrazyXMLAttribute* _attr_vehicle = _kid_node->get_AttributeByTypeIfAny(ATTR_VEHICLE);
				KrazyVehicalName _vehicle = (KrazyVehicalName)atoi(_attr_vehicle->get_Data().c_str());
				KrazyVehical* __vehicle = get_Vehicle(_vehicle);
				KrazyTouchData* _touch_data = new KrazyTouchData;
				_touch_data->on_set_vehical(__vehicle);
				KrazyTTSequenceTouch* _seq_touch = new KrazyTTSequenceTouch(__all_vehicle_stop);
				_seq_touch->on_attach_touchdata(_touch_data);
				_seq_touch->add_to_addwaitlist(_add_wait_list);
				_seq_touch->add_to_clearwaitlist(_clear_wait_list);
				_SeqKueue.push(_seq_touch);
			}
			break;
		case KRAZY_TTSEQ_Waiting_for_Turns     : 
			{
				KrazyIntList _index_list_;KrazyXMLAttribute* _LaneIndex = _kid_node->get_AttributeByTypeIfAny(ATTR_LANE);
				_KRAZY_GET_INT_INDICES_FROM_STRING(_index_list_ , _LaneIndex);
				KrazyXMLAttribute* _attr_vehicle = _kid_node->get_AttributeByTypeIfAny(ATTR_VEHICLE);
				KrazyVehicalName _vehicle = (KrazyVehicalName)atoi(_attr_vehicle->get_Data().c_str());
				int _lane_parent_index = _index_list_[0];
				int _lane_index        = _index_list_[1];
				KrazyLaneNode* _lane = get_Lane(_lane_parent_index , _lane_index);
				KrazyVehical* __vehicle = get_Vehicle(_vehicle);
				KrazyTurnData* _turn_data = new KrazyTurnData;
				_turn_data->on_set_turninglane(_lane);
				_turn_data->on_set_vehical(__vehicle);
				KrazyTTSequenceTurns* _seq_turn = new KrazyTTSequenceTurns(__all_vehicle_stop);
				_seq_turn->on_attach_turndata(_turn_data);
				_seq_turn->add_to_addwaitlist(_add_wait_list);
				_seq_turn->add_to_clearwaitlist(_clear_wait_list);
				_SeqKueue.push(_seq_turn);
			}
			break;
		case KRAZY_TTSEQ_Waiting_for_Distance  : 
			{
				KrazyIntList _point_data;KrazyXMLAttribute* _point = _kid_node->get_AttributeByTypeIfAny(ATTR_POINT);
				_KRAZY_GET_INT_INDICES_FROM_STRING(_point_data , _point);
				KrazyXMLAttribute* _attr_vehicle = _kid_node->get_AttributeByTypeIfAny(ATTR_VEHICLE);
				KrazyVehicalName _vehicle = (KrazyVehicalName)atoi(_attr_vehicle->get_Data().c_str());
				KrazyXMLAttribute* _attr_distance = _kid_node->get_AttributeByTypeIfAny(ATTR_DISTANCE);
				float _distance;_KRAZY_GET_FLOAT(_distance, _attr_distance);
			    //..,
				int _lane_parent_index = _point_data[0];
				int _lane_index        = _point_data[1];
				int _indc_index        = _point_data[2];
				//..,
				KrazyLaneNode* _lane = get_Lane(_lane_parent_index , _lane_index);
				KrazyTrafficIndicator* _indc_point = get_Point(_indc_index , _lane);
				KrazyTrafficIndicator* _dest_point = 0;
				switch(_indc_point->Type()){
				case KRAZY_TRAFFIC_INDC_FlipPoint : 
					_dest_point = _indc_point->PTRParentLaneNode()->getOppositeSingleLane()->getEntryIndicator();
					break;
				default:
					_dest_point = _indc_point->get_DestinationIndc(0);
  					break;
				}
	            //..,			
				KrazyVehical* __vehicle = get_Vehicle(_vehicle);
				KrazyDistanceData* _distance_data  = new KrazyDistanceData;
				_distance_data->on_set_distance(_distance);
				_distance_data->on_set_startingvector(*_dest_point->Vector());
				_distance_data->on_set_vehical(__vehicle);
				KrazyTTSequenceDistance* _seq_distance = new KrazyTTSequenceDistance(__all_vehicle_stop);
				_seq_distance->on_attach_distancedata(_distance_data);
				_seq_distance->add_to_addwaitlist(_add_wait_list);
				_seq_distance->add_to_clearwaitlist(_clear_wait_list);
				_SeqKueue.push(_seq_distance);

			}
			break;
		case KRAZY_TTSEQ_Waiting_for_Flip      : 
			{
				KrazyXMLAttribute* _attr_vehicle = _kid_node->get_AttributeByTypeIfAny(ATTR_VEHICLE);
				KrazyVehicalName _vehicle = (KrazyVehicalName)atoi(_attr_vehicle->get_Data().c_str());
				KrazyVehical* __vehicle = get_Vehicle(_vehicle);
				KrazyFlipData* _touch_data = new KrazyFlipData;
				_touch_data->on_set_vehical(__vehicle);
				KrazyTTSequenceFlip* _seq_touch = new KrazyTTSequenceFlip(__all_vehicle_stop);
				_seq_touch->on_attach_flipdata(_touch_data);
				_seq_touch->add_to_addwaitlist(_add_wait_list);
				_seq_touch->add_to_clearwaitlist(_clear_wait_list);
				_SeqKueue.push(_seq_touch);

			}
			break;
		case KRAZY_TTSEQ_Waiting_for_Exit:
			{
				KrazyXMLAttribute* _attr_vehicle = _kid_node->get_AttributeByTypeIfAny(ATTR_VEHICLE);
				KrazyVehicalName _vehicle = (KrazyVehicalName)atoi(_attr_vehicle->get_Data().c_str());
				KrazyVehical* __vehicle = get_Vehicle(_vehicle);
				KrazyExitData* _exit_data = new KrazyExitData;
				_exit_data->on_set_vehical(__vehicle);
				KrazyTTSequenceExit* _seq_exit = new KrazyTTSequenceExit(__all_vehicle_stop);
				_seq_exit->on_attach_exitdata(_exit_data);
				_seq_exit->add_to_addwaitlist(_add_wait_list);
				_seq_exit->add_to_clearwaitlist(_clear_wait_list);
				_SeqKueue.push(_seq_exit);
			}
			break;
		case KRAZY_TTSEQ_Waiting_before_thepoint:
			{
				KrazyIntList _point_data;KrazyXMLAttribute* _point = _kid_node->get_AttributeByTypeIfAny(ATTR_POINT);
				_KRAZY_GET_INT_INDICES_FROM_STRING(_point_data , _point);
				KrazyXMLAttribute* _attr_vehicle = _kid_node->get_AttributeByTypeIfAny(ATTR_VEHICLE);
				KrazyVehicalName _vehicle = (KrazyVehicalName)atoi(_attr_vehicle->get_Data().c_str());
				KrazyXMLAttribute* _attr_distance = _kid_node->get_AttributeByTypeIfAny(ATTR_DISTANCE);
				float _distance;_KRAZY_GET_FLOAT(_distance, _attr_distance);
			    //..,
				int _lane_parent_index = _point_data[0];
				int _lane_index        = _point_data[1];
				int _indc_index        = _point_data[2];
				//..,
				KrazyLaneNode* _lane = get_Lane(_lane_parent_index , _lane_index);
				KrazyTrafficIndicator* _indc_point = get_Point(_indc_index , _lane);
				KrazyVehical* __vehicle = get_Vehicle(_vehicle);
				//..,
				KrazyDistanceData* _distance_data  = new KrazyDistanceData;
				_distance_data->on_set_distance(_distance);
				_distance_data->on_set_startingvector(*_indc_point->Vector());
				_distance_data->on_set_vehical(__vehicle);
				KrazyTTSequenceDistance* _seq_distance = new KrazyTTSequenceDistance(__all_vehicle_stop);
				_seq_distance->on_attach_distancedata(_distance_data);
				_seq_distance->add_to_addwaitlist(_add_wait_list);
				_seq_distance->add_to_clearwaitlist(_clear_wait_list);
				_SeqKueue.push(_seq_distance);
			}
			break;
		case KRAZY_TTSEQ_Waiting_after_vehical_start:
			{
				KrazyXMLAttribute* _attr_vehicle = _kid_node->get_AttributeByTypeIfAny(ATTR_VEHICLE);
				KrazyVehicalName _vehicle = (KrazyVehicalName)atoi(_attr_vehicle->get_Data().c_str());
				KrazyXMLAttribute* _attr_distance = _kid_node->get_AttributeByTypeIfAny(ATTR_DISTANCE);
				float _distance;_KRAZY_GET_FLOAT(_distance, _attr_distance);
			    //..,
				KrazyVehical* __vehicle = get_Vehicle(_vehicle);
				//..,
				KrazyDistanceData* _distance_data  = new KrazyDistanceData;
				_distance_data->on_set_distance(_distance);
				_distance_data->on_set_startingvector(*__vehicle->Vector());
				_distance_data->on_set_vehical(__vehicle);
				KrazyTTSequenceDistance* _seq_distance = new KrazyTTSequenceDistance(__all_vehicle_stop);
				_seq_distance->on_attach_distancedata(_distance_data);
				_seq_distance->add_to_addwaitlist(_add_wait_list);
				_seq_distance->add_to_clearwaitlist(_clear_wait_list);
				_SeqKueue.push(_seq_distance);
			}
			break;
		}
		

	}
}
//..,
KrazyLaneNode* KrazyTutorialThriver::get_Lane(int _parent_index , int _kid_index)
{
	KrazyNetworkCity_01Phase* ptr_parent_network = KrazyNetworkCity_01Phase::getNetwork();
	vector<KrazyWheelNetwork_01Phase*>* ptr_network_nodes = ptr_parent_network->getNodeCache();
	KrazyLaneNode* _lane = ((KrazyNetworkCity_01Phase*)ptr_network_nodes->at(_parent_index))->getOwner();
	//..,
	return _lane->getLane(_kid_index);
}
//..,
KrazyTrafficIndicator* KrazyTutorialThriver::get_Point(int _indc_index , KrazyLaneNode* _lane)
{
	return _lane->get_TrafficIndicators()->at(_indc_index);
}
//..,
KrazyVehical* KrazyTutorialThriver::get_Vehicle(KrazyVehicalName _vehicle_name)
{
	KrazyVehicalCache* ptr_cache = KrazyVehicalCache::getCache(); 
	int vehical_count = ptr_cache->get_VehicalsInTheNetworkCount();
	//
	for(int i = 0; i < vehical_count; i++){
		KrazyVehical* _vehical = ptr_cache->get_VehicalFromTheNetwork(i);
		if(_vehical->vehical_name() == _vehicle_name){
			return _vehical;
		}
	}
	//..,
	return 0;
}
//..,
void KrazyTutorialThriver::on_point(KrazyVehical* _vehical , KrazyTrafficIndicator* _point)
{
	on_set_currentseq();
	if(_CurrentSequence){
		switch(_CurrentSequence->seq_id())
		{
		case KRAZY_TTSEQ_Waiting_for_Point : 
			{
				KrazyTTSequencePoint* _seq_point = (KrazyTTSequencePoint*)_CurrentSequence;
				KrazyPointData* _point_data = _seq_point->data();
				KrazyVehical* _seq_vehical = _point_data->vehical();
				KrazyTrafficIndicator* __point = _point_data->point();
				if(_seq_vehical == _vehical && __point == _point){
					// sequence met..,
					_AllVehicalStopped = _seq_point->all_vehical_stop();
					on_add_to_waitlist(_seq_point);
					on_clear_waitlist(_seq_point);
					erase_from_seq_list(_CurrentSequence);_CurrentSequence = 0;
					on_set_currentseq();
				}
			}
			break;
		}
	}

}
//..,
void KrazyTutorialThriver::on_turned(KrazyVehical* _vehical)
{
	on_set_currentseq();
	if(_CurrentSequence){
		switch(_CurrentSequence->seq_id())
		{
		case KRAZY_TTSEQ_Waiting_for_Turns : 
			{
				KrazyTTSequenceTurns* _seq_turn = (KrazyTTSequenceTurns*)_CurrentSequence;
				KrazyTurnData* _turn_data = _seq_turn->data();
				KrazyVehical* _seq_vehical   = _turn_data->vehical();
				KrazyLaneNode* _lane_to_turn = _turn_data->lane_to_turn();  
				if(_seq_vehical == _vehical && _vehical->PTRCurrentAssignedSingleLane() == _lane_to_turn){
					// sequence met..,
					_AllVehicalStopped = _seq_turn->all_vehical_stop();
					on_add_to_waitlist(_seq_turn);
					on_clear_waitlist(_seq_turn);
					erase_from_seq_list(_CurrentSequence);_CurrentSequence = 0;
					on_set_currentseq();
				}
				else
				{
					// game over can occur .., in self try mode..,
				}
			}
			break;
		}
	}
}
//..,
void KrazyTutorialThriver::on_moved(KrazyVehical* _vehical)
{
	on_set_currentseq();
	if(_CurrentSequence){
		switch(_CurrentSequence->seq_id())
		{
		case KRAZY_TTSEQ_Waiting_for_Distance : 
			{
				KrazyTTSequenceDistance* _seq_distance = (KrazyTTSequenceDistance*)_CurrentSequence;
				KrazyDistanceData* _distance_data = _seq_distance->data();
				KrazyVehical* _seq_vehical   = _distance_data->vehical();
				if( _seq_vehical == _vehical){
					glm::vec2* _starting = _distance_data->starting();
					float _distance = _distance_data->distance();
					float _travelled_distance = glm::distance(*_starting , *_vehical->Vector());
					if(_travelled_distance >= _distance){
						_AllVehicalStopped = _seq_distance->all_vehical_stop();
						on_add_to_waitlist(_seq_distance);
						on_clear_waitlist(_seq_distance);
						erase_from_seq_list(_CurrentSequence);_CurrentSequence = 0;
					    on_set_currentseq();
					}
				}
			}
			break;
		}
	}
}
//..,
bool KrazyTutorialThriver::on_touched(KrazyVehical* _vehical)
{
	on_set_currentseq();
	if(_CurrentSequence){
		switch(_CurrentSequence->seq_id())
		{
		case KRAZY_TTSEQ_Waiting_for_Touch : 
			{
				KrazyTTSequenceTouch* _seq_touched = (KrazyTTSequenceTouch*)_CurrentSequence;
				KrazyTouchData* _touch_data = _seq_touched->data();
				KrazyVehical* _seq_vehical   = _touch_data->vehical();
				if(_seq_vehical == _vehical){
					_AllVehicalStopped = _seq_touched->all_vehical_stop();
					on_add_to_waitlist(_seq_touched);
					on_clear_waitlist(_seq_touched);
					erase_from_seq_list(_CurrentSequence);_CurrentSequence = 0;
					on_set_currentseq();
					return true;
				}
			}
			break;
		}
	}
	return false;
}
//..,
bool KrazyTutorialThriver::on_reflection_touched(KrazyVehical* _vehical)
{
	on_set_currentseq();
	if(_CurrentSequence){
		switch(_CurrentSequence->seq_id())
		{
		case KRAZY_TTSEQ_Waiting_for_Flip : 
			{
				KrazyTTSequenceFlip* _seq_touched = (KrazyTTSequenceFlip*)_CurrentSequence;
				KrazyFlipData* _flip_data = _seq_touched->data();
				KrazyVehical* _seq_vehical   = _flip_data->vehical();
				if( _seq_vehical == _vehical){
					_AllVehicalStopped = _seq_touched->all_vehical_stop();
					on_add_to_waitlist(_seq_touched);
					on_clear_waitlist(_seq_touched);
					erase_from_seq_list(_CurrentSequence);_CurrentSequence = 0;
					on_set_currentseq();
					return true;
				}
			}
			break;
		}
	}
	return false;
}
//..,
void KrazyTutorialThriver::on_exit(KrazyVehical* _vehical)
{
	on_set_currentseq();
	if(_CurrentSequence){
		switch(_CurrentSequence->seq_id())
		{
		case KRAZY_TTSEQ_Waiting_for_Exit : 
			{
				KrazyTTSequenceExit* _seq_exit = (KrazyTTSequenceExit*)_CurrentSequence;
				KrazyExitData* _exit_data = _seq_exit->data();
				KrazyVehical* _seq_vehical   = _exit_data->vehical();
				if( _seq_vehical == _vehical){
					_AllVehicalStopped = _seq_exit->all_vehical_stop();
					on_add_to_waitlist(_seq_exit);
					on_clear_waitlist(_seq_exit);
					erase_from_seq_list(_CurrentSequence);_CurrentSequence = 0;
					on_set_currentseq();
				}
			}
			break;
		}
	}
}
//..,
void KrazyTutorialThriver::on_waiting_before_point(KrazyVehical* _vehical)
{
	on_set_currentseq();
	if(_CurrentSequence){
		switch(_CurrentSequence->seq_id())
		{
		case KRAZY_TTSEQ_Waiting_before_thepoint : 
			{
				KrazyTTSequenceDistance* _seq_distance = (KrazyTTSequenceDistance*)_CurrentSequence;
				KrazyDistanceData* _distance_data = _seq_distance->data();
				KrazyVehical* _seq_vehical   = _distance_data->vehical();
				if( _seq_vehical == _vehical){
					glm::vec2* _starting = _distance_data->starting();
					float _distance = _distance_data->distance();
					float _travelled_distance = glm::distance(*_starting , *_vehical->Vector());
					if(_travelled_distance >= _distance){
						_AllVehicalStopped = _seq_distance->all_vehical_stop();
						on_add_to_waitlist(_seq_distance);
						on_clear_waitlist(_seq_distance);
						erase_from_seq_list(_CurrentSequence);_CurrentSequence = 0;
						on_set_currentseq();
					}
				}
			}
			break;
		}
	}
}
//..,
void KrazyTutorialThriver::on_waiting_after_vehicle_start(KrazyVehical* _vehical)
{
	on_set_currentseq();
	if(_CurrentSequence){
		switch(_CurrentSequence->seq_id())
		{
		case KRAZY_TTSEQ_Waiting_after_vehical_start : 
			{
				KrazyTTSequenceDistance* _seq_distance = (KrazyTTSequenceDistance*)_CurrentSequence;
				KrazyDistanceData* _distance_data = _seq_distance->data();
				KrazyVehical* _seq_vehical   = _distance_data->vehical();
				if( _seq_vehical == _vehical){
					glm::vec2* _starting = _distance_data->starting();
					float _distance = _distance_data->distance();
					float _travelled_distance = glm::distance(*_starting , *_vehical->Vector());
					if(_travelled_distance >= _distance){
						_AllVehicalStopped = _seq_distance->all_vehical_stop();
						on_add_to_waitlist(_seq_distance);
						on_clear_waitlist(_seq_distance);
						erase_from_seq_list(_CurrentSequence);_CurrentSequence = 0;
						on_set_currentseq();
					}
				}
			}
			break;
		}
	}
}
//..,
void KrazyTutorialThriver::execute()
{
	_DoubleLaneRendrer->render();
	switch(_CurrentState)
	{
	case KRAZY_TUT_STATE_EngineHelping : break;
	case KRAZY_TUT_STATE_SelfTry       : break;
	}
}
//..,