#include "StdAfx.h"
#include "KrazyDefaultAlgoMovingVehical_01Phase.h"
#include "KrazyLaneNode.h"
/////////////////////////////////////////////////////////
KrazyDefaultAlgoMovingVehical_01Phase::KrazyDefaultAlgoMovingVehical_01Phase(void)
{
}
////////////////////////////////////////////////////////
KrazyDefaultAlgoMovingVehical_01Phase::~KrazyDefaultAlgoMovingVehical_01Phase(void)
{
}
////////////////////////////////////////////////////////
bool KrazyDefaultAlgoMovingVehical_01Phase::matchDistance(KrazyVehical* ptr_vehical)
{
	KrazyVehical* ptr_vehical_front = ptr_vehical->PTRFrontVehical();
	if(ptr_vehical_front){
		Vector2* ptr_vehical_vector_front_bumper = ptr_vehical->VectorFrontBumper();
		Vector2* ptr_front_vehical_vector_rear_bumper = ptr_vehical_front->VectorRearBumper();
		return ( ptr_vehical_vector_front_bumper->distance(*ptr_front_vehical_vector_rear_bumper) > 0.001f );
	}
	return true;
}
/////////////////////////////////////////////////////////
void KrazyDefaultAlgoMovingVehical_01Phase::createRouteMap(KrazyVehical* ptr_vehical)
{
	KrazyLaneNode* ptr_assigned_single_lane = ptr_vehical->PTRCurrentAssignedSingleLane();
	if(!ptr_assigned_single_lane)
		return;

	KrazyTrafficIndicatorList* ptr_car_route = ptr_vehical->get_TrafficIndicatorMap();
	KrazyTrafficIndicatorList* ptr_trafficindc_list = ptr_assigned_single_lane->get_TrafficIndicators();
	KrazyLaneProperties* properties = ptr_assigned_single_lane->getProperties();
	int count = ptr_trafficindc_list->size();
	for(int i = 0; i < count; i++){
		KrazyTrafficIndicator* ptr_traffic_indc = ptr_trafficindc_list->at(i);
		Vector2* ptr_traffic_indc_vec = ptr_traffic_indc->Vector();
		Vector2* ptr_vehical_vec = ptr_vehical->Vector();
		switch(properties->get_LaneDirection())
		{
		case KRAZY_LANE_DIRECTION_UP_HORZS     : if(ptr_traffic_indc_vec->x < ptr_vehical_vec->x) {ptr_car_route->push_back(ptr_traffic_indc);}break;
		case KRAZY_LANE_DIRECTION_DOWN_HORZS   : if(ptr_traffic_indc_vec->x > ptr_vehical_vec->x) {ptr_car_route->push_back(ptr_traffic_indc);}break;
		case KRAZY_LANE_DIRECTION_UP_VERTS     : if(ptr_traffic_indc_vec->y > ptr_vehical_vec->y) {ptr_car_route->push_back(ptr_traffic_indc);}break;
		case KRAZY_LANE_DIRECTION_DOWN_VERTS   : if(ptr_traffic_indc_vec->y < ptr_vehical_vec->y) {ptr_car_route->push_back(ptr_traffic_indc);}break;
		}
	}

}
/////////////////////////////////////////////////////////
bool KrazyDefaultAlgoMovingVehical_01Phase::get_DetectedTrafficIndicator(KrazyVehical* ptr_vehical , KrazyTrafficIndicator** ptr_trafficIndicator)
{
	KrazyTrafficIndicatorList* ptr_trafficIndcList = ptr_vehical->get_TrafficIndicatorMap();
	int traffic_indc_count = ptr_trafficIndcList->size();
	KrazyLaneNode* ptr_single_lane_node = ptr_vehical->PTRCurrentAssignedSingleLane();
	for(int traffic_indc = ptr_vehical->getNextTurnIndex();traffic_indc < traffic_indc_count;traffic_indc++){
		KrazyTrafficIndicator* ptr_traffic_indicator = ptr_trafficIndcList->at(traffic_indc);
		Vector2* ptr_vector_traffic_indc = ptr_traffic_indicator->Vector();
		Vector2* ptr_vector_vehical_front_bumper_vector = ptr_vehical->VectorFrontBumper();
		float distance = ptr_vector_traffic_indc->distance(*ptr_vector_vehical_front_bumper_vector);
		if((distance >= 0.001f && distance <= 0.002f)){
			if(ptr_traffic_indicator->is_alreadyTaken()){
				ptr_traffic_indicator->updateTaken();
				ptr_vehical->on_setNextTurnIndex(traffic_indc + 1);
				continue;
			}
			if(ptr_traffic_indicator->Type() == 1)
				ptr_traffic_indicator->updateTaken();

			ptr_vehical->on_setNextTurnIndex(0);
			*ptr_trafficIndicator = ptr_traffic_indicator;
			return true;
		}
		return false;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////