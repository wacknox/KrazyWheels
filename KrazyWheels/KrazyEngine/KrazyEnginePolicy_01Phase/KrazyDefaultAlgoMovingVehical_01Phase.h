#pragma once
#include "KrazyInterfaceGamePlayPolicy.h"
#include "KrazyAlgoDiagnosticData_01Phase.h"
#include "KrazyPrimitiveDataCollections.h"
#include "KrazyEnums.h"
#include "KrazyGLMInclusion.h"
//#define _KRAZY_ALGO_TRACE_ENBALED 
#define  MAX_WAIT_FOR_BLOCKING_CARS  200
#define _KRIME_SIMULATION_ON_TOUCH
//#define _UN_OPT_PATH_SEARCH
//
enum KrazyVehicalStateInSiblingLanes
{
	KRAZY_VEHICAL_STATE_IN_SIBLINLANES_Default                   ,
	KRAZY_VEHICAL_STATE_IN_SIBLINLANES_approaching_from_opposite ,
	KRAZY_VEHICAL_STATE_IN_SIBLINLANES_passing_from_opposite     ,
	KRAZY_VEHICAL_STATE_IN_SIBLINLANES_passed_from_opposite      ,
}
;
//
enum KrazyRouteSearchType
{
	KRAZY_ROUTE_SEARCH_Default , 
	KRAZY_ROUTE_SEARCH_for_SHORTEST,
	KRAZY_ROUTE_SEARCH_for_LONGEST,
};
//
struct Vector2;
class KrazyVehical;
class KrazyLaneNode;
class KrazyLaneProperties;
class KrazyTrafficIndicator;
class KrazyTrafficIndicatorList;
#ifdef _UN_OPT_PATH_SEARCH
struct routing_index;
#else
struct KrazyShortPathRoutingIndexer;
#endif

static float MATCH_SPACE_THRESHOLD = 0.1f;
static float GAP_ = 0.25f;
static const float _KRAZY_TRAFFIC_INDC_SPACE_MIN = 0.04f;
static const float _KRAZY_TRAFFIC_INDC_SPACE_MAX = 0.06f;
//
class KrazyDefaultAlgoMovingVehical_01Phase : public KrazyInterfaceGamePlayPolicy<KrazyALGODiagnostic>
{
public:
	KrazyDefaultAlgoMovingVehical_01Phase(void);
	virtual ~KrazyDefaultAlgoMovingVehical_01Phase(void);
	void onExecute(KrazyVehical* vehical , KrazyALGODiagnostic** diagnostic){}
	void reset(){}
	bool matchDistance(KrazyVehical* vehical);
	void createRouteMap(KrazyVehical* vehical);
	KrazyBool get_DetectedTrafficIndicatorByVehical(KrazyVehical* ptr_vehical , KrazyTrafficIndicator** ptr_trafficIndicator);
	KrazyBool get_DetectedTrafficIndicatorSeq(KrazyVehical* ptr_vehical , KrazyTrafficIndicator** ptr_trafficIndicator);
	KrazyBool get_DetectedTrafficIndicator_in_Gameplay_ByVehical(KrazyVehical* ptr_vehical , KrazyTrafficIndicator** ptr_trafficIndicator);
	// rem - 00..b
	bool get_DetectedTrafficIndicatorByLane(KrazyVehical* ptr_vehical , KrazyTrafficIndicator** ptr_trafficIndicator);
	// rem - 00..e
	void get_ShortestRouteMap(KrazyLaneNode* ptr_source_lane , KrazyLaneNode* ptr_destination_lane , KrazyTrafficIndicatorList* ptr_route_map);
	void getConnectedTrafficIndicatorsandDistance(KrazyLaneNode* source , float& distance , KrazyTrafficIndicatorList* r_list);
	bool shouldAddExtraFlips(KrazyLaneNode* lane , KrazyTrafficIndicator* turn  , KrazyTrafficIndicator* dest);
	void flip_on_spot(KrazyVehical* ptr_vehical , KrazyLaneNode* _next_sibiling_lane);
	KrazyVehicalStateInSiblingLanes _detect_target_vehical_in_oppositelane(KrazyVehical* _monitoring_vehical , KrazyVehical* _target_vehical , KrazyLaneNode* _opposite_lane);
	//
#ifdef  _UN_OPT_PATH_SEARCH
	bool   KrazyDefaultAlgoMovingVehical_01Phase::getRouteWithDistance(
											    KrazyLaneNode* ptr_source_lane ,  KrazyLaneNode* ptr_destination_lane , 
												KrazyLaneNode* ptr_prev , KrazyTrafficIndicatorList** ptr_route_map, float& distance ,  hash_map<KrazyLaneNode* , routing_index> * ptr_indexer = 0);
#else
	void   getRouteWithDistance(KrazyLaneNode* ptr_source_lane ,  KrazyLaneNode* ptr_destination_lane , KrazyLaneNode* ptr_curr,
								 KrazyLaneNode* ptr_prev , KrazyTrafficIndicatorList* ptr_route_map , unsigned int case_include_origin = 0);

#endif
	void get_route_for_vehical(KrazyVehical* _vehical , KrazyLaneNode* _source_lane , KrazyLaneNode* _destination_lane , KrazyTrafficIndicatorList* _car_route , KrazyRouteSearchType _searchType =  KRAZY_ROUTE_SEARCH_for_SHORTEST);
	void get_route_for_block(KrazyVehical* _vehical , KrazyTrafficIndicator* _block , KrazyLaneNode* _source_lane , KrazyLaneNode* _destination_lane , KrazyTrafficIndicatorList* _car_route , KrazyRouteSearchType _searchType =  KRAZY_ROUTE_SEARCH_for_SHORTEST);
	KrazyTrafficIndicator* getOriginTrafficIndicator(KrazyVehical* ptr_vehical , int index = 0);
	KrazyBool _detect_if_short_route_detection_is_required(KrazyLaneNode* _source_lane , KrazyLaneNode* _destination_lane ,  KrazyTrafficIndicatorList* ptr_route_map);
	void _fix_incomplete_route_if_any(KrazyTrafficIndicator* _origin_indicator , KrazyTrafficIndicatorList*   _route);
	void _fix_incomplete_route_if_any_for_vehical(KrazyVehical* _vehical , KrazyTrafficIndicatorList*   _route);
	void _fix_incomplete_route_if_any_for_blocks(KrazyVehical* _vehical , KrazyTrafficIndicatorList*   _route);
	KrazyBool getFinishingTrafficIndicator(KrazyVehical* ptr_vehical , KrazyTrafficIndicator** ptr_last_indc_met);
	KrazyBool _check_opposite_lane_vehical(KrazyVehical* _vehical , KrazyVehical* _vehical_opposit , 
	                      KrazyVehicalStateInSiblingLanes& _vehical_state);

	KrazyBool  _check_vehical_overlap_with_indicator(KrazyVehical* _vehical , KrazyTrafficIndicator* _indicator);
	// events.. like ,
	void on_vehicalTurn(KrazyVehical* ptr_vehical , KrazyTrafficIndicator* ptr_traffic_indc_matched);
	void cleanUp();

	float _CurrentDistance;
	float _RealDistance;
	KrazyTrafficIndicatorList* _PTRAuxshortroutemap;
	glm::vec2  _VecSearchOrigin;
	//For Debugging ...................
	int getLaneIndex(KrazyLaneNode* _lane);
	//
};

