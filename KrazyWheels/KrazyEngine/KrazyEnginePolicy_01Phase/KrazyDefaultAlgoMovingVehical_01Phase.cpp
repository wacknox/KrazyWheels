#include "StdAfx.h"
#include "KrazyDefaultAlgoMovingVehical_01Phase.h"
#include "KrazyLaneNode.h"
#include "KrazyLevelCityNetwork.h"
#include "KrazyGLMInclusion.h"
#include "KrazyUNTanglers.h"
#include "KrazyClogDetector.h"
//..,
#define MATCH_DISTANCE_EPSILON  0.401f
#define MAX_DISTANCE_EPSILON    10000.0f
#define KRAZY_ASSIGN_DESTINATION() \
	KrazyTrafficIndicator* ptr_dest_end = ptr_destination_lane->getDestinationEnd();\
	if(!ptr_dest_end){\
	ptr_dest_end = ptr_destination_lane->getFlipTrafficIndicator();\
	}\
	_PTRAuxshortroutemap->clear();_PTRAuxshortroutemap->push_back(ptr_dest_end);\

////

#define KRAZY_RESET_FLIP_SWITCHES() \
		switch(ptr_vehical->flip_mode())\
		{	\
			case KRAZY_VEHICAL_FLIP_MODE_Normal: 	\
			case KRAZY_VEHICAL_FLIP_MODE_Spot:\
				 ptr_vehical->flip_mode() = KRAZY_VEHICAL_FLIP_MODE_None;\
				 break;\
		}\
