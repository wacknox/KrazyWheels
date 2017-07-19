#include "StdAfx.h"
#include "KrazyRouteThriver.h"
#include "KrazyPrimitiveDataCollections.h"
#include "KrazyDutyKueue.h"
#include "KrazyVehical.h"
#include "KrazyLaneNode.h"
#include "KrazyTrafficIndicator.h"
#include "KrazyVehicalGameplayStateMovingPolicy_01Phase.h"
#include "KrazyLevelThriver.h"
//..,
#define _KRAZY_FOUND_RESPECTIVE_VEHICAL(_monitoring_vehical , _respective_vehical)  \
	char _msg[1024];sprintf(_msg , "%s Found %s" , _monitoring_vehical->to_string().c_str() , _respective_vehical->to_string().c_str()); \
	KrazyLevelThriver::get_default()->help_tip() = string(_msg); \
	KrazyLevelThriver::get_default()->level_mode() = KRAZY_LEVELMODE_gameover; \

KrazyUnaBrotherRouteThriver::KrazyUnaBrotherRouteThriver(void) :
KrazyRouteThriver(KRAZY_ROUTE_THRIVER_UnaFamily)
{
	reset();
}
//..,
KrazyUnaBrotherRouteThriver::~KrazyUnaBrotherRouteThriver(void)
{
	clean_dutylist();
}
//..,
void KrazyUnaBrotherRouteThriver::on_attach_vehical(KrazyVehical* _vehical)
{
	_AssignedVehical = _vehical;
}
//..,
KrazyBool KrazyUnaBrotherRouteThriver::execute(KrazyVehicalGameplayStateMovingPolicy_01Phase* _policy)
{
	if(!_AssignedVehical)
		return KRAZY_BOOL_False;
	
	//...,
	switch(_State)
	{
	case KRAZY_ROUTETHRIVER_STATE_MovingNormal          : return onExecuteMovingNormal(_policy);       
	case KRAZY_ROUTETHRIVER_STATE_InSearchShortPath     : return onExecuteInSearchShortPath(_policy);   
	case KRAZY_ROUTETHRIVER_STATE_RunningWithShortPath  : return onExecuteRunningWithShortPath(_policy);
	case KRAZY_ROUTETHRIVER_STATE_Stop_Respective_Vehical_if_Found : return onExecuteRunningStopRespectiveVehical(_policy);
	}
	//..,
	return KRAZY_BOOL_False;
}
//..,
void KrazyUnaBrotherRouteThriver::clean_dutylist()
{
	for(;_DutyList.size() > 0;){
		KrazyDuty* _duty = _DutyList.front();
		delete _duty;_duty = 0;_DutyList.pop();
	}
}
//..,
void KrazyUnaBrotherRouteThriver::reset()
{
	_AssignedVehical			= 0;
	_RespectiveVehical_to_Stop  = 0;
	_State						= KRAZY_ROUTETHRIVER_STATE_MovingNormal;
	clean_dutylist();
}
//...,
void KrazyUnaBrotherRouteThriver::reset_for_gameplay()
{
	_State = KRAZY_ROUTETHRIVER_STATE_MovingNormal;
	clean_dutylist();
}
//..,
KrazyBool KrazyUnaBrotherRouteThriver::onExecuteMovingNormal(KrazyVehicalGameplayStateMovingPolicy_01Phase* _policy)
{
	switch(_AssignedVehical->waitstate())
	{
	case KRAZY_VEHICAL_WAIT_STATE_Default : break;
	default: return KRAZY_BOOL_False;
	}
	//..,
	KrazyDutyKueue* _dutyQ = KrazyDutyKueue::getDefault();
	KrazyDuty* _duty = _dutyQ->getNextDuty();
	if(!_duty)
		return KRAZY_BOOL_False;
	//..,
	switch(_duty->duty_id())
	{
	case KRAZY_BROTHER_DUTY_Search_for_block   : _State = KRAZY_ROUTETHRIVER_STATE_InSearchShortPath; break;
	case KRAZY_BROTHER_DUTY_Search_for_vehical : 
		{			
			_dutyQ->enable() = KRAZY_BOOL_False;
			KrazyUnaBrotherDuty_for_StoppingVehical* _duty_to_stop_vehical = (KrazyUnaBrotherDuty_for_StoppingVehical*)_duty;
			_RespectiveVehical_to_Stop = _duty_to_stop_vehical->data();
			_State = KRAZY_ROUTETHRIVER_STATE_Stop_Respective_Vehical_if_Found;
			KLEAN_a_bit_of_KRAZINESS(_duty);
		}
		return KRAZY_BOOL_True;
	}
	//..,
	_DutyList.push(_duty);	
	//..,
	return KRAZY_BOOL_True;
}
//...,
KrazyBool KrazyUnaBrotherRouteThriver::onExecuteInSearchShortPath(KrazyVehicalGameplayStateMovingPolicy_01Phase* _policy)
{
	KrazyBool  _res =  KRAZY_BOOL_False;
	KrazyDuty* _duty = _DutyList.front();_DutyList.pop();
	switch(_duty->duty_id())
	{
	case KRAZY_BROTHER_DUTY_Search_for_block : 
		onExecuteSearch_for_Blocks(_policy , (KrazyUnaBrotherDuty_for_SearchingBlocks*)_duty); 
		KLEAN_a_bit_of_KRAZINESS(_duty);
		break;
	}
	//...,
	return _res;
}
//...,
KrazyBool KrazyUnaBrotherRouteThriver::onExecuteRunningWithShortPath(KrazyVehicalGameplayStateMovingPolicy_01Phase* _policy)
{
	KrazyBool _res = KRAZY_BOOL_False;
	return _res;
}
//...,
KrazyBool KrazyUnaBrotherRouteThriver::on_vehical_turned(KrazyVehicalGameplayStateMovingPolicy_01Phase* _policy)
{
	KrazyBool _res = KRAZY_BOOL_False;
	return _res;
}
//...,
KrazyBool KrazyUnaBrotherRouteThriver::onExecuteSearch_for_Blocks(KrazyVehicalGameplayStateMovingPolicy_01Phase* _policy , KrazyUnaBrotherDuty_for_SearchingBlocks* _duty_for_search_block)
{
	KrazyBool _res = KRAZY_BOOL_False;
	KrazyTrafficIndicator* _indicator = _duty_for_search_block->data();
	KrazyLaneNode* _destination_single_lane = _indicator->PTRParentLaneNode();
	KrazyLaneNode* _source_lane = _AssignedVehical->PTRCurrentAssignedSingleLane();
	KrazyTrafficIndicatorList* _car_route = _AssignedVehical->get_TrafficIndicatorMap();
	//..,
	switch(short_route_required_for_block(_source_lane , _destination_single_lane , _indicator))
	{
		
	case KRAZY_BOOL_True	:
		{
			/*KrazyTrafficIndicatorList* _indc_list = _destination_single_lane->get_TrafficIndicators();*/
			_destination_single_lane->setDestinationEnd(_indicator);
			_AssignedVehical->on_setDestinationLane(_destination_single_lane);
			_policy->get_route_for_block(_AssignedVehical , _indicator , _source_lane , _destination_single_lane , _car_route);
#if 0
			_policy->adjust_blocks_in_the_reckoning(_AssignedVehical);
#endif
			
		}
		break;
	}
	//..,
	int _routes = _car_route->size();_State = KRAZY_ROUTETHRIVER_STATE_RunningWithShortPath;
	if(_routes > 0){
		_AssignedVehical->route_mode() = KRAZY_ROUTEMODE_Reckoning;
		_AssignedVehical->on_setNextTurnIndex(_routes - 1);
		_AssignedVehical->gameplay_turns() = 0;
		_AssignedVehical->moving_after_touch() = KRAZY_BOOL_False;
		_AssignedVehical->gameplay_movingstate() = KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Default;
	}
	else{
		printf("\n something wrong with finding shrt path for %s !" , _AssignedVehical->to_string().c_str());
	}
	//..,
	return _res;
}
//...,
KrazyBool KrazyUnaBrotherRouteThriver::onExecuteRunningStopRespectiveVehical(KrazyVehicalGameplayStateMovingPolicy_01Phase* _policy)
{
	if(_RespectiveVehical_to_Stop){
		if(_AssignedVehical->PTRFrontVehical() == _RespectiveVehical_to_Stop || 
			_AssignedVehical->PTRRearVehical() == _RespectiveVehical_to_Stop){
			_KRAZY_FOUND_RESPECTIVE_VEHICAL(_AssignedVehical , _RespectiveVehical_to_Stop);
			return KRAZY_BOOL_True;
		}
		//..,
		KrazyLaneNode* _assigned_vehical_lane   =  _AssignedVehical->PTRCurrentAssignedSingleLane();
		KrazyLaneNode* _respective_vehical_lane = _RespectiveVehical_to_Stop->PTRCurrentAssignedSingleLane();
		//..,
		if(_assigned_vehical_lane != _respective_vehical_lane && 
			_assigned_vehical_lane->getParent() == _respective_vehical_lane->getParent()){
				switch(_policy->_detect_target_vehical_in_oppositelane(_AssignedVehical , _RespectiveVehical_to_Stop , _assigned_vehical_lane))
				{
					case KRAZY_VEHICAL_STATE_IN_SIBLINLANES_approaching_from_opposite:
					case KRAZY_VEHICAL_STATE_IN_SIBLINLANES_passing_from_opposite:
						 _KRAZY_FOUND_RESPECTIVE_VEHICAL(_AssignedVehical , _RespectiveVehical_to_Stop);
						 return KRAZY_BOOL_True;
				}
		}
	}
	//..,
	return KRAZY_BOOL_False;
}
//...,
KrazyBool KrazyUnaBrotherRouteThriver::short_route_required_for_block(KrazyLaneNode* _source_lane , KrazyLaneNode* _destination_lane , KrazyTrafficIndicator* _blocking)
{
	KrazyTrafficIndicatorList* _car_route = _AssignedVehical->get_TrafficIndicatorMap();
	glm::vec2* _vec_front_bumper = _AssignedVehical->VectorFrontBumper();
	glm::vec2* _vec_rear_bumper  = _AssignedVehical->VectorRearBumper();
	glm::vec2* _vec_blocking     = _blocking->Vector();
	_car_route->clear();
	KrazyBool _res = KRAZY_BOOL_True;
	KrazyLaneNode* _source_parent = _source_lane->getParent();
	KrazyLaneNode* _destination_parent = _destination_lane->getParent();
	if(_source_lane == _destination_lane){
		KrazyBool _add_extra_flip = KRAZY_BOOL_False;
		switch(_destination_lane->getProperties()->get_LaneDirection())
		{
		case KRAZY_LANE_DIRECTION_UP_HORZS		: if(_vec_rear_bumper->x < _vec_blocking->x) _add_extra_flip = KRAZY_BOOL_True; break;
		case KRAZY_LANE_DIRECTION_DOWN_HORZS	: if(_vec_rear_bumper->x > _vec_blocking->x) _add_extra_flip = KRAZY_BOOL_True; break;
		case KRAZY_LANE_DIRECTION_UP_VERTS		: if(_vec_rear_bumper->y > _vec_blocking->y) _add_extra_flip = KRAZY_BOOL_True; break;
		case KRAZY_LANE_DIRECTION_DOWN_VERTS	: if(_vec_rear_bumper->y < _vec_blocking->y) _add_extra_flip = KRAZY_BOOL_True; break;
		}
		//..,
		switch(_add_extra_flip)
		{
		case KRAZY_BOOL_True  : 
			 _car_route->push_back(_source_lane->getFlipTrafficIndicator());
			 _car_route->push_back(_blocking);
			 _car_route->push_back(_source_lane->getOppositeSingleLane()->getFlipTrafficIndicator());
			 _car_route->push_back(_source_lane->getFlipTrafficIndicator());
			 break;
		case KRAZY_BOOL_False : 
			 _car_route->push_back(_source_lane->getFlipTrafficIndicator());
			 _car_route->push_back(_blocking);
			 break; 
		}
		//...,
		_res = KRAZY_BOOL_False;
	}
	//..,
	else if(_source_lane != _destination_lane && 
		    _source_parent == _destination_parent){
			_car_route->push_back(_destination_lane->getFlipTrafficIndicator());
			_car_route->push_back(_blocking);
			_car_route->push_back(_source_lane->getFlipTrafficIndicator());
			_res = KRAZY_BOOL_False;
	}
	//..,
	return _res;
}
//...,
KrazyRouteThriverFactory*   KrazyRouteThriverFactory::_Me = 0;
KrazyRouteThriverFactory*	KrazyRouteThriverFactory::getDefault()
{
	if(!_Me){
		_Me = new KrazyRouteThriverFactory;
	}
	//..,
	return _Me;
}
//..,
void KrazyRouteThriverFactory::releaseDefault()
{
	if(_Me){
		delete _Me;_Me = 0;
	}
}
//..,
KrazyRouteThriverFactory::KrazyRouteThriverFactory()
{
}
//..,
KrazyRouteThriverFactory::~KrazyRouteThriverFactory()
{
	_KRAZY_DEALLOCATE_LIST(_RouteThrivers);
}
//..,
KrazyRouteThriver* KrazyRouteThriverFactory::getNewRouteThriver(KrazyRouteThriverName _thriver_name)
{
	//..,
	KrazyRouteThriver* _thriver = 0;
	switch(_thriver_name)
	{
	case KRAZY_ROUTE_THRIVER_UnaFamily : 
		_thriver = new KrazyUnaBrotherRouteThriver;
		_RouteThrivers.push_back(_thriver);
		break;
	}
	//..,
	return _thriver;
}
