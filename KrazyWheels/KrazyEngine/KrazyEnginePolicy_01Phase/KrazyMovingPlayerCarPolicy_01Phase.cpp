#include "StdAfx.h"
#include "KrazyVehicalCache.h"
#include "KrazyMovingPlayerCarPolicy_01Phase.h"
#include "KrazyLaneNode.h"
#include "KrazyRedundentCallHelperMacros.h"
#include "KrazyLevelCityNetwork.h"
#include "KrazyUNTanglers.h"
#include "KrazyUtilityMacros.h"
#include "KrazyLevelThriver.h"
#include "KrazyTutorialThriver.h"
/////////////////////////////////////////////////
KrazyMovingPlayerCarPolicy_01Phase::KrazyMovingPlayerCarPolicy_01Phase() 
{
}
////////////////////////////////////////////////
KrazyMovingPlayerCarPolicy_01Phase::~KrazyMovingPlayerCarPolicy_01Phase(void)
{
}
////////////////////////////////////////////////
void KrazyMovingPlayerCarPolicy_01Phase::reset()
{
	_RangtoonaMoving.reset();
	_BangtoonaMoving.reset();
	
	_GangiMoving.reset();
	_SherrifMoving.reset();
	_PangunaMoving.reset();
	_NangunaMoving.reset();
	
	_GangiTutorial.reset();
	_PangunaTutorial.reset();
	_NangunaTutorial.reset();
	_RangToonaTutorial.reset();
	_BangToonaTutorial.reset();
	_SherrifTutorial.reset();
}
//..,
bool KrazyMovingPlayerCarPolicy_01Phase::onExecute()
{

#if 1
	KrazyNetworkCity_01Phase* ptr_parent_network = KrazyNetworkCity_01Phase::getNetwork();
	vector<KrazyWheelNetwork_01Phase*>* ptr_network_nodes = ptr_parent_network->getNodeCache();
#endif

	KrazyVehicalCache* ptr_cache = KrazyVehicalCache::getCache(); // make the list directly retrieved..,
	int vehical_count = ptr_cache->get_VehicalsInTheNetworkCount();
	KrazyALGODiagnostic* ptr_diagnostic = 0;
	//
	for(int i = 0; i < vehical_count; i++){
		KrazyVehical* ptr_vehical = ptr_cache->get_VehicalFromTheNetwork(i);// change it to ..,direct list ..,
		KrazyVehicalWaitState vehical_wait_state = ptr_vehical->waitstate();
		switch(vehical_wait_state)
		{
#if 0
		case KRAZY_VEHICAL_WAIT_STATE_Turning  :
			{  
				KrazyVehical* ptr_rear_vehical = ptr_vehical->PTRRearVehical();
				if(ptr_rear_vehical){
					KrazyTrafficIndicator* ptr_turn  = ptr_vehical->get_NextTurn(); 
					KrazyLaneNode* ptr_assigned_lane = ptr_turn->PTRParentLaneNode();
					KrazyLaneDirection direction     = ptr_assigned_lane->getProperties()->get_LaneDirection();
					int turn_index = ptr_turn->index();
					KrazyTrafficIndicator* ptr_next_traffic_indicator = (turn_index > 0) ? ptr_assigned_lane->get_TrafficIndicator(--turn_index) : ptr_turn;
					glm::vec2* ptr_vec_indicator = ptr_next_traffic_indicator->Vector();
					KrazyVehicalList* ptr_prev_to_be_Q = ptr_vehical->get_QueuedPreviousVehicals();
					for(;ptr_rear_vehical;){
						glm::vec2* ptr_vec_vehical = ptr_rear_vehical->VectorFrontBumper();
						switch(ptr_rear_vehical->visibilitystate()){
						case KRAZY_BOOL_False: ptr_rear_vehical = 0;continue;
						}
						switch(ptr_rear_vehical->waitstate())
						{
						case KRAZY_VEHICAL_WAIT_STATE_Turned : 
#if 1
							KrazyUNTanglers::_unqueue_prev_vehical_after_turn(ptr_rear_vehical);
#else
							KrazyVehicalList* ptr_prev_Q = ptr_rear_vehical->get_QueuedPreviousVehicals();
							int count = ptr_prev_Q->size();
							for(int i = 0; i < count; i++){
								KrazyVehical* ptr_prev = ptr_prev_Q->at(i);
								switch(ptr_prev->waitstate())
								{
								case KRAZY_VEHICAL_WAIT_STATE_Queued:ptr_prev->decrement_qcounter();break;
								default:
									{
										int i = 0;
									}
									break;
								}
							}
							ptr_prev_Q->clear();
							ptr_rear_vehical->waitstate() = KRAZY_VEHICAL_WAIT_STATE_Default;
							ptr_rear_vehical->on_laneChange(0);
#endif
							break;
						}
						switch(direction)
						{
							// from -ve to +ve;
						case KRAZY_LANE_DIRECTION_UP_VERTS     : 
							if(ptr_vec_vehical->y < ptr_vec_indicator->y){
								/* 
								there are other vehicals .., which starts after this indicator need 
								not to be queued .., coz they can have seprate route..,
								*/
								ptr_rear_vehical = 0;continue;
							}
							break;
						case KRAZY_LANE_DIRECTION_DOWN_HORZS   : 
							if(ptr_vec_vehical->x < ptr_vec_indicator->x){
								/* 
								there are other vehicals .., which starts after this indicator need 
								not to be queued .., coz they can have seprate route..,
								*/
								ptr_rear_vehical = 0;continue;
							}
							break;
							// from +ve to -ve;
						case KRAZY_LANE_DIRECTION_UP_HORZS     :
							if(ptr_vec_indicator->x < ptr_vec_vehical->x){
								/* 
								there are other vehicals .., which starts after this indicator need 
								not to be queued .., coz they can have seprate route..,
								*/
								ptr_rear_vehical = 0;continue;
							}
							break;
						case KRAZY_LANE_DIRECTION_DOWN_VERTS   : 
							if(ptr_vec_indicator->y < ptr_vec_vehical->y){
								/* 
								there are other vehicals .., which starts after this indicator need 
								not to be queued .., coz they can have seprate route..,
								*/
								ptr_rear_vehical = 0;continue;
							}
							break;
						}//end switch direction..
					//	ptr_rear_vehical->increment_qcounter();
					//	ptr_prev_to_be_Q->push_back(ptr_rear_vehical);
						ptr_rear_vehical = ptr_rear_vehical->PTRRearVehical();
					}//end for(;ptr_rear_vehical;)
				}//end if(ptr_rear_vehical)
			}
			break;
#endif
		case KRAZY_VEHICAL_WAIT_STATE_Turned   :
			{
#if 1
				//KrazyUNTanglers::_unqueue_prev_vehical_after_turn(ptr_vehical);
				ptr_vehical->on_default();
#else
				KrazyVehicalList* ptr_prev_Q = ptr_vehical->get_QueuedPreviousVehicals();
				int count = ptr_prev_Q->size();
				for(int i = 0; i < count; i++){
					KrazyVehical* ptr_prev = ptr_prev_Q->at(i);
					switch(ptr_prev->waitstate())
					{
					case KRAZY_VEHICAL_WAIT_STATE_Queued:ptr_prev->decrement_qcounter();break;
					default:
						{
							int i = 0;
						}
						break;
					}
				}
				ptr_prev_Q->clear();
				ptr_vehical->waitstate() = KRAZY_VEHICAL_WAIT_STATE_Default;
				ptr_vehical->on_laneChange(0);
#endif
			}
			break;
		case KRAZY_VEHICAL_WAIT_STATE_Queued   :  continue;
		}//end switch vehicalstate..,
		//..,
		switch(KrazyTutorialThriver::get_default()->mode()){
		case KRAZY_BOOL_True : 
			 switch(ptr_vehical->vehical_name()){
			 case KRAZY_VEHICAL_in_GAMEPLAY_RangToona    : _RangToonaTutorial.onExecute(ptr_vehical);   break;
			 case KRAZY_VEHICAL_in_GAMEPLAY_Ganungi	     : _GangiTutorial.onExecute(ptr_vehical);       break;
			 case KRAZY_VEHICAL_in_GAMEPLAY_BangToona    : _BangToonaTutorial.onExecute(ptr_vehical);   break;
			 case KRAZY_VEHICAL_in_GAMEPLAY_Panguna      : _PangunaTutorial.onExecute(ptr_vehical);     break;
			 case KRAZY_VEHICAL_in_GAMEPLAY_Nanguna	     : _NangunaTutorial.onExecute(ptr_vehical);     break;
			 case KRAZY_VEHICAL_in_GAMEPLAY_Sherrif	     : _SherrifTutorial.onExecute(ptr_vehical ,  &ptr_diagnostic);       break; 
			}//end switch vehical type
			 break;
		case KRAZY_BOOL_False:
			switch(ptr_vehical->vehical_name()){
				case KRAZY_VEHICAL_in_GAMEPLAY_RangToona : _RangtoonaMoving.onExecute(ptr_vehical , &ptr_diagnostic);  break;
				case KRAZY_VEHICAL_in_GAMEPLAY_Ganungi	 : _GangiMoving.onExecute(ptr_vehical , &ptr_diagnostic);	   break;
				case KRAZY_VEHICAL_in_GAMEPLAY_BangToona : _BangtoonaMoving.onExecute(ptr_vehical);					   break;
				case KRAZY_VEHICAL_in_GAMEPLAY_Panguna   : _PangunaMoving.onExecute(ptr_vehical);					   break;
				case KRAZY_VEHICAL_in_GAMEPLAY_Nanguna	 : _NangunaMoving.onExecute(ptr_vehical);					   break;
				case KRAZY_VEHICAL_in_GAMEPLAY_Sherrif	 : _SherrifMoving.onExecute(ptr_vehical,&ptr_diagnostic);	   break; 
			}//end switch vehical type
			break;
		}
		//..,
#if 1
		switch(KrazyUNTanglers::_lost_track(ptr_vehical))
		{
		case KRAZY_BOOL_True :
			{
				switch(ptr_vehical->vehical_name())
				{
				case KRAZY_VEHICAL_in_GAMEPLAY_Sherrif  :  
					{
						char _msg[1024];
						KrazyViolatorRegister* _regs = ptr_vehical->get_violator_register();
						KrazyVehical* _violated_vehical = _regs->get_violatedVehical();		
						if(_violated_vehical){
							sprintf(_msg , "%s Busted !" , _violated_vehical->to_string().c_str()); 
						}
						else{
							sprintf(_msg , "%s Lost Track !" , ptr_vehical->to_string().c_str()); 
						}
						KrazyLevelThriver::get_default()->help_tip() = string(_msg); 
						KrazyLevelThriver::get_default()->level_mode() = KRAZY_LEVELMODE_gameover; 
					}
					break;
#if 0
				default : sprintf(_msg , "%s Lost Track !" , ptr_vehical->to_string().c_str());  break;
#endif
				}
				//..,
#if 0
				KrazyLevelThriver::get_default()->help_tip() = string(_msg); 
				KrazyLevelThriver::get_default()->level_mode() = KRAZY_LEVELMODE_gameover; 
#endif
			}
			break;
		}
#endif
		//..,
	}
 	return true;
}
//..,