/////////////////////////////////////////////////////////
KrazyDefaultAlgoMovingVehical_01Phase::KrazyDefaultAlgoMovingVehical_01Phase(void) :
_CurrentDistance(0.0f)
,_PTRAuxshortroutemap(new KrazyTrafficIndicatorList())
,_RealDistance(10000.0f)
{
}
////////////////////////////////////////////////////////
KrazyDefaultAlgoMovingVehical_01Phase::~KrazyDefaultAlgoMovingVehical_01Phase(void)
{
	cleanUp();
}
////////////////////////////////////////////////////////
void KrazyDefaultAlgoMovingVehical_01Phase::cleanUp()
{
	if(_PTRAuxshortroutemap){
		_PTRAuxshortroutemap->clear();
		delete _PTRAuxshortroutemap;_PTRAuxshortroutemap = 0;
	}
}
//////////////////////////////////////////////////////////////////////
#if 1
bool KrazyDefaultAlgoMovingVehical_01Phase::matchDistance(KrazyVehical* ptr_vehical)
{
	KrazyTrafficIndicator* ptr_next_turn = ptr_vehical->get_NextTurn();
	if(ptr_next_turn){
		glm::vec2* ptr_vec_turn_indc   = 0;
		KrazyLaneNode* ptr_dest_lane = 0;
		switch(ptr_next_turn->Type()){
		case KRAZY_TRAFFIC_INDC_FlipPoint:
			ptr_dest_lane = ptr_vehical->PTRCurrentAssignedSingleLane()->getOppositeSingleLane();
			ptr_vec_turn_indc = ptr_dest_lane->getEntryIndicator()->Vector();
			break;
		case KRAZY_TRAFFIC_INDC_Entry:
			{
				ptr_dest_lane = ptr_next_turn->PTRParentLaneNode();
				ptr_vec_turn_indc = ptr_next_turn->Vector();
				KrazyVehicalList* ptr_vehicals_e = ptr_dest_lane->getVehicalList();
				int count_e = ptr_vehicals_e->size();
				KrazyTrafficIndicator* _turn = ptr_next_turn;
				if(count_e > 0){
					KrazyVehical* ptr_last_vehical = ptr_vehicals_e->at(count_e - 1);
					if(!ptr_last_vehical->PTRRearVehical()){
						ptr_vehical->on_attachFrontVehical(ptr_last_vehical);
						ptr_last_vehical->on_attachRearVehical(ptr_vehical);
						KrazyLaneNode* _assigned_lane = ptr_last_vehical->PTRCurrentAssignedSingleLane();
						_turn = _assigned_lane->createOnSpotFlipIndicator();
						glm::vec2 _vec;
						switch(_assigned_lane->getProperties()->get_LaneDirection())
						{
							case KRAZY_LANE_DIRECTION_UP_HORZS  :
								 _vec = glm::vec2(ptr_last_vehical->VectorRearBumper()->x + _KRAZY_STD_GAP , ptr_last_vehical->VectorRearBumper()->y);
								 break;
							case KRAZY_LANE_DIRECTION_DOWN_HORZS   : 
								 _vec = glm::vec2(ptr_last_vehical->VectorRearBumper()->x + _KRAZY_STD_GAP , ptr_last_vehical->VectorRearBumper()->y);
								 break;
							case KRAZY_LANE_DIRECTION_UP_VERTS     : 
								 _vec = glm::vec2(ptr_last_vehical->VectorRearBumper()->x  , ptr_last_vehical->VectorRearBumper()->y - _KRAZY_STD_GAP);
								 break;
							case KRAZY_LANE_DIRECTION_DOWN_VERTS   : 
								 _vec = glm::vec2(ptr_last_vehical->VectorRearBumper()->x , ptr_last_vehical->VectorRearBumper()->y + _KRAZY_STD_GAP);
								 break;
						}
						_turn->Type() = KRAZY_TRAFFIC_INDC_Entry;
						_turn->on_reset(_vec , _assigned_lane);
					}
					else{
						int a = 0;
					}
				}
				on_vehicalTurn(ptr_vehical , _turn);
				ptr_vehical->on_laneChange(0);
				return false;
			}
			break;
		case KRAZY_TRAFFIC_INDC_Turn:
			{
				KrazyTrafficIndicator* ptr_dest_indc = ptr_next_turn->get_DestinationIndc(0);
				ptr_vec_turn_indc = ptr_dest_indc->Vector();
				ptr_dest_lane     = ptr_dest_indc->PTRParentLaneNode();
			}
			break; 
		case KRAZY_TRAFFIC_INDC_OnSpotFlip:
			{
				ptr_vec_turn_indc = ptr_next_turn->Vector();
				ptr_dest_lane     = ptr_next_turn->PTRParentLaneNode();
			}
			break;
		case KRAZY_TRAFFIC_INDC_SliderPoint:
			 ptr_vehical->on_attachFrontVehical(0);
			 ptr_vehical->on_attachRearVehical(0);
			 on_vehicalTurn(ptr_vehical,ptr_next_turn);
			 ptr_vehical->movestate() = KRAZY_VEHICAL_STATE_Moving;
			 ptr_vehical->waitstate() = KRAZY_VEHICAL_WAIT_STATE_Turned;
			 return false;
		}
		KrazyVehicalList* ptr_vehicals = ptr_dest_lane->getVehicalList();
		int count = ptr_vehicals->size();
		KrazyLaneDirection lane_direction = ptr_dest_lane->getProperties()->get_LaneDirection();
		float front_distance = MAX_DISTANCE_EPSILON;
		float rear_distance  = MAX_DISTANCE_EPSILON;
		KrazyVehical* ptr_sel_front_vehical = 0;
		KrazyVehical* ptr_sel_rear_vehical  = 0;
		int rear_index = 0;
		KrazyVehical* ptr_rear_entering_vehical = 0;
		for(int i = 0;i < count; i++){
			KrazyVehical* ptr_last_vehical = ptr_vehicals->at(i);
			switch(ptr_last_vehical->visibilitystate())
			{
			case KRAZY_BOOL_False:
				if(!ptr_rear_entering_vehical)
					ptr_rear_entering_vehical = ptr_last_vehical;
				
				continue;
			}
			glm::vec2* ptr_vec_front_bumper = ptr_last_vehical->VectorFrontBumper();
			glm::vec2* ptr_vec_rear_bumper  = ptr_last_vehical->VectorRearBumper();
			float  x_turn_indc = ptr_vec_turn_indc->x;
			float  y_turn_indc = ptr_vec_turn_indc->y;
			switch(lane_direction)
			{
			case KRAZY_LANE_DIRECTION_UP_HORZS     :
				if(ptr_vec_rear_bumper->x > x_turn_indc && ptr_vec_front_bumper->x < x_turn_indc){
					ptr_vehical->waitstate() = KRAZY_VEHICAL_WAIT_STATE_Waiting_to_Turn;
					KrazyClogDetector::getDefault()->append_waitingvehicals(ptr_vehical , ptr_last_vehical);
					return false;
				}
				else if(ptr_vec_front_bumper->x > x_turn_indc){
					float curr_distance = glm::distance(*ptr_vec_front_bumper , *ptr_vec_turn_indc);//ptr_vec_front_bumper->distance(*ptr_vec_turn_indc);
					if(curr_distance < MATCH_SPACE_THRESHOLD){
						ptr_vehical->waitstate() = KRAZY_VEHICAL_WAIT_STATE_Waiting_to_Turn;
						KrazyClogDetector::getDefault()->append_waitingvehicals(ptr_vehical , ptr_last_vehical);
						return false;
					}
					if(curr_distance < front_distance){
						front_distance = curr_distance;
						ptr_sel_front_vehical = ptr_last_vehical;
					}
				}
				else if(ptr_vec_rear_bumper->x < x_turn_indc){
					float curr_distance = glm::distance(*ptr_vec_rear_bumper , *ptr_vec_turn_indc);//ptr_vec_rear_bumper->distance(*ptr_vec_turn_indc);
					if(curr_distance < MATCH_SPACE_THRESHOLD){
						ptr_vehical->waitstate() = KRAZY_VEHICAL_WAIT_STATE_Waiting_to_Turn;
						KrazyClogDetector::getDefault()->append_waitingvehicals(ptr_vehical , ptr_last_vehical);
						return false;
					}
					if(curr_distance < rear_distance){
						rear_distance = curr_distance;
						ptr_sel_rear_vehical = ptr_last_vehical;
						rear_index = i;
					}
				}
				break;
			case KRAZY_LANE_DIRECTION_DOWN_HORZS   :
				if(ptr_vec_front_bumper->x > x_turn_indc && ptr_vec_rear_bumper->x < x_turn_indc){
					ptr_vehical->waitstate() = KRAZY_VEHICAL_WAIT_STATE_Waiting_to_Turn;
					KrazyClogDetector::getDefault()->append_waitingvehicals(ptr_vehical , ptr_last_vehical);
					return false;
				}
				else if(ptr_vec_front_bumper->x < x_turn_indc){
					float curr_distance = glm::distance(*ptr_vec_front_bumper , *ptr_vec_turn_indc);//ptr_vec_front_bumper->distance(*ptr_vec_turn_indc);
					if(curr_distance < MATCH_SPACE_THRESHOLD){
						ptr_vehical->waitstate() = KRAZY_VEHICAL_WAIT_STATE_Waiting_to_Turn;
						KrazyClogDetector::getDefault()->append_waitingvehicals(ptr_vehical , ptr_last_vehical);
						return false;
					}
					if(curr_distance < front_distance){
						front_distance = curr_distance;
						ptr_sel_front_vehical = ptr_last_vehical;
					}
				}
				else if(ptr_vec_rear_bumper->x > x_turn_indc){
					float curr_distance = glm::distance(*ptr_vec_rear_bumper , *ptr_vec_turn_indc);//ptr_vec_rear_bumper->distance(*ptr_vec_turn_indc);
					if(curr_distance < MATCH_SPACE_THRESHOLD){
						ptr_vehical->waitstate() = KRAZY_VEHICAL_WAIT_STATE_Waiting_to_Turn;
						KrazyClogDetector::getDefault()->append_waitingvehicals(ptr_vehical , ptr_last_vehical);
						return false;
					}
					if(curr_distance < rear_distance){
						rear_distance = curr_distance;
						ptr_sel_rear_vehical = ptr_last_vehical;
						rear_index = i;
					}
				}
				break;
			case KRAZY_LANE_DIRECTION_UP_VERTS     : 
				if(ptr_vec_front_bumper->y > y_turn_indc && ptr_vec_rear_bumper->y < y_turn_indc){
					ptr_vehical->waitstate() = KRAZY_VEHICAL_WAIT_STATE_Waiting_to_Turn;
					KrazyClogDetector::getDefault()->append_waitingvehicals(ptr_vehical , ptr_last_vehical);
					return false;
				}
				else if(ptr_vec_front_bumper->y < y_turn_indc){
					float curr_distance = glm::distance(*ptr_vec_front_bumper , *ptr_vec_turn_indc);
					if(curr_distance < MATCH_SPACE_THRESHOLD){
						ptr_vehical->waitstate() = KRAZY_VEHICAL_WAIT_STATE_Waiting_to_Turn;
						KrazyClogDetector::getDefault()->append_waitingvehicals(ptr_vehical , ptr_last_vehical);
						return false;
					}
					if(curr_distance < front_distance){
						front_distance = curr_distance;
						ptr_sel_front_vehical = ptr_last_vehical;
					}
				}
				else if(ptr_vec_rear_bumper->y > y_turn_indc){
					float curr_distance = glm::distance(*ptr_vec_rear_bumper , *ptr_vec_turn_indc);//ptr_vec_rear_bumper->distance(*ptr_vec_turn_indc);
					if(curr_distance < MATCH_SPACE_THRESHOLD){
						ptr_vehical->waitstate() = KRAZY_VEHICAL_WAIT_STATE_Waiting_to_Turn;
						KrazyClogDetector::getDefault()->append_waitingvehicals(ptr_vehical , ptr_last_vehical);
						return false;
					}
					if(curr_distance < rear_distance){
						rear_distance = curr_distance;
						ptr_sel_rear_vehical = ptr_last_vehical;
						rear_index = i;
					}
				}
				break;
			case KRAZY_LANE_DIRECTION_DOWN_VERTS   : 
				if(ptr_vec_rear_bumper->y > y_turn_indc && ptr_vec_front_bumper->y < y_turn_indc){
					ptr_vehical->waitstate() = KRAZY_VEHICAL_WAIT_STATE_Waiting_to_Turn;
					KrazyClogDetector::getDefault()->append_waitingvehicals(ptr_vehical , ptr_last_vehical);
					return false;
				}
				else if(ptr_vec_front_bumper->y > y_turn_indc){
					float curr_distance = glm::distance(*ptr_vec_front_bumper , *ptr_vec_turn_indc);//ptr_vec_front_bumper->distance(*ptr_vec_turn_indc);
					if(curr_distance < MATCH_SPACE_THRESHOLD){
						ptr_vehical->waitstate() = KRAZY_VEHICAL_WAIT_STATE_Waiting_to_Turn;
						KrazyClogDetector::getDefault()->append_waitingvehicals(ptr_vehical , ptr_last_vehical);
						return false;
					}
					if(curr_distance < front_distance){
						front_distance = curr_distance;
						ptr_sel_front_vehical = ptr_last_vehical;
					}
				}
				else if(ptr_vec_rear_bumper->y < y_turn_indc){
					float curr_distance = glm::distance(*ptr_vec_rear_bumper , *ptr_vec_turn_indc);;//ptr_vec_rear_bumper->distance(*ptr_vec_turn_indc);
					if(curr_distance < MATCH_SPACE_THRESHOLD){
						ptr_vehical->waitstate() = KRAZY_VEHICAL_WAIT_STATE_Waiting_to_Turn;
						KrazyClogDetector::getDefault()->append_waitingvehicals(ptr_vehical , ptr_last_vehical);
						return false;
					}
					if(curr_distance < rear_distance){
						rear_distance = curr_distance;
						ptr_sel_rear_vehical = ptr_last_vehical;
						rear_index = i;
					}
				}
				break;
			}
		}		
		// space found.., attach rear front vehical .., ..,
		KrazyVehical* ptr_front_vehical = ptr_vehical->PTRFrontVehical();
		KrazyVehical* ptr_rear_vehical  = ptr_vehical->PTRRearVehical();
		ptr_vehical->on_setPreviousRearVehical(ptr_rear_vehical);
		if(ptr_sel_front_vehical && ptr_sel_rear_vehical){
			ptr_sel_front_vehical->on_attachFrontVehical(ptr_vehical);
			ptr_sel_rear_vehical->on_attachRearVehical(ptr_vehical);
		}
		else if(ptr_sel_front_vehical){
			ptr_sel_front_vehical->on_attachFrontVehical(ptr_vehical);
		}
		else if(ptr_sel_rear_vehical){
			ptr_sel_rear_vehical->on_attachRearVehical(ptr_vehical);
			if(ptr_rear_entering_vehical){
				switch(ptr_rear_entering_vehical->movestate()){
				case KRAZY_VEHICAL_STATE_Entering:
					ptr_rear_entering_vehical->on_attachFrontVehical(ptr_vehical);
					ptr_sel_front_vehical = ptr_rear_entering_vehical;
					break;
				}
			}
		}
		if(ptr_front_vehical){
			ptr_front_vehical->on_attachRearVehical(ptr_rear_vehical);
		}
		if(ptr_rear_vehical){
			ptr_rear_vehical->on_attachFrontVehical(ptr_front_vehical);
		}
		ptr_vehical->on_attachFrontVehical(ptr_sel_rear_vehical);
		ptr_vehical->on_attachRearVehical(ptr_sel_front_vehical);
		on_vehicalTurn(ptr_vehical,ptr_next_turn);
		ptr_vehical->movestate() = KRAZY_VEHICAL_STATE_Moving;
		ptr_vehical->waitstate() = KRAZY_VEHICAL_WAIT_STATE_Turned;
		return false;
	}
	else{
		// reset turnning switches...,
		KRAZY_RESET_FLIP_SWITCHES();
		KrazyVehical* ptr_vehical_front = ptr_vehical->PTRFrontVehical();
		if(ptr_vehical_front){
			glm::vec2* ptr_vehical_vector_front_bumper = ptr_vehical->VectorFrontBumper();
			glm::vec2* ptr_front_vehical_vector_rear_bumper = ptr_vehical_front->VectorRearBumper();
			float distance = glm::distance(*ptr_vehical_vector_front_bumper , *ptr_front_vehical_vector_rear_bumper);//ptr_vehical_vector_front_bumper->distance(*ptr_front_vehical_vector_rear_bumper);
			bool _distance_maintained(distance > ptr_vehical->gap());
#if 1
			if(!_distance_maintained){
				KrazyClogDetector::getDefault()->append_waitingvehicals(ptr_vehical , ptr_vehical_front);
			}
#endif
			//..,
			return _distance_maintained;//( distance > GAP_ );
		}
	}
	return true;
}
#else
bool KrazyDefaultAlgoMovingVehical_01Phase::matchDistance(KrazyVehical* ptr_vehical)
{
	KrazyTrafficIndicator* ptr_next_turn = ptr_vehical->get_NextTurn();
	if(ptr_next_turn){
		glm::vec2* ptr_vec_turn_indc   = 0;
		KrazyLaneNode* ptr_dest_lane = 0;
		switch(ptr_next_turn->Type()){
		case KRAZY_TRAFFIC_INDC_FlipPoint:
			ptr_dest_lane = ptr_vehical->PTRCurrentAssignedSingleLane()->getOppositeSingleLane();
			ptr_vec_turn_indc = ptr_dest_lane->getEntryIndicator()->Vector();
			break;
		case KRAZY_TRAFFIC_INDC_Entry:
			{
				ptr_dest_lane = ptr_next_turn->PTRParentLaneNode();
				ptr_vec_turn_indc = ptr_next_turn->Vector();
				KrazyVehicalList* ptr_vehicals_e = ptr_dest_lane->getVehicalList();
				int count_e = ptr_vehicals_e->size();
				KrazyTrafficIndicator* _turn = ptr_next_turn;
				if(count_e > 0){
					KrazyVehical* ptr_last_vehical = ptr_vehicals_e->at(count_e - 1);
					if(!ptr_last_vehical->PTRRearVehical()){
						ptr_vehical->on_attachFrontVehical(ptr_last_vehical);
						ptr_last_vehical->on_attachRearVehical(ptr_vehical);
						KrazyLaneNode* _assigned_lane = ptr_last_vehical->PTRCurrentAssignedSingleLane();
						_turn = _assigned_lane->createOnSpotFlipIndicator();
						glm::vec2 _vec;
						switch(_assigned_lane->getProperties()->get_LaneDirection())
						{
							case KRAZY_LANE_DIRECTION_UP_HORZS  :
								 _vec = glm::vec2(ptr_last_vehical->VectorRearBumper()->x + _KRAZY_STD_GAP , ptr_last_vehical->VectorRearBumper()->y);
								 break;
							case KRAZY_LANE_DIRECTION_DOWN_HORZS   : 
								 _vec = glm::vec2(ptr_last_vehical->VectorRearBumper()->x + _KRAZY_STD_GAP , ptr_last_vehical->VectorRearBumper()->y);
								 break;
							case KRAZY_LANE_DIRECTION_UP_VERTS     : 
								 _vec = glm::vec2(ptr_last_vehical->VectorRearBumper()->x  , ptr_last_vehical->VectorRearBumper()->y - _KRAZY_STD_GAP);
								 break;
							case KRAZY_LANE_DIRECTION_DOWN_VERTS   : 
								 _vec = glm::vec2(ptr_last_vehical->VectorRearBumper()->x , ptr_last_vehical->VectorRearBumper()->y + _KRAZY_STD_GAP);
								 break;
						}
						_turn->Type() = KRAZY_TRAFFIC_INDC_Entry;
						_turn->on_reset(_vec , _assigned_lane);
					}
					else{
						int a = 0;
					}
				}
				on_vehicalTurn(ptr_vehical , _turn);
				ptr_vehical->on_laneChange(0);
				return false;
			}
			break;
		case KRAZY_TRAFFIC_INDC_Turn:
			{
				KrazyTrafficIndicator* ptr_dest_indc = ptr_next_turn->get_DestinationIndc(0);
				ptr_vec_turn_indc = ptr_dest_indc->Vector();
				ptr_dest_lane     = ptr_dest_indc->PTRParentLaneNode();
			}
			break; 
		case KRAZY_TRAFFIC_INDC_OnSpotFlip:
			{
				ptr_vec_turn_indc = ptr_next_turn->Vector();
				ptr_dest_lane     = ptr_next_turn->PTRParentLaneNode();
			}
			break;
		case KRAZY_TRAFFIC_INDC_SliderPoint:
			 ptr_vehical->on_attachFrontVehical(0);
			 ptr_vehical->on_attachRearVehical(0);
			 on_vehicalTurn(ptr_vehical,ptr_next_turn);
			 ptr_vehical->movestate() = KRAZY_VEHICAL_STATE_Moving;
			 ptr_vehical->waitstate() = KRAZY_VEHICAL_WAIT_STATE_Turned;
			 return false;
		}
		KrazyVehicalList* ptr_vehicals = ptr_dest_lane->getVehicalList();
		int count = ptr_vehicals->size();
		KrazyLaneDirection lane_direction = ptr_dest_lane->getProperties()->get_LaneDirection();
		float front_distance = MAX_DISTANCE_EPSILON;
		float rear_distance  = MAX_DISTANCE_EPSILON;
		KrazyVehical* ptr_sel_front_vehical = 0;
		KrazyVehical* ptr_sel_rear_vehical  = 0;
		int rear_index = 0;
		KrazyVehical* ptr_rear_entering_vehical = 0;
		for(int i = 0;i < count; i++){
			KrazyVehical* ptr_last_vehical = ptr_vehicals->at(i);
			switch(ptr_last_vehical->visibilitystate())
			{
			case KRAZY_BOOL_False:
				if(!ptr_rear_entering_vehical)
					ptr_rear_entering_vehical = ptr_last_vehical;
				
				continue;
			}
			glm::vec2* ptr_vec_front_bumper = ptr_last_vehical->VectorFrontBumper();
			glm::vec2* ptr_vec_rear_bumper  = ptr_last_vehical->VectorRearBumper();
			float  x_turn_indc = ptr_vec_turn_indc->x;
			float  y_turn_indc = ptr_vec_turn_indc->y;
			switch(lane_direction)
			{
			case KRAZY_LANE_DIRECTION_UP_HORZS     :
				if(ptr_vec_rear_bumper->x > x_turn_indc && ptr_vec_front_bumper->x < x_turn_indc){
					ptr_vehical->waitstate() = KRAZY_VEHICAL_WAIT_STATE_Waiting_to_Turn;
					return false;
				}
				else if(ptr_vec_front_bumper->x > x_turn_indc){
					float curr_distance = glm::distance(*ptr_vec_front_bumper , *ptr_vec_turn_indc);//ptr_vec_front_bumper->distance(*ptr_vec_turn_indc);
					if(curr_distance < MATCH_SPACE_THRESHOLD){
						ptr_vehical->waitstate() = KRAZY_VEHICAL_WAIT_STATE_Waiting_to_Turn;
						return false;
					}
					if(curr_distance < front_distance){
						front_distance = curr_distance;
						ptr_sel_front_vehical = ptr_last_vehical;
					}
				}
				else if(ptr_vec_rear_bumper->x < x_turn_indc){
					float curr_distance = glm::distance(*ptr_vec_rear_bumper , *ptr_vec_turn_indc);//ptr_vec_rear_bumper->distance(*ptr_vec_turn_indc);
					if(curr_distance < MATCH_SPACE_THRESHOLD){
						ptr_vehical->waitstate() = KRAZY_VEHICAL_WAIT_STATE_Waiting_to_Turn;
						return false;
					}
					if(curr_distance < rear_distance){
						rear_distance = curr_distance;
						ptr_sel_rear_vehical = ptr_last_vehical;
						rear_index = i;
					}
				}
				break;
			case KRAZY_LANE_DIRECTION_DOWN_HORZS   :
				if(ptr_vec_front_bumper->x > x_turn_indc && ptr_vec_rear_bumper->x < x_turn_indc){
					ptr_vehical->waitstate() = KRAZY_VEHICAL_WAIT_STATE_Waiting_to_Turn;
					return false;
				}
				else if(ptr_vec_front_bumper->x < x_turn_indc){
					float curr_distance = glm::distance(*ptr_vec_front_bumper , *ptr_vec_turn_indc);//ptr_vec_front_bumper->distance(*ptr_vec_turn_indc);
					if(curr_distance < MATCH_SPACE_THRESHOLD){
						ptr_vehical->waitstate() = KRAZY_VEHICAL_WAIT_STATE_Waiting_to_Turn;
						return false;
					}
					if(curr_distance < front_distance){
						front_distance = curr_distance;
						ptr_sel_front_vehical = ptr_last_vehical;
					}
				}
				else if(ptr_vec_rear_bumper->x > x_turn_indc){
					float curr_distance = glm::distance(*ptr_vec_rear_bumper , *ptr_vec_turn_indc);//ptr_vec_rear_bumper->distance(*ptr_vec_turn_indc);
					if(curr_distance < MATCH_SPACE_THRESHOLD){
						ptr_vehical->waitstate() = KRAZY_VEHICAL_WAIT_STATE_Waiting_to_Turn;
						return false;
					}
					if(curr_distance < rear_distance){
						rear_distance = curr_distance;
						ptr_sel_rear_vehical = ptr_last_vehical;
						rear_index = i;
					}
				}
				break;
			case KRAZY_LANE_DIRECTION_UP_VERTS     : 
				if(ptr_vec_front_bumper->y > y_turn_indc && ptr_vec_rear_bumper->y < y_turn_indc){
					ptr_vehical->waitstate() = KRAZY_VEHICAL_WAIT_STATE_Waiting_to_Turn;
					return false;
				}
				else if(ptr_vec_front_bumper->y < y_turn_indc){
					float curr_distance = glm::distance(*ptr_vec_front_bumper , *ptr_vec_turn_indc);
					if(curr_distance < MATCH_SPACE_THRESHOLD){
						ptr_vehical->waitstate() = KRAZY_VEHICAL_WAIT_STATE_Waiting_to_Turn;
						return false;
					}
					if(curr_distance < front_distance){
						front_distance = curr_distance;
						ptr_sel_front_vehical = ptr_last_vehical;
					}
				}
				else if(ptr_vec_rear_bumper->y > y_turn_indc){
					float curr_distance = glm::distance(*ptr_vec_rear_bumper , *ptr_vec_turn_indc);;//ptr_vec_rear_bumper->distance(*ptr_vec_turn_indc);
					if(curr_distance < MATCH_SPACE_THRESHOLD){
						ptr_vehical->waitstate() = KRAZY_VEHICAL_WAIT_STATE_Waiting_to_Turn;
						return false;
					}
					if(curr_distance < rear_distance){
						rear_distance = curr_distance;
						ptr_sel_rear_vehical = ptr_last_vehical;
						rear_index = i;
					}
				}
				break;
			case KRAZY_LANE_DIRECTION_DOWN_VERTS   : 
				if(ptr_vec_rear_bumper->y > y_turn_indc && ptr_vec_front_bumper->y < y_turn_indc){
					ptr_vehical->waitstate() = KRAZY_VEHICAL_WAIT_STATE_Waiting_to_Turn;
					return false;
				}
				else if(ptr_vec_front_bumper->y > y_turn_indc){
					float curr_distance = glm::distance(*ptr_vec_front_bumper , *ptr_vec_turn_indc);//ptr_vec_front_bumper->distance(*ptr_vec_turn_indc);
					if(curr_distance < MATCH_SPACE_THRESHOLD){
						ptr_vehical->waitstate() = KRAZY_VEHICAL_WAIT_STATE_Waiting_to_Turn;
						return false;
					}
					if(curr_distance < front_distance){
						front_distance = curr_distance;
						ptr_sel_front_vehical = ptr_last_vehical;
					}
				}
				else if(ptr_vec_rear_bumper->y < y_turn_indc){
					float curr_distance = glm::distance(*ptr_vec_rear_bumper , *ptr_vec_turn_indc);;//ptr_vec_rear_bumper->distance(*ptr_vec_turn_indc);
					if(curr_distance < MATCH_SPACE_THRESHOLD){
						ptr_vehical->waitstate() = KRAZY_VEHICAL_WAIT_STATE_Waiting_to_Turn;
						return false;
					}
					if(curr_distance < rear_distance){
						rear_distance = curr_distance;
						ptr_sel_rear_vehical = ptr_last_vehical;
						rear_index = i;
					}
				}
				break;
			}
		}		
		// space found.., attach rear front vehical .., ..,
		KrazyVehical* ptr_front_vehical = ptr_vehical->PTRFrontVehical();
		KrazyVehical* ptr_rear_vehical  = ptr_vehical->PTRRearVehical();
		ptr_vehical->on_setPreviousRearVehical(ptr_rear_vehical);
		if(ptr_sel_front_vehical && ptr_sel_rear_vehical){
			ptr_sel_front_vehical->on_attachFrontVehical(ptr_vehical);
			ptr_sel_rear_vehical->on_attachRearVehical(ptr_vehical);
		}
		else if(ptr_sel_front_vehical){
			ptr_sel_front_vehical->on_attachFrontVehical(ptr_vehical);
		}
		else if(ptr_sel_rear_vehical){
			ptr_sel_rear_vehical->on_attachRearVehical(ptr_vehical);
			if(ptr_rear_entering_vehical){
				switch(ptr_rear_entering_vehical->movestate()){
				case KRAZY_VEHICAL_STATE_Entering:
					ptr_rear_entering_vehical->on_attachFrontVehical(ptr_vehical);
					ptr_sel_front_vehical = ptr_rear_entering_vehical;
					break;
				}
			}
		}
		if(ptr_front_vehical){
			ptr_front_vehical->on_attachRearVehical(ptr_rear_vehical);
		}
		if(ptr_rear_vehical){
			ptr_rear_vehical->on_attachFrontVehical(ptr_front_vehical);
		}
		ptr_vehical->on_attachFrontVehical(ptr_sel_rear_vehical);
		ptr_vehical->on_attachRearVehical(ptr_sel_front_vehical);
		on_vehicalTurn(ptr_vehical,ptr_next_turn);
		ptr_vehical->movestate() = KRAZY_VEHICAL_STATE_Moving;
		ptr_vehical->waitstate() = KRAZY_VEHICAL_WAIT_STATE_Turned;
		return false;
	}
	else{
		// reset turnning switches...,
		KRAZY_RESET_FLIP_SWITCHES();
		KrazyVehical* ptr_vehical_front = ptr_vehical->PTRFrontVehical();
		if(ptr_vehical_front){
			glm::vec2* ptr_vehical_vector_front_bumper = ptr_vehical->VectorFrontBumper();
			glm::vec2* ptr_front_vehical_vector_rear_bumper = ptr_vehical_front->VectorRearBumper();
			float distance = glm::distance(*ptr_vehical_vector_front_bumper , *ptr_front_vehical_vector_rear_bumper);//ptr_vehical_vector_front_bumper->distance(*ptr_front_vehical_vector_rear_bumper);
			return (distance > ptr_vehical->gap());//( distance > GAP_ );
		}
	}
	return true;
}
#endif
/////////////////////////////////////////////////////////
void KrazyDefaultAlgoMovingVehical_01Phase::createRouteMap(KrazyVehical* ptr_vehical)
{
	KrazyLaneNode* ptr_assigned_single_lane = ptr_vehical->PTRCurrentAssignedSingleLane();
	if(!ptr_assigned_single_lane)
		return;

	KrazyTrafficIndicatorList* ptr_car_route = ptr_vehical->get_TrafficIndicatorMap();
	ptr_car_route->clear();
	KrazyTrafficIndicatorList* ptr_trafficindc_list = ptr_assigned_single_lane->get_TrafficIndicators();
	KrazyLaneProperties* properties = ptr_assigned_single_lane->getProperties();
	int count = ptr_trafficindc_list->size();
	for(int i = 0; i < count; i++){
		KrazyTrafficIndicator* ptr_traffic_indc = ptr_trafficindc_list->at(i);
		glm::vec2* ptr_traffic_indc_vec = ptr_traffic_indc->Vector();
#if 0
		glm::vec2* ptr_vehical_vec = ptr_vehical->Vector();
#else
		glm::vec2* ptr_vehical_vec = ptr_vehical->VectorFrontBumper();
#endif
		switch(properties->get_LaneDirection())
		{
		case KRAZY_LANE_DIRECTION_UP_HORZS     : if(ptr_traffic_indc_vec->x < ptr_vehical_vec->x) {ptr_car_route->push_back(ptr_traffic_indc);}break;
		case KRAZY_LANE_DIRECTION_DOWN_HORZS   : if(ptr_traffic_indc_vec->x > ptr_vehical_vec->x) {ptr_car_route->push_back(ptr_traffic_indc);}break;
		case KRAZY_LANE_DIRECTION_UP_VERTS     : if(ptr_traffic_indc_vec->y > ptr_vehical_vec->y) {ptr_car_route->push_back(ptr_traffic_indc);}break;
		case KRAZY_LANE_DIRECTION_DOWN_VERTS   : if(ptr_traffic_indc_vec->y < ptr_vehical_vec->y) {ptr_car_route->push_back(ptr_traffic_indc);}break;
		}
	}
}
////////////////////////////////////////////////////////
KrazyTrafficIndicator* KrazyDefaultAlgoMovingVehical_01Phase::getOriginTrafficIndicator(KrazyVehical* ptr_vehical , int index /*= 0*/)
{
	//..........................................................................................................................................................
	KrazyLaneNode* ptr_current_assigned_lane = ptr_vehical->PTRCurrentAssignedSingleLane();
	KrazyLaneDirection lane_direction = ptr_current_assigned_lane->getProperties()->get_LaneDirection();
	KrazyTrafficIndicatorList* ptr_trafficIndcList = ptr_current_assigned_lane->get_TrafficIndicators();
	int traffic_indc_count = ptr_trafficIndcList->size();
	for(int traffic_indc = index;traffic_indc < traffic_indc_count;traffic_indc++){
		KrazyTrafficIndicator* ptr_traffic_indc = ptr_trafficIndcList->at(traffic_indc);
		glm::vec2* ptr_traffic_indc_vec = ptr_traffic_indc->Vector();
		glm::vec2* ptr_vehical_vec = ptr_vehical->VectorFrontBumper();
		float distance = glm::distance(*ptr_traffic_indc_vec , *ptr_vehical_vec);
		if((distance >= _KRAZY_TRAFFIC_INDC_SPACE_MIN/* && distance <= _KRAZY_TRAFFIC_INDC_SPACE_MAX*/)){
			return ptr_traffic_indc;
		}
	}
    return ptr_trafficIndcList->at(traffic_indc_count - 1);  // return flip it passed even the flip ..,	
}
////////////////////////////////////////////////////////
bool KrazyDefaultAlgoMovingVehical_01Phase::get_DetectedTrafficIndicatorByLane(KrazyVehical* ptr_vehical , KrazyTrafficIndicator** ptr_trafficIndicator)
{
	// remove this .....,,
	//To Do Not Implemented yet...,
	KrazyTrafficIndicatorList* ptr_trafficIndcList = ptr_vehical->get_TrafficIndicatorMap();
	int traffic_indc_count = ptr_trafficIndcList->size();

	for(int traffic_indc = ptr_vehical->getNextTurnIndex();traffic_indc < traffic_indc_count;traffic_indc++){
		KrazyTrafficIndicator* ptr_traffic_indicator = ptr_trafficIndcList->at(traffic_indc);
		glm::vec2* ptr_vector_traffic_indc = ptr_traffic_indicator->Vector();
		glm::vec2* ptr_vector_vehical_front_bumper_vector = ptr_vehical->VectorFrontBumper();
		float distance = glm::distance(*ptr_vector_traffic_indc , *ptr_vector_vehical_front_bumper_vector);//ptr_vector_traffic_indc->distance(*ptr_vector_vehical_front_bumper_vector);
		if((distance >= 0.001f && distance <= 0.002f)){
			if(ptr_traffic_indicator->is_alreadyTaken()){
				ptr_traffic_indicator->updateTaken();
				ptr_vehical->on_setNextTurnIndex(traffic_indc + 1);
				return false;
				
			}
			if(ptr_traffic_indicator->Type() == 1)
				ptr_traffic_indicator->updateTaken();

			ptr_vehical->on_setNextTurnIndex(traffic_indc + 1);
			*ptr_trafficIndicator = ptr_traffic_indicator;
			return true;
		}
	//	return false;
	}
    return false;	
}
//
KrazyBool KrazyDefaultAlgoMovingVehical_01Phase::get_DetectedTrafficIndicator_in_Gameplay_ByVehical(KrazyVehical* ptr_vehical , KrazyTrafficIndicator** ptr_trafficIndicator)
{
	KrazyTrafficIndicatorList* ptr_trafficIndcList = ptr_vehical->get_TrafficIndicatorMap();
	int traffic_indc_count = ptr_trafficIndcList->size();
	for(int traffic_indc = ptr_vehical->getNextTurnIndex();traffic_indc < traffic_indc_count;traffic_indc++){
		KrazyTrafficIndicator* ptr_traffic_indicator = ptr_trafficIndcList->at(traffic_indc);
		glm::vec2* ptr_vector_traffic_indc = ptr_traffic_indicator->Vector();
		glm::vec2* ptr_vector_vehical_front_bumper_vector = ptr_vehical->VectorFrontBumper();
		float distance = glm::distance(*ptr_vector_traffic_indc , *ptr_vector_vehical_front_bumper_vector);
		if(distance < _KRAZY_TRAFFIC_INDC_SPACE_MIN){
			//...dont wait at N-Entry.., if in violation...
#if 0

#else
			switch(ptr_vehical->in_violation()){
			case KRAZY_BOOL_True:
				 switch(ptr_traffic_indicator->Type()) {
				 case KRAZY_TRAFFIC_INDC_NoEntry : ptr_vehical->on_setNextTurnIndex(traffic_indc + 1); return KRAZY_BOOL_False;
				 }
				 break;
			 case KRAZY_BOOL_False:
				switch(ptr_traffic_indicator->Type()) {
				 case KRAZY_TRAFFIC_INDC_Exit : ptr_vehical->on_setNextTurnIndex(traffic_indc + 1); return KRAZY_BOOL_False;
				 }
				break;
			}
			//
			switch(ptr_vehical->was_in_violation()){
			case KRAZY_BOOL_True:
				 switch(ptr_traffic_indicator->Type()) {
				 case KRAZY_TRAFFIC_INDC_Exit : ptr_vehical->on_setNextTurnIndex(traffic_indc + 1); return KRAZY_BOOL_True;
				 }
				 break;
			}
#endif
			//
			if(ptr_traffic_indicator->is_alreadyTaken()){
				ptr_vehical->on_setNextTurnIndex(traffic_indc + 1);
				ptr_traffic_indicator->updateTaken();
				continue;
			}
			if(ptr_traffic_indicator->Type() == 1)
				ptr_traffic_indicator->updateTaken();
			    
			ptr_vehical->on_setNextTurnIndex(0);
			*ptr_trafficIndicator = ptr_traffic_indicator;
			return KRAZY_BOOL_True;
		}
	}
    return KRAZY_BOOL_False;	
}
//
KrazyBool KrazyDefaultAlgoMovingVehical_01Phase::get_DetectedTrafficIndicatorByVehical(KrazyVehical* ptr_vehical , KrazyTrafficIndicator** ptr_trafficIndicator)
{
	KrazyTrafficIndicatorList* ptr_trafficIndcList = ptr_vehical->get_TrafficIndicatorMap();
	int traffic_indc_count = ptr_trafficIndcList->size();
	for(int traffic_indc = ptr_vehical->getNextTurnIndex();traffic_indc < traffic_indc_count;traffic_indc++){
		KrazyTrafficIndicator* ptr_traffic_indicator = ptr_trafficIndcList->at(traffic_indc);
		glm::vec2* ptr_vector_traffic_indc = ptr_traffic_indicator->Vector();
		glm::vec2* ptr_vector_vehical_front_bumper_vector = ptr_vehical->VectorFrontBumper();
		float distance = glm::distance(*ptr_vector_traffic_indc , *ptr_vector_vehical_front_bumper_vector);
		if(distance < _KRAZY_TRAFFIC_INDC_SPACE_MIN){
			if(ptr_traffic_indicator->is_alreadyTaken()){
				ptr_vehical->on_setNextTurnIndex(traffic_indc + 1);
				ptr_traffic_indicator->updateTaken();
				continue;
			}
			if(ptr_traffic_indicator->Type() == 1)
				ptr_traffic_indicator->updateTaken();
			    
			ptr_vehical->on_setNextTurnIndex(0);
			*ptr_trafficIndicator = ptr_traffic_indicator;
			return KRAZY_BOOL_True;
		}
	}
    return KRAZY_BOOL_False;	
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
KrazyBool KrazyDefaultAlgoMovingVehical_01Phase::get_DetectedTrafficIndicatorSeq(KrazyVehical* ptr_vehical , KrazyTrafficIndicator** ptr_trafficIndicator)
{
	KrazyTrafficIndicatorList* ptr_trafficIndcList = ptr_vehical->get_TrafficIndicatorMap();
	int traffic_indc_count = ptr_trafficIndcList->size();
	for(int traffic_indc = ptr_vehical->getNextTurnIndex();traffic_indc < traffic_indc_count;traffic_indc++){
		KrazyTrafficIndicator* ptr_traffic_indicator = ptr_trafficIndcList->at(traffic_indc);
		glm::vec2* ptr_vector_traffic_indc = ptr_traffic_indicator->Vector();
		glm::vec2* ptr_vector_vehical_front_bumper_vector = ptr_vehical->VectorFrontBumper();
		float distance = glm::distance(*ptr_vector_traffic_indc , *ptr_vector_vehical_front_bumper_vector);
		if(distance < _KRAZY_TRAFFIC_INDC_SPACE_MIN){
			ptr_vehical->on_setNextTurnIndex(traffic_indc + 1); 
			//...dont wait at N-Entry.., if in violation...
			switch(ptr_vehical->in_violation()){
			case KRAZY_BOOL_True:
				 switch(ptr_traffic_indicator->Type()) {
				 case KRAZY_TRAFFIC_INDC_NoEntry : return KRAZY_BOOL_False;
				 }
				 break;
			 case KRAZY_BOOL_False:
				switch(ptr_traffic_indicator->Type()) {
				 case KRAZY_TRAFFIC_INDC_Exit : return KRAZY_BOOL_False;
				 }
				break;
			}
			//
			switch(ptr_vehical->was_in_violation()){
			case KRAZY_BOOL_True:
				 switch(ptr_traffic_indicator->Type()) {
				 case KRAZY_TRAFFIC_INDC_Exit : ptr_vehical->on_setNextTurnIndex(traffic_indc + 1); return KRAZY_BOOL_True;
				 }
				 break;
			}
			*ptr_trafficIndicator = ptr_traffic_indicator;
			return KRAZY_BOOL_True;
		}
	}
	return KRAZY_BOOL_False;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _UN_OPT_PATH_SEARCH
struct routing_index
	{
		routing_index(){ reset(); }
		bool initialized(){ return _Initialized; }
		void initialize(int size , KrazyLaneNode* ptr_prev){ _Initialized = true;_Size = size;_PTRPrev = ptr_prev; }
		KrazyLaneNode* getPrev(){ return _PTRPrev; }
		int& getCursor(){ return _Cursor; }
		int  getSize()  { return _Size; }
		bool exhausted(){ return !(_Size > 0 && _Cursor < _Size); }
		bool& skip(){ return _Skip; }
		void reset()
		{
			_Cursor     = 0;
		    _Size       = 0;
		   _PTRPrev     = 0;
		   _Initialized = false;
		   _Skip        = false;
		}
	private:
		bool _Initialized;
		int  _Cursor;
		int  _Size;
		KrazyLaneNode* _PTRPrev;
		bool  _Skip;
};
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void KrazyDefaultAlgoMovingVehical_01Phase::get_ShortestRouteMap(KrazyLaneNode* ptr_source_lane , KrazyLaneNode* ptr_destination_lane , KrazyTrafficIndicatorList* ptr_route_map)
{
#ifndef  _UN_OPT_PATH_SEARCH
	KRAZY_ASSIGN_DESTINATION();
	//
	KrazyLaneNode* ptr_source_double_lane  = ptr_source_lane->getParent();
	KrazyLaneNodeList* ptr_connected_lanes = ptr_source_double_lane->getConnectedLanes();
	int count_connected_from_source = ptr_connected_lanes->size();
	_RealDistance = MAX_DISTANCE_EPSILON;_CurrentDistance = 0.0f;
	KrazyShortPathRoutingIndexer* ptr_source_indexer = ptr_source_double_lane->getRoutingIndexer();
	ptr_source_indexer->reset();ptr_source_indexer->processing() = true;
	for(int connected_lane_source = 0; connected_lane_source < count_connected_from_source; connected_lane_source++){
		KrazyLaneNode* ptr_conn_lane = ptr_connected_lanes->at(connected_lane_source);
		KrazyShortPathRoutingIndexer* ptr_indexer_conn = ptr_conn_lane->getRoutingIndexer();
		ptr_indexer_conn->reset();ptr_indexer_conn->processing() = true;
		KrazyLaneNodeList* ptr_conn_lanes = ptr_conn_lane->getConnectedLanes();
		int count_conn_lane = ptr_conn_lanes->size();
		for(int conn_lane = 0; conn_lane < count_conn_lane; conn_lane++){
			KrazyLaneNode* ptr_curr_lane = ptr_conn_lanes->at(conn_lane);
			if((ptr_curr_lane == ptr_source_double_lane)){
				//touching with origin..,
				continue;
			}
#if 1
			switch(ptr_curr_lane->foundSingleLane(ptr_destination_lane))
			{
			case KRAZY_BOOL_True:getRouteWithDistance(ptr_source_lane,ptr_destination_lane,ptr_curr_lane, ptr_conn_lane,ptr_route_map , 1); continue;
			}

#else
			if(ptr_curr_lane->foundSingleLane(ptr_destination_lane)){
				 getRouteWithDistance(ptr_source_lane,ptr_destination_lane,ptr_curr_lane, ptr_conn_lane,ptr_route_map , 1);
				 continue;
			}
#endif
			KrazyLaneNode* ptr_prev = 0;
			KrazyLaneNode* ptr_processing_lane = ptr_curr_lane;
			KrazyShortPathRoutingIndexer* ptr_indexer = ptr_curr_lane->getRoutingIndexer();
			ptr_indexer->processing() = true;
			for(;ptr_processing_lane;){
				if(!ptr_indexer){
					ptr_indexer = ptr_processing_lane->getRoutingIndexer();
				}
				KrazyLaneNodeList* ptr_in_conn_lanes = ptr_processing_lane->getConnectedLanes();
				if(!ptr_indexer->initialized()){
					ptr_indexer->initialize(ptr_in_conn_lanes->size(),ptr_prev);
				}
#if 1
					
			switch(ptr_processing_lane->foundSingleLane(ptr_destination_lane))
			{
			case KRAZY_BOOL_True : 
				getRouteWithDistance(ptr_source_lane,ptr_destination_lane,ptr_conn_lane,ptr_indexer->getPrev(),ptr_route_map);
				ptr_indexer->skip() = true;	ptr_indexer->getPrev()->getRoutingIndexer()->skip() = true;
				break;
			}
#else
			if(ptr_processing_lane->foundSingleLane(ptr_destination_lane)){
				getRouteWithDistance(ptr_source_lane,ptr_destination_lane,ptr_conn_lane,ptr_indexer->getPrev(),ptr_route_map);
				ptr_indexer->skip() = true;	ptr_indexer->getPrev()->getRoutingIndexer()->skip() = true;
			}
#endif
				
			if(ptr_indexer->getSize() == 0){
				ptr_processing_lane = ptr_indexer->getPrev();
				ptr_indexer->reset();
				ptr_indexer = 0;
				continue;
			}
			if(!ptr_indexer->exhausted() && !ptr_indexer->skip()){
				KrazyLaneNode* ptr_tobe_processed_conn_lane = ptr_in_conn_lanes->at(ptr_indexer->getCursor()++);
				KrazyShortPathRoutingIndexer* ptr_to_be_routing_indexer = ptr_tobe_processed_conn_lane->getRoutingIndexer();
				if(!ptr_to_be_routing_indexer->processing()){
					ptr_to_be_routing_indexer->processing() = true;
				    ptr_prev = ptr_processing_lane;
					ptr_processing_lane = ptr_tobe_processed_conn_lane;
					ptr_indexer = ptr_to_be_routing_indexer;
					continue;
				}
				
			}
			else{
				ptr_indexer->processing() = false;
				ptr_processing_lane = ptr_indexer->getPrev();
				ptr_indexer->reset();
				ptr_indexer = 0;
				continue;
				}
			}
		}
		ptr_indexer_conn->processing() = false;
		ptr_indexer_conn->reset();
	}
	ptr_source_indexer->processing() = false;
	ptr_source_indexer->reset();
#else
	KRAZY_ASSIGN_DESTINATION();
	_PTRAuxshortroutemap->clear();_PTRAuxshortroutemap->push_back(ptr_dest_end);
	KrazyLaneNode* ptr_source_double_lane  = ptr_source_lane->getParent();
	KrazyLaneNodeList* ptr_connected_lanes = ptr_source_double_lane->getConnectedLanes();
	int count_connected_from_source = ptr_connected_lanes->size();
	float distance = 10000.0f;_CurrentDistance = 0.0f;
	for(int connected_lane_source = 0; connected_lane_source < count_connected_from_source; connected_lane_source++){
		KrazyLaneNode* ptr_conn_lane = ptr_connected_lanes->at(connected_lane_source);
		KrazyLaneNodeList* ptr_conn_lanes = ptr_conn_lane->getConnectedLanes();
		int count_conn_lane = ptr_conn_lanes->size();
		for(int conn_lane = 0; conn_lane < count_conn_lane; conn_lane++){
			KrazyLaneNode* ptr_curr_lane = ptr_conn_lanes->at(conn_lane);
			if((ptr_curr_lane == ptr_source_double_lane)){
				//touching with origin..,
				continue;
			}
			if(ptr_curr_lane->foundSingleLane(ptr_destination_lane)
				&& getRouteWithDistance(ptr_source_lane,ptr_destination_lane,ptr_conn_lane,ptr_route_map,distance)){
					getRouteWithDistance(ptr_source_lane,ptr_destination_lane,ptr_source_double_lane,ptr_route_map,distance);
					continue;
			}
			hash_map<KrazyLaneNode* , bool> _prevToBeProcessedLanes;
			hash_map<KrazyLaneNode* , routing_index>  _routing_indexer_map;
			KrazyLaneNode* ptr_prev = 0;
			KrazyLaneNode* ptr_processing_lane = ptr_curr_lane;
			_prevToBeProcessedLanes[ptr_source_double_lane] = true;
			_prevToBeProcessedLanes[ptr_conn_lane] = true;
			_prevToBeProcessedLanes[ptr_curr_lane] = true;
			KrazyLaneNode* ptr_continued = 0;
			routing_index* ptr_indexer = 0;
			for(;ptr_processing_lane;){
				if(ptr_continued != ptr_processing_lane && !ptr_indexer){
					ptr_indexer = &_routing_indexer_map[ptr_processing_lane];
					ptr_continued = ptr_processing_lane;
				}
				KrazyLaneNodeList* ptr_in_conn_lanes = ptr_processing_lane->getConnectedLanes();
				if(!ptr_indexer->initialized()){
					ptr_indexer->initialize(ptr_in_conn_lanes->size(),ptr_prev);
				}
				if(ptr_processing_lane->foundSingleLane(ptr_destination_lane) && 
					getRouteWithDistance(ptr_source_lane,ptr_destination_lane,ptr_indexer->getPrev(),ptr_route_map,distance,&_routing_indexer_map)){
					getRouteWithDistance(ptr_source_lane,ptr_destination_lane,ptr_conn_lane,ptr_route_map,distance);
					getRouteWithDistance(ptr_source_lane,ptr_destination_lane,ptr_source_double_lane,ptr_route_map,distance);// need to club this in one call..,
					ptr_indexer->skip() = true;	_routing_indexer_map[ptr_indexer->getPrev()].skip() = true;
				}
				if(ptr_indexer->getSize() == 0){
					//dont remove it from the to be processed table .., this lane is not in the series... or no need to be in the searching chain..,,
					ptr_processing_lane = ptr_indexer->getPrev();
					continue;
				}
				if(!ptr_indexer->exhausted() && !ptr_indexer->skip()){
					KrazyLaneNode* ptr_tobe_processed_conn_lane = ptr_in_conn_lanes->at(ptr_indexer->getCursor()++);
					if(!_prevToBeProcessedLanes[ptr_tobe_processed_conn_lane]){
						_prevToBeProcessedLanes[ptr_tobe_processed_conn_lane] = true;
					    ptr_prev = ptr_processing_lane;
						ptr_processing_lane = ptr_tobe_processed_conn_lane;
						ptr_indexer = 0;
						continue;
					}
				}
				else{
					_prevToBeProcessedLanes[ptr_processing_lane] = false;
					ptr_processing_lane = ptr_indexer->getPrev();
					ptr_indexer->reset();
					ptr_indexer = 0;
					continue;
				}
			}
		}
	}
#endif
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _UN_OPT_PATH_SEARCH
bool   KrazyDefaultAlgoMovingVehical_01Phase::getRouteWithDistance(
											  KrazyLaneNode* ptr_source_lane ,  KrazyLaneNode* ptr_destination_lane , 
											  KrazyLaneNode* ptr_prev , KrazyTrafficIndicatorList** ptr_route_map , 
											  float& distance ,  hash_map<KrazyLaneNode* , routing_index> * ptr_indexer /*= 0*/)
{
	float& curr_distance = _CurrentDistance;
	KrazyTrafficIndicatorList& aux_route = *_PTRAuxshortroutemap;
	KrazyLaneNode* ptr_source_doublelane_routing = ptr_prev;
	for(;ptr_source_doublelane_routing;){
		getConnectedTrafficIndicatorsandDistance(ptr_source_doublelane_routing , curr_distance , &aux_route);
		int count_route_map = aux_route.size();
		if(!(count_route_map > 0)){
			return false;
		}
		KrazyLaneNode* ptr_single_lane_match = aux_route.at(count_route_map - 1)->PTRParentLaneNode();
		if(ptr_source_lane->getParent() == ptr_single_lane_match->getParent()){
			if(ptr_source_lane != ptr_single_lane_match){
				aux_route.push_back(ptr_source_lane->getFlipTrafficIndicator());
				curr_distance += aux_route.at(count_route_map)->Vector()->distance(*ptr_source_lane->getEntryIndicator()->Vector());
			}
			if(curr_distance > 0 && curr_distance < distance){
				distance = curr_distance;
			/*	_MinDistance = distance;*/
				(*ptr_route_map)->clear();
				for(int j = 0; j < (int)aux_route.size();j++){
					(*ptr_route_map)->push_back(aux_route.at(j));
				}
			}
			curr_distance = 0.0f;
			aux_route.clear();
			KRAZY_ASSIGN_DESTINATION();
			aux_route.push_back(ptr_dest_end);
		}
		if(ptr_indexer){
			ptr_source_doublelane_routing = ptr_indexer->at(ptr_source_doublelane_routing).getPrev();
			continue;
		}
		ptr_source_doublelane_routing = 0;
	}
	return (curr_distance > 0);
}
#else
void   KrazyDefaultAlgoMovingVehical_01Phase::getRouteWithDistance(
											  KrazyLaneNode* ptr_source_lane ,  KrazyLaneNode* ptr_destination_lane ,  KrazyLaneNode* ptr_curr,
											  KrazyLaneNode* ptr_prev , KrazyTrafficIndicatorList* ptr_route_map , unsigned int case_include_origin /*= 0*/
											  )
{
	;
	KrazyLaneNode* ptr_source_doublelane_routing = ptr_prev;
	for(;ptr_source_doublelane_routing;){
		///// Start.........................................................
		int count_r_list = _PTRAuxshortroutemap->size();
		if(count_r_list > 0){
			KrazyTrafficIndicator* ptr_last_turnIndc = _PTRAuxshortroutemap->at(count_r_list - 1);
			KrazyLaneNode* ptr_last_lane = ptr_last_turnIndc->PTRParentLaneNode();
			bool proceeding_with_opposite_lane(false);
			bool finish_route_detection(false);
			for(int it = 0; it < 2 /*&& !finish_route_detection*/; it++){
				KrazyTrafficIndicatorList* ptr_last_lane_dest_indicators = ptr_last_lane->getDestinationIndicators();
				int dest_indicator_count = ptr_last_lane_dest_indicators->size();
				/* find connected source to these destination indicators if connected source belongs to the current lane then 
				found.,,. or else , take the next opposite single lane in the same double lane and find the correct destination along and match with the source single lane..,
				*/
				for(int i = 0; i < dest_indicator_count/* && !finish_route_detection*/; i++){
					KrazyTrafficIndicator* ptr_dest = ptr_last_lane_dest_indicators->at(i);
					KrazyTrafficIndicator* ptr_source  = ptr_dest->get_Source();
					KrazyLaneNode* ptr_source_lane = ptr_source->PTRParentLaneNode();
					for(int j = 0; j < 2 && !finish_route_detection; j++){
						if(ptr_source_lane == ptr_source_doublelane_routing->getLane(j)){
							if(!proceeding_with_opposite_lane){
								if(shouldAddExtraFlips(ptr_last_lane,ptr_last_turnIndc,ptr_dest)){
									KrazyLaneNode* ptr_opposite_lane = ptr_last_lane->getOppositeSingleLane();
									KrazyTrafficIndicator* ptr_opposite_lane_flip_indc = ptr_opposite_lane->getFlipTrafficIndicator();
									_PTRAuxshortroutemap->push_back(ptr_opposite_lane_flip_indc);count_r_list++;
									_PTRAuxshortroutemap->push_back(ptr_last_lane->getFlipTrafficIndicator());count_r_list++;
									_CurrentDistance += glm::distance(*ptr_last_turnIndc->Vector() , *ptr_last_lane->getEntryIndicator()->Vector());//ptr_last_turnIndc->Vector()->distance(*ptr_last_lane->getEntryIndicator()->Vector());
									_CurrentDistance += glm::distance(*ptr_opposite_lane_flip_indc->Vector() , *ptr_opposite_lane->getEntryIndicator()->Vector());//ptr_opposite_lane_flip_indc->Vector()->distance(*ptr_opposite_lane->getEntryIndicator()->Vector());
								}
							}
							else{
								_CurrentDistance += glm::distance(*_PTRAuxshortroutemap->at(count_r_list - 1)->Vector() , *ptr_last_lane->getOppositeSingleLane()->getEntryIndicator()->Vector());//_PTRAuxshortroutemap->at(count_r_list - 1)->Vector()->distance(*ptr_last_lane->getOppositeSingleLane()->getEntryIndicator()->Vector());
								_PTRAuxshortroutemap->push_back(ptr_last_lane->getFlipTrafficIndicator());count_r_list++;
							}
							_CurrentDistance += glm::distance(*_PTRAuxshortroutemap->at(count_r_list - 1)->Vector() , *ptr_dest->Vector());//_PTRAuxshortroutemap->at(count_r_list - 1)->Vector()->distance(*ptr_dest->Vector());
							_PTRAuxshortroutemap->push_back(ptr_source);
							finish_route_detection = true;
							break;
						}
					}
					if(finish_route_detection)
						break;
				}
				if(finish_route_detection)
						break;
				ptr_last_lane = ptr_last_lane->getOppositeSingleLane();
				proceeding_with_opposite_lane = true;// make all these bool to unsigned int and convert if with the switch case;
			}
		}
		////  End............................................................
		int count_route_map = _PTRAuxshortroutemap->size();
		if(!(count_route_map > 0)){
			return; // should never happen in proper city lay out ..,
		}
		KrazyLaneNode* ptr_single_lane_match = _PTRAuxshortroutemap->at(count_route_map - 1)->PTRParentLaneNode();
		if(ptr_source_lane->getParent() == ptr_single_lane_match->getParent()){
			if(ptr_source_lane != ptr_single_lane_match){
				_PTRAuxshortroutemap->push_back(ptr_source_lane->getFlipTrafficIndicator());
				_CurrentDistance += glm::distance(*_PTRAuxshortroutemap->at(count_route_map)->Vector() , *ptr_source_lane->getEntryIndicator()->Vector());//_PTRAuxshortroutemap->at(count_route_map)->Vector()->distance(*ptr_source_lane->getEntryIndicator()->Vector());
			}
			//
			/* this may be a patch or may be approproiately said "Tweaking" .., need this to move around srd vehicals 
			   this is to check wheather the vehical is standing on the or after the traffic indicator.., or passed the the traffic indicator which is included
			   in the route map .., the path is linked by adding 2 flip indicators..,
			*/
			KrazyTrafficIndicator* _indicator = _PTRAuxshortroutemap->at(_PTRAuxshortroutemap->size() - 1);
			if(_indicator->PTRParentLaneNode() == ptr_source_lane){
				glm::vec2* _vec_indicator = _indicator->Vector();
				KrazyBool _add_flips = KRAZY_BOOL_False;
				switch(ptr_source_lane->getProperties()->get_LaneDirection())
				{
				case KRAZY_LANE_DIRECTION_UP_HORZS   :_add_flips = (_vec_indicator->x > _VecSearchOrigin.x) ? KRAZY_BOOL_True : KRAZY_BOOL_False;break;
				case KRAZY_LANE_DIRECTION_DOWN_HORZS :_add_flips = (_vec_indicator->x < _VecSearchOrigin.x) ? KRAZY_BOOL_True : KRAZY_BOOL_False;break;
				case KRAZY_LANE_DIRECTION_UP_VERTS   :_add_flips = (_vec_indicator->y < _VecSearchOrigin.y) ? KRAZY_BOOL_True : KRAZY_BOOL_False;break;
				case KRAZY_LANE_DIRECTION_DOWN_VERTS :_add_flips = (_vec_indicator->y > _VecSearchOrigin.y) ? KRAZY_BOOL_True : KRAZY_BOOL_False;break;
				}
				/////////////.....................
				switch(_add_flips)
				{
				case KRAZY_BOOL_True:
#ifdef _KRAZY_ALGO_TRACE_ENBALED
					printf("\n Incomplete Path Detected ---- !");
#endif
					KrazyLaneNode* _opposite_lane = ptr_source_lane->getOppositeSingleLane();
					KrazyTrafficIndicator* _auxy_flip = _opposite_lane->getFlipTrafficIndicator();
					_PTRAuxshortroutemap->push_back(_auxy_flip);count_route_map++;
					_CurrentDistance += glm::distance(*_opposite_lane->getEntryIndicator()->Vector() , *_auxy_flip->Vector());
					_auxy_flip = ptr_source_lane->getFlipTrafficIndicator();
					_PTRAuxshortroutemap->push_back(_auxy_flip);count_route_map++;
					_CurrentDistance += glm::distance(_VecSearchOrigin , *_auxy_flip->Vector());
					_CurrentDistance += glm::distance(*ptr_source_lane->getEntryIndicator()->Vector() , *_indicator->Vector());
#ifdef _KRAZY_ALGO_TRACE_ENBALED
					printf("\n Incomplete Path Fixed ---- !");
#endif
					break;
				}

			}
			//
#ifdef _KRAZY_ALGO_TRACE_ENBALED
				printf("\n  ---- _CurrentDistance = %f !" , _CurrentDistance);
#endif
			//
			if(_CurrentDistance > 0 && _CurrentDistance < _RealDistance){
#ifdef _KRAZY_ALGO_TRACE_ENBALED
				printf("\n  ---- Picked _CurrentDistance = %f !" , _CurrentDistance);
#endif
				_RealDistance = _CurrentDistance;
				ptr_route_map->clear();
				int count_aux_map = (int)_PTRAuxshortroutemap->size();
				for(int j = 0; j < count_aux_map; j++){
					ptr_route_map->push_back(_PTRAuxshortroutemap->at(j));
				}
			}
			_CurrentDistance = 0.0f;
			KRAZY_ASSIGN_DESTINATION();
		}
		ptr_source_doublelane_routing = ptr_source_doublelane_routing->getRoutingIndexer()->getPrev();
		if(!ptr_source_doublelane_routing){
			switch(case_include_origin){
			case 0:case_include_origin++;ptr_source_doublelane_routing = ptr_curr;continue;
			case 1:case_include_origin++;ptr_source_doublelane_routing = ptr_source_lane->getParent();continue;
			}
		}
	}
}
#endif
///
bool KrazyDefaultAlgoMovingVehical_01Phase::shouldAddExtraFlips(KrazyLaneNode* lane , KrazyTrafficIndicator* turn  , KrazyTrafficIndicator* dest)
{
	// these direction came to my rescue yet another time.., it may be crude , I feel its more readable and fast -
	Vector2 vec_turn (*turn->Vector());
	Vector2 vec_dest (*dest->Vector());
	switch(lane->getProperties()->get_LaneDirection())
	{
		// from greater coordinates to lesser coordinates..,
	case KRAZY_LANE_DIRECTION_UP_HORZS   : 
	case KRAZY_LANE_DIRECTION_DOWN_VERTS : 
		return(vec_dest < vec_turn);
		// from lesser coordinates to greater coordinates..,
	case KRAZY_LANE_DIRECTION_DOWN_HORZS : 
	case KRAZY_LANE_DIRECTION_UP_VERTS   : 
		return(vec_turn < vec_dest);
		
	}
	return false;
}
//
void   KrazyDefaultAlgoMovingVehical_01Phase::getConnectedTrafficIndicatorsandDistance(KrazyLaneNode* source , float& distance , KrazyTrafficIndicatorList* r_list)
{
	int count_r_list = r_list->size();
	if(count_r_list > 0){
		KrazyTrafficIndicator* ptr_last_turnIndc = r_list->at(count_r_list - 1);
		KrazyLaneNode* ptr_last_lane = ptr_last_turnIndc->PTRParentLaneNode();
		bool proceeding_with_opposite_lane(false);
		for(int it = 0; it < 2; it++){
			KrazyTrafficIndicatorList* ptr_last_lane_dest_indicators = ptr_last_lane->getDestinationIndicators();
			int dest_indicator_count = ptr_last_lane_dest_indicators->size();
			/* find connected source to these destination indicators if connected source belongs to the current lane then 
			found.,,. or else , take the next opposite single lane in the same double lane and find the correct destination along and match with the source single lane..,
			*/
			for(int i = 0; i < dest_indicator_count; i++){
				KrazyTrafficIndicator* ptr_dest = ptr_last_lane_dest_indicators->at(i);
				KrazyTrafficIndicator* ptr_source  = ptr_dest->get_Source();
				KrazyLaneNode* ptr_source_lane = ptr_source->PTRParentLaneNode();
				for(int j = 0; j < 2; j++){
					if(ptr_source_lane == source->getLane(j)){
						if(!proceeding_with_opposite_lane){
							if(shouldAddExtraFlips(ptr_last_lane,ptr_last_turnIndc,ptr_dest)){
								KrazyLaneNode* ptr_opposite_lane = ptr_last_lane->getOppositeSingleLane();
								KrazyTrafficIndicator* ptr_opposite_lane_flip_indc = ptr_opposite_lane->getFlipTrafficIndicator();
								r_list->push_back(ptr_opposite_lane_flip_indc);count_r_list++;
								r_list->push_back(ptr_last_lane->getFlipTrafficIndicator());count_r_list++;
								distance += glm::distance(*ptr_last_turnIndc->Vector() , *ptr_last_lane->getEntryIndicator()->Vector());//ptr_last_turnIndc->Vector()->distance(*ptr_last_lane->getEntryIndicator()->Vector());
								distance += glm::distance(*ptr_opposite_lane_flip_indc->Vector() , *ptr_opposite_lane->getEntryIndicator()->Vector());//ptr_opposite_lane_flip_indc->Vector()->distance(*ptr_opposite_lane->getEntryIndicator()->Vector());
							}
						}
						else{
							distance += glm::distance(*r_list->at(count_r_list - 1)->Vector() , *ptr_last_lane->getOppositeSingleLane()->getEntryIndicator()->Vector());//r_list->at(count_r_list - 1)->Vector()->distance(*ptr_last_lane->getOppositeSingleLane()->getEntryIndicator()->Vector());
							r_list->push_back(ptr_last_lane->getFlipTrafficIndicator());count_r_list++;
						}
						distance += glm::distance(*r_list->at(count_r_list - 1)->Vector() , *ptr_dest->Vector());//r_list->at(count_r_list - 1)->Vector()->distance(*ptr_dest->Vector());
						r_list->push_back(ptr_source);
						return;
					}
				}
			}
			ptr_last_lane = ptr_last_lane->getOppositeSingleLane();
			proceeding_with_opposite_lane = true;
		}
	}
}
//
void KrazyDefaultAlgoMovingVehical_01Phase::on_vehicalTurn(KrazyVehical* ptr_vehical , KrazyTrafficIndicator* ptr_traffic_indc_matched)
{
	glm::vec2  dest_vector;
	KrazyLaneNode* ptr_dest_lane = 0;
	KrazyVehicalMoveCommand vehical_move_command = KRAZY_VEHICALMOVECMD_Default;
	KrazyVehicalFlipMode _flip_mode = KRAZY_VEHICAL_FLIP_MODE_None;
	switch(ptr_traffic_indc_matched->Type()){
	case KRAZY_TRAFFIC_INDC_Entry:
		{
			dest_vector = *ptr_traffic_indc_matched->Vector();
			ptr_dest_lane   = ptr_traffic_indc_matched->PTRParentLaneNode();
			vehical_move_command = KRAZY_VEHICALMOVECMD_MoveandRotate;
		}
		break;
	case KRAZY_TRAFFIC_INDC_Turn  : 
		{
			KrazyTrafficIndicator* ptr_destination_indc = ptr_traffic_indc_matched->get_DestinationIndc(0);
			dest_vector     = *ptr_destination_indc->Vector();
			ptr_dest_lane   = ptr_destination_indc->PTRParentLaneNode();
			vehical_move_command = KRAZY_VEHICALMOVECMD_MoveandRotate;
		}
		break;
	case KRAZY_TRAFFIC_INDC_FlipPoint:
		{
			KrazyLaneNode* ptr_lane = ptr_traffic_indc_matched->PTRParentLaneNode();
			KrazyLaneDirection lane_direction = ptr_lane->getProperties()->get_LaneDirection();
			KrazyLaneNode* ptr_other_single_road = ptr_lane->getOppositeSingleLane();
			dest_vector = *ptr_other_single_road->getEntryIndicator()->Vector();//ptr_traffic_indc_matched->Vector();
			ptr_dest_lane = ptr_other_single_road;
			_flip_mode = KRAZY_VEHICAL_FLIP_MODE_Normal;
			vehical_move_command = KRAZY_VEHICALMOVECMD_Flip;
		}
		break;
	case KRAZY_TRAFFIC_INDC_OnSpotFlip:
		{
			dest_vector = *ptr_traffic_indc_matched->Vector();
			ptr_dest_lane   = ptr_traffic_indc_matched->PTRParentLaneNode();
			_flip_mode = KRAZY_VEHICAL_FLIP_MODE_Spot;
			vehical_move_command = KRAZY_VEHICALMOVECMD_Flip;
		}
		break;
	case KRAZY_TRAFFIC_INDC_SliderPoint:
		{
			dest_vector = *ptr_traffic_indc_matched->get_bay()->Vector();
			ptr_vehical->reached_slider_point() = KRAZY_BOOL_True;
		}
		break;
	default:return;
	}
	*ptr_vehical->Vector() = dest_vector;
	KrazyLaneNode* ptr_assigned_lane = ptr_vehical->PTRCurrentAssignedSingleLane();
	if(ptr_assigned_lane){
		ptr_assigned_lane->on_detachVehical(ptr_vehical);
	}
	//
	ptr_vehical->on_attachCurrentLane(ptr_dest_lane);
	if(ptr_dest_lane)
		ptr_dest_lane->on_attachVehical(ptr_vehical);
	//
	switch(ptr_vehical->Type())
	{
	case KRAZY_VEHICAL_TYPE_NORMAL: 
		switch(ptr_vehical->route_mode())
		{
		case KRAZY_ROUTEMODE_ByVehical: 
		case KRAZY_ROUTEMODE_Normal:
			 ptr_vehical->get_TrafficIndicatorMap()->clear(); 	
			 ptr_vehical->on_setNextTurnIndex(0);
			 break;
		}
		break;
	}
	switch(vehical_move_command)
	{
	case KRAZY_VEHICALMOVECMD_MoveandRotate:ptr_vehical->on_moveandrotate();break;
	case KRAZY_VEHICALMOVECMD_Flip:ptr_vehical->on_flip();ptr_vehical->flip_mode() = _flip_mode;break;
	}
}
//
void KrazyDefaultAlgoMovingVehical_01Phase::flip_on_spot(KrazyVehical* ptr_vehical , KrazyLaneNode* _next_sibiling_lane)
{
	KrazyTrafficIndicator* _on_spot_indicator = ptr_vehical->get_onSpotFlipIndicator();
	glm::vec2 vec;
	switch(_next_sibiling_lane->getProperties()->get_LaneDirection())
	{
		case KRAZY_LANE_DIRECTION_UP_HORZS    :
		case KRAZY_LANE_DIRECTION_DOWN_HORZS  :
			 vec.x = ptr_vehical->Vector()->x;
			 vec.y = _next_sibiling_lane->getEntryIndicator()->Vector()->y;
			 break;
		case KRAZY_LANE_DIRECTION_UP_VERTS	  :   
		case KRAZY_LANE_DIRECTION_DOWN_VERTS  : 
			 vec.x = _next_sibiling_lane->getEntryIndicator()->Vector()->x;
			 vec.y = ptr_vehical->Vector()->y;
			 break;
	}
	_on_spot_indicator->on_reset(vec , _next_sibiling_lane);
	ptr_vehical->on_turning(_on_spot_indicator);
}
//
KrazyVehicalStateInSiblingLanes KrazyDefaultAlgoMovingVehical_01Phase::_detect_target_vehical_in_oppositelane(KrazyVehical* _monitoring_vehical , 
                                       KrazyVehical* _target_vehical , KrazyLaneNode* _opposite_lane)
{
	KrazyVehicalList* _vehical_list = _opposite_lane->getVehicalList();
	KrazyLaneDirection _lane_direction = _opposite_lane->getProperties()->get_LaneDirection();
	int vehicals = _vehical_list->size();
	for(int vehical = 0; vehical < vehicals; vehical++)
	{
		KrazyVehical* _vehical = _vehical_list->at(vehical);
		if(_vehical == _target_vehical)
		{
			glm::vec2* _vec_monotoring_vehical_front = _monitoring_vehical->VectorFrontBumper();
			glm::vec2* _vec_target_vehical_front = _target_vehical->VectorFrontBumper();
			glm::vec2* _vec_monotoring_vehical_rear = _monitoring_vehical->VectorRearBumper();
			glm::vec2* _vec_target_vehical_rear = _target_vehical->VectorRearBumper();
			glm::vec2* _flip_indicator = _opposite_lane->getFlipTrafficIndicator()->Vector();
			KrazyBool  isPassed = KRAZY_BOOL_False;
			switch(_lane_direction)
			{
			case KRAZY_LANE_DIRECTION_UP_HORZS    :
				if(_vec_target_vehical_front->x > _vec_monotoring_vehical_front->x){
					return KRAZY_VEHICAL_STATE_IN_SIBLINLANES_approaching_from_opposite;
				}
				else if(
					//..,
					(
					_vec_target_vehical_rear->x     == _vec_monotoring_vehical_front->x &&
					_vec_monotoring_vehical_rear->x == _vec_target_vehical_front->x
					)
					||
					( 
					_vec_target_vehical_rear->x >= _vec_monotoring_vehical_rear->x &&
					_vec_monotoring_vehical_front->x >= _vec_target_vehical_front->x)
					)
					//..,
				{
					//..,
					return KRAZY_VEHICAL_STATE_IN_SIBLINLANES_passing_from_opposite;
				}
				//..,
				else if(_vec_monotoring_vehical_rear->x > _vec_target_vehical_rear->x)
				{
					isPassed = KRAZY_BOOL_True;
				}
				break;
				//..,,
			case KRAZY_LANE_DIRECTION_DOWN_HORZS  : 
				if(_vec_target_vehical_front->x < _vec_monotoring_vehical_front->x){
					return KRAZY_VEHICAL_STATE_IN_SIBLINLANES_approaching_from_opposite;
				}
				else if(
					//..,
					(
					_vec_target_vehical_rear->x     == _vec_monotoring_vehical_front->x &&
					_vec_monotoring_vehical_rear->x == _vec_target_vehical_front->x
					)
					||
					(
					_vec_target_vehical_rear->x <= _vec_monotoring_vehical_rear->x &&
					_vec_monotoring_vehical_front->x <= _vec_target_vehical_front->x
					)
					){
						return KRAZY_VEHICAL_STATE_IN_SIBLINLANES_passing_from_opposite;
				}
				else if(_vec_monotoring_vehical_rear->x < _vec_target_vehical_rear->x){
					isPassed = KRAZY_BOOL_True;
				}
				break;
				//..,
			case KRAZY_LANE_DIRECTION_UP_VERTS	  : 
				if(_vec_target_vehical_front->y < _vec_monotoring_vehical_front->y){
					return KRAZY_VEHICAL_STATE_IN_SIBLINLANES_approaching_from_opposite;
				}
				else if(
					//..,
					(
					_vec_target_vehical_rear->x     == _vec_monotoring_vehical_front->x &&
					_vec_monotoring_vehical_rear->x == _vec_target_vehical_front->x
					)
					||
					(
					_vec_target_vehical_rear->y <= _vec_monotoring_vehical_rear->y &&
					_vec_monotoring_vehical_front->y <= _vec_target_vehical_front->y

					)
					){
						return KRAZY_VEHICAL_STATE_IN_SIBLINLANES_passing_from_opposite;
				}
				else if(_vec_monotoring_vehical_rear->y < _vec_target_vehical_rear->y){
					isPassed = KRAZY_BOOL_True;
				}
				break;
				//..,
			case KRAZY_LANE_DIRECTION_DOWN_VERTS  : 
				if(_vec_target_vehical_front->y > _vec_monotoring_vehical_front->y){
					return KRAZY_VEHICAL_STATE_IN_SIBLINLANES_approaching_from_opposite;
				} 
				else if(
					//..,
					(
					_vec_target_vehical_rear->x     == _vec_monotoring_vehical_front->x &&
					_vec_monotoring_vehical_rear->x == _vec_target_vehical_front->x
					)
					||

					( _vec_target_vehical_rear->y >= _vec_monotoring_vehical_rear->y &&
					_vec_monotoring_vehical_front->y >= _vec_target_vehical_front->y)
					){
						return KRAZY_VEHICAL_STATE_IN_SIBLINLANES_passing_from_opposite;
				}
				else if(_vec_monotoring_vehical_rear->y > _vec_target_vehical_rear->y){
					isPassed = KRAZY_BOOL_True;
				}
				break;
				//..,
			}
			//..,
			switch(isPassed)
			{
			case KRAZY_BOOL_True :
				{
					float distance = glm::distance(*_vec_monotoring_vehical_rear , *_vec_target_vehical_rear);
					if(distance <= _monitoring_vehical->gap()){
						return KRAZY_VEHICAL_STATE_IN_SIBLINLANES_passing_from_opposite;
					}
					else{
						return KRAZY_VEHICAL_STATE_IN_SIBLINLANES_passed_from_opposite;
					}
				}
				break;
			}
			break;
		}
	}
	return KRAZY_VEHICAL_STATE_IN_SIBLINLANES_Default;
}
//
void KrazyDefaultAlgoMovingVehical_01Phase::_fix_incomplete_route_if_any(KrazyTrafficIndicator* _origin_indicator , KrazyTrafficIndicatorList*   _route)
{
	KrazyTrafficIndicator* _indicator = _route->at(_route->size() - 1);
	KrazyLaneNode* _lane = _origin_indicator->PTRParentLaneNode();
	if(_indicator->PTRParentLaneNode() == _lane){
		glm::vec2* _vec_origin_indicator = _origin_indicator->Vector();
		glm::vec2* _vec_indicator = _indicator->Vector();
		KrazyBool _add_flips = KRAZY_BOOL_False;
		switch(_lane->getProperties()->get_LaneDirection())
		{
		case KRAZY_LANE_DIRECTION_UP_HORZS   :_add_flips = (_vec_indicator->x > _vec_origin_indicator->x) ? KRAZY_BOOL_True : KRAZY_BOOL_False;break;
		case KRAZY_LANE_DIRECTION_DOWN_HORZS :_add_flips = (_vec_indicator->x < _vec_origin_indicator->x) ? KRAZY_BOOL_True : KRAZY_BOOL_False;break;
		case KRAZY_LANE_DIRECTION_UP_VERTS   :_add_flips = (_vec_indicator->y < _vec_origin_indicator->y) ? KRAZY_BOOL_True : KRAZY_BOOL_False;break;
		case KRAZY_LANE_DIRECTION_DOWN_VERTS :_add_flips = (_vec_indicator->y > _vec_origin_indicator->y) ? KRAZY_BOOL_True : KRAZY_BOOL_False;break;
		}
		//
		switch(_add_flips)
		{
		case KRAZY_BOOL_True:
#ifdef _KRAZY_ALGO_TRACE_ENBALED
            printf("\n Incomplete Path Detected ---- !");
#endif
			_route->push_back(_lane->getOppositeSingleLane()->getFlipTrafficIndicator());
			_route->push_back(_lane->getFlipTrafficIndicator());

#ifdef _KRAZY_ALGO_TRACE_ENBALED
            printf("\n Incomplete Path Fixed ---- !");
#endif
			break;
		}
	}
}
//
KrazyBool KrazyDefaultAlgoMovingVehical_01Phase::_detect_if_short_route_detection_is_required(KrazyLaneNode* ptr_source_lane , KrazyLaneNode* ptr_destination_lane  , 
	                                              KrazyTrafficIndicatorList* ptr_route_map )
{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	KrazyLaneNode* _source_parent_lane = ptr_source_lane->getParent();
	KrazyLaneNodeList* _node_list = _source_parent_lane->getConnectedLanes();
	int nodes = _node_list->size();
	KrazyBool _finish_routing = KRAZY_BOOL_False;
	for(int node = 0; node < nodes; node++){
		KrazyLaneNode* _node = _node_list->at(node);
		switch(_node->foundSingleLane(ptr_destination_lane))
		{
		case KRAZY_BOOL_True:
			{
				KRAZY_ASSIGN_DESTINATION();
				KrazyTrafficIndicator* _last_indc = _PTRAuxshortroutemap->at(_PTRAuxshortroutemap->size() - 1);
				KrazyLaneNode* _lane = _last_indc->PTRParentLaneNode();
				for(int source = 0; source < 2; source++){
					KrazyTrafficIndicatorList* _dest_indicators = _lane->getDestinationIndicators();
					KrazyLaneNode* _opposite_lane = ptr_source_lane->getOppositeSingleLane();
					int indicators = _dest_indicators->size();	
					for(int indicator = 0; indicator < indicators; indicator++){
						KrazyTrafficIndicator* _dest_indicator = _dest_indicators->at(indicator);
						KrazyTrafficIndicator* _source_indicator = _dest_indicator->get_Source();
						KrazyLaneNode* _source_indicator_lane = _source_indicator->PTRParentLaneNode();
						KrazyLaneNode* _opposite_lane = _source_indicator_lane->getOppositeSingleLane();
						if(ptr_source_lane == _source_indicator_lane){
							_PTRAuxshortroutemap->push_back(_source_indicator);
							_finish_routing = KRAZY_BOOL_True;
							goto FILL_KRAZY_ROUTE;
						}
						else if(ptr_source_lane == _opposite_lane){
							_PTRAuxshortroutemap->push_back(_source_indicator);
							_PTRAuxshortroutemap->push_back(_opposite_lane->getFlipTrafficIndicator());
							_finish_routing = KRAZY_BOOL_True;
							goto FILL_KRAZY_ROUTE;
						}
					}
					//
					_lane = _lane->getOppositeSingleLane();
					_PTRAuxshortroutemap->push_back(_lane->getFlipTrafficIndicator());
				}
							 
			}
			return KRAZY_BOOL_False;
		}
	}
	//
FILL_KRAZY_ROUTE:
	switch(_finish_routing)
	{
	case KRAZY_BOOL_True:
		{
			int count = _PTRAuxshortroutemap->size();
			for(int i = 0; i < count; i++){
				ptr_route_map->push_back(_PTRAuxshortroutemap->at(i));
			}
			_PTRAuxshortroutemap->clear();
			return KRAZY_BOOL_False; 
		}
		break;
	}
	//
	return KRAZY_BOOL_True;
}
//
void KrazyDefaultAlgoMovingVehical_01Phase::_fix_incomplete_route_if_any_for_vehical(KrazyVehical* _vehical , KrazyTrafficIndicatorList*   _route)
{
	KrazyTrafficIndicator* _indicator = _route->at(_route->size() - 1);
	KrazyLaneNode* _lane = _vehical->PTRCurrentAssignedSingleLane();
	if(_indicator->PTRParentLaneNode() == _lane){
#if 0
		glm::vec2* _vec_vehical = _vehical->Vector();
#else
		glm::vec2* _vec_vehical = _vehical->VectorFrontBumper();
#endif
		glm::vec2* _vec_indicator = _indicator->Vector();
		KrazyBool _add_flips = KRAZY_BOOL_False;
		switch(_lane->getProperties()->get_LaneDirection())
		{
		case KRAZY_LANE_DIRECTION_UP_HORZS   :_add_flips = (_vec_indicator->x > _vec_vehical->x) ? KRAZY_BOOL_True : KRAZY_BOOL_False;break;
		case KRAZY_LANE_DIRECTION_DOWN_HORZS :_add_flips = (_vec_indicator->x < _vec_vehical->x) ? KRAZY_BOOL_True : KRAZY_BOOL_False;break;
		case KRAZY_LANE_DIRECTION_UP_VERTS   :_add_flips = (_vec_indicator->y < _vec_vehical->y) ? KRAZY_BOOL_True : KRAZY_BOOL_False;break;
		case KRAZY_LANE_DIRECTION_DOWN_VERTS :_add_flips = (_vec_indicator->y > _vec_vehical->y) ? KRAZY_BOOL_True : KRAZY_BOOL_False;break;
		}
		//
		switch(_add_flips)
		{
		case KRAZY_BOOL_True:
#ifdef _KRAZY_ALGO_TRACE_ENBALED
            printf("\n Incomplete Path Detected For Vehical ---- !");
#endif
			_route->push_back(_lane->getOppositeSingleLane()->getFlipTrafficIndicator());
			_route->push_back(_lane->getFlipTrafficIndicator());

#ifdef _KRAZY_ALGO_TRACE_ENBALED
            printf("\n Incomplete Path Fixed  For Vehical---- !");
#endif
			break;
		}
	}
}
//
int KrazyDefaultAlgoMovingVehical_01Phase::getLaneIndex(KrazyLaneNode* _lane)
{

	KrazyNetworkCity_01Phase* ptr_parent_network = KrazyNetworkCity_01Phase::getNetwork();
	vector<KrazyWheelNetwork_01Phase*>* ptr_network_nodes = ptr_parent_network->getNodeCache();
	
	int lanes = ptr_network_nodes->size();
	for(int lane = 0 ; lane < lanes; lane++)
	{
		KrazyLaneNode* _parent_lane = ((KrazyNetworkCity_01Phase*)ptr_network_nodes->at(lane))->getOwner();
		string direction_name;
		switch(_parent_lane->foundSingleLane(_lane))
		{			
		case KRAZY_BOOL_True:
			{
				switch(_lane->getProperties()->get_LaneDirection())
				{
					case KRAZY_LANE_DIRECTION_UP_HORZS   : direction_name = "KRAZY_LANE_DIRECTION_UP_HORZS"   ;break;
					case KRAZY_LANE_DIRECTION_DOWN_HORZS : direction_name = "KRAZY_LANE_DIRECTION_DOWN_HORZS" ;break;
					case KRAZY_LANE_DIRECTION_UP_VERTS   : direction_name = "KRAZY_LANE_DIRECTION_UP_VERTS"   ;break;
					case KRAZY_LANE_DIRECTION_DOWN_VERTS : direction_name = "KRAZY_LANE_DIRECTION_DOWN_VERTS" ;break;
				}
			}
#ifdef _KRAZY_ALGO_TRACE_ENBALED
			printf("\n Lane Index [%d] in direction %s" , lane , direction_name.c_str());
#endif
			return lane;
		}
	}

	return -1;
}
//
void KrazyDefaultAlgoMovingVehical_01Phase::get_route_for_vehical(KrazyVehical* _vehical , KrazyLaneNode* _source_lane , KrazyLaneNode* _destination_lane , KrazyTrafficIndicatorList* _car_route , KrazyRouteSearchType _searchType /*=  KRAZY_ROUTE_SEARCH_for_SHORTEST*/)
{
	_car_route->clear();
	switch(_searchType)
	{
	case KRAZY_ROUTE_SEARCH_for_SHORTEST:
		switch(_detect_if_short_route_detection_is_required(_source_lane , _destination_lane , _car_route))
		{
		case KRAZY_BOOL_True:
			_VecSearchOrigin = *_vehical->VectorFrontBumper();
			KrazyDefaultAlgoMovingVehical_01Phase::get_ShortestRouteMap(_source_lane , _destination_lane , _car_route);
			break;
		case KRAZY_BOOL_False:
#ifdef _KRAZY_ALGO_TRACE_ENBALED
			printf("\n srd not required for %s" , _vehical->to_string().c_str());
#endif
			_fix_incomplete_route_if_any_for_vehical(_vehical , _car_route);
			break;
		}
		break;
	case KRAZY_ROUTE_SEARCH_for_LONGEST:break;
	}
}
//
//#define _LOG_FINISHING_TI_srd_SHERRIF
KrazyBool KrazyDefaultAlgoMovingVehical_01Phase::getFinishingTrafficIndicator(KrazyVehical* ptr_vehical , KrazyTrafficIndicator** ptr_last_indc_met)
{
	KrazyTrafficIndicatorList* ptr_car_route = ptr_vehical->get_TrafficIndicatorMap();
	int count = ptr_car_route->size();
	if(!(count > 0))
		return KRAZY_BOOL_False;
	//
	for(int traffic_indc = ptr_vehical->getNextTurnIndex(); traffic_indc > -1; traffic_indc--){
		KrazyTrafficIndicator* ptr_traffic_indicator = ptr_car_route->at(traffic_indc);
		glm::vec2* ptr_vector_traffic_indc = ptr_traffic_indicator->Vector();
		glm::vec2* ptr_vector_vehical_front_bumper_vector = ptr_vehical->VectorFrontBumper();
		float distance = glm::distance(*ptr_vector_traffic_indc , *ptr_vector_vehical_front_bumper_vector);
		if(distance < _KRAZY_TRAFFIC_INDC_SPACE_MIN){
			*ptr_last_indc_met = ptr_traffic_indicator;
			ptr_vehical->on_setNextTurnIndex((traffic_indc > 0) ? traffic_indc - 1 : 0);
			return KRAZY_BOOL_True;
		}
		return KRAZY_BOOL_False;
	}
	return KRAZY_BOOL_False;
}
//
KrazyBool KrazyDefaultAlgoMovingVehical_01Phase::_check_opposite_lane_vehical(KrazyVehical* _vehical , KrazyVehical* _vehical_opposit , 
	                      KrazyVehicalStateInSiblingLanes& _vehical_state)
{
	
	KrazyLaneNode* _current_assigned_lane = _vehical->PTRCurrentAssignedSingleLane();
	KrazyLaneNode* _destination_lane = _vehical_opposit->PTRCurrentAssignedSingleLane();
			
	if(!_current_assigned_lane) 
		return KRAZY_BOOL_False;

	if(!_destination_lane)
		return KRAZY_BOOL_False;
	
	if(_current_assigned_lane != _destination_lane && _current_assigned_lane->getParent() == _destination_lane->getParent()){
		//vehical can be caught.., check the vehical in the opposite lane..,
		switch(_detect_target_vehical_in_oppositelane(_vehical , _vehical_opposit , _destination_lane))
		{
		case KRAZY_VEHICAL_STATE_IN_SIBLINLANES_approaching_from_opposite:
			_vehical_state = KRAZY_VEHICAL_STATE_IN_SIBLINLANES_approaching_from_opposite;
			return KRAZY_BOOL_True;
			//.....
		case KRAZY_VEHICAL_STATE_IN_SIBLINLANES_passing_from_opposite:
			_vehical_state = KRAZY_VEHICAL_STATE_IN_SIBLINLANES_passing_from_opposite;
			return KRAZY_BOOL_True;
			//..,
		case KRAZY_VEHICAL_STATE_IN_SIBLINLANES_passed_from_opposite:
			 _vehical_state = KRAZY_VEHICAL_STATE_IN_SIBLINLANES_passed_from_opposite;
			 return KRAZY_BOOL_True;
			 //..,
		}
	}
	return KRAZY_BOOL_False;
}
//
KrazyBool KrazyDefaultAlgoMovingVehical_01Phase::_check_vehical_overlap_with_indicator(KrazyVehical* _vehical , KrazyTrafficIndicator* _indicator)
{
	KrazyLaneNode* _assigned_lane  = _vehical->PTRCurrentAssignedSingleLane();
	KrazyLaneNode* _indicator_lane = _indicator->PTRParentLaneNode();
	if(_assigned_lane == _indicator_lane){
		switch(_assigned_lane->getProperties()->get_LaneDirection())
		{
			case KRAZY_LANE_DIRECTION_UP_HORZS   : break;
			case KRAZY_LANE_DIRECTION_DOWN_HORZS : break;
			case KRAZY_LANE_DIRECTION_UP_VERTS   : break;
			case KRAZY_LANE_DIRECTION_DOWN_VERTS : break;
		}
	}
	else
	{
		int i = 0;
	}

	return KRAZY_BOOL_False;
}
//..
void KrazyDefaultAlgoMovingVehical_01Phase::get_route_for_block(KrazyVehical* _vehical , KrazyTrafficIndicator* _block , KrazyLaneNode* _source_lane , KrazyLaneNode* _destination_lane , KrazyTrafficIndicatorList* _car_route , KrazyRouteSearchType _searchType /*=  KRAZY_ROUTE_SEARCH_for_SHORTEST*/)
{
	_car_route->clear();
	switch(_searchType)
	{
	case KRAZY_ROUTE_SEARCH_for_SHORTEST:
		switch(_detect_if_short_route_detection_is_required(_source_lane , _destination_lane , _car_route))
		{
		case KRAZY_BOOL_True:
			_VecSearchOrigin = *_vehical->VectorFrontBumper();
			KrazyDefaultAlgoMovingVehical_01Phase::get_ShortestRouteMap(_source_lane , _destination_lane , _car_route);
			break;
		case KRAZY_BOOL_False:
#ifdef _KRAZY_ALGO_TRACE_ENBALED
			printf("\n srd not required for %s" , _vehical->to_string().c_str());
#endif
			_fix_incomplete_route_if_any_for_blocks(_vehical , _car_route);
			break;
		}
		break;
	case KRAZY_ROUTE_SEARCH_for_LONGEST:break;
	}
}
//..,
void KrazyDefaultAlgoMovingVehical_01Phase::_fix_incomplete_route_if_any_for_blocks(KrazyVehical* _vehical ,  KrazyTrafficIndicatorList*   _route)
{

	int _count = _route->size();
	KrazyLaneNode* _assigned_lane = _vehical->PTRCurrentAssignedSingleLane();
	KrazyTrafficIndicator* _blocking = _route->at(0);
	KrazyTrafficIndicator* _destination = 0;
	KrazyLaneNode* _destination_parent  = 0;
	KrazyLaneNode* _blocking_parent     = 0;
	//..,
	bool _check_flip = false;
	for(int indc = _count - 1; indc > -1 && !_check_flip; indc--){
		KrazyTrafficIndicator* _turnning = _route->at(indc);
		switch(_turnning->Type())
		{
		case KRAZY_TRAFFIC_INDC_Turn :  
			 _destination = _turnning->get_DestinationIndc(0);
			 _destination_parent  = _destination->PTRParentLaneNode();
			 _blocking_parent     = _blocking->PTRParentLaneNode();			 
			 if(_destination_parent == _blocking_parent){
				 _check_flip = true;
				 continue;
			 }
			 break;
		}
	}
	//..,
	if(_check_flip){
		glm::vec2* _vec_destination = _destination->Vector();
		glm::vec2* _vec_blocking    = _blocking->Vector();
		KrazyBool  _add_extra_flip  = KRAZY_BOOL_False;
		switch(_blocking_parent->getProperties()->get_LaneDirection())
		{
			case KRAZY_LANE_DIRECTION_UP_HORZS		: if(_vec_destination->x < _vec_blocking->x) _add_extra_flip = KRAZY_BOOL_True; break;
			case KRAZY_LANE_DIRECTION_DOWN_HORZS	: if(_vec_destination->x > _vec_blocking->x) _add_extra_flip = KRAZY_BOOL_True; break;
			case KRAZY_LANE_DIRECTION_UP_VERTS		: if(_vec_destination->y > _vec_blocking->y) _add_extra_flip = KRAZY_BOOL_True; break;
			case KRAZY_LANE_DIRECTION_DOWN_VERTS	: if(_vec_destination->y < _vec_blocking->y) _add_extra_flip = KRAZY_BOOL_True; break;
		}
		//..,
		switch(_add_extra_flip)
		{
		case KRAZY_BOOL_True : 
			{
				KrazyTrafficIndicatorList _route_list;
				_route_list.push_back(_blocking);
				_route_list.push_back(_blocking_parent->getOppositeSingleLane()->getFlipTrafficIndicator());
				_route_list.push_back(_blocking_parent->getFlipTrafficIndicator());
				KrazyBool _add = KRAZY_BOOL_False;
				for(int i = 0; i < _count; i++){
					if(_route->at(i) == _blocking){
						_add = KRAZY_BOOL_True;
						continue;
					}
					//..,
					switch(_add) {
						case KRAZY_BOOL_True : _route_list.push_back(_route->at(i)); break;
					}
					//..,
				}
				//..,
				_route->clear();
				_count = _route_list.size();
				for(int i = 0; i < _count; i++){
					_route->push_back(_route_list[i]);
				}
			 }
			 break;
		}
	}
	//..,
}