#include "StdAfx.h"
#include "KrazyUNTanglers.h"
#include "KrazyLaneNode.h"
//
void KrazyUNTanglers::_check_queued_vehical(KrazyVehical* _vehical)
{
	switch(_vehical->waitstate())
	{
	case KRAZY_VEHICAL_WAIT_STATE_Turning		   : 
		{
			if( _vehical->get_QueuedPreviousVehicals()->size() > 0 )
			{
				int i = 0;
			}
			
		}
		break;
	case KRAZY_VEHICAL_WAIT_STATE_Waiting_to_Turn  :
		{
			KrazyVehicalList* _prev_Q = _vehical->get_QueuedPreviousVehicals();
			int queue_ = _prev_Q->size();
			for(int i = 0; i < queue_; i++){
				KrazyVehical* q_vehical = _prev_Q->at(i);
				switch(q_vehical->waitstate())
				{
				case KRAZY_VEHICAL_WAIT_STATE_Queued : q_vehical->decrement_qcounter();break;
				default:
					{
						int i = 0;
					}
					break;
				}
			}
			_prev_Q->clear();
		}
		break;
	case KRAZY_VEHICAL_WAIT_STATE_Turned           : 	_unqueue_prev_vehical_after_turn(_vehical);     break;
	case KRAZY_VEHICAL_WAIT_STATE_Queued           : 
		{
			KrazyVehical* _aux_vehical  = _vehical->PTRFrontVehical();
			KrazyVehicalList _aux_queued_vehical;
			KrazyBool _going_front = KRAZY_BOOL_True;
			for(;_aux_vehical;){
				switch(_going_front)
				{
				case KRAZY_BOOL_True :
					 switch(_aux_vehical->waitstate()){
						case KRAZY_VEHICAL_WAIT_STATE_Queued  : _aux_queued_vehical.push_back(_aux_vehical); break;
						default:
							{
								KrazyVehicalList* _prev_Q = _aux_vehical->get_QueuedPreviousVehicals();
								switch(_prev_Q->size()){
								case 0:
									{
										int i = 0;
									}
									break;
								default:
									{
										_prev_Q->clear();
										int queue_ = _aux_queued_vehical.size();
										for(int i = 0; i < queue_; i++){
											_prev_Q->push_back(_aux_queued_vehical[i]);
										}
										_going_front  = KRAZY_BOOL_False;
										_aux_vehical  = _vehical->PTRRearVehical();
										_aux_queued_vehical.clear();
										continue;
									}
									break;
								}
							}
							break;
					}
					_aux_vehical  = _aux_vehical->PTRFrontVehical();
					break;
				case KRAZY_BOOL_False:
					 switch(_aux_vehical->waitstate()){
						case KRAZY_VEHICAL_WAIT_STATE_Queued  :
							 _aux_queued_vehical.push_back(_aux_vehical); 
							 _aux_vehical  = _aux_vehical->PTRRearVehical();
							break;
							//.............
						default : _aux_vehical = 0; break;
					 }
					 if(!_aux_vehical){
						 KrazyVehicalList* _prev_Q = _vehical->get_QueuedPreviousVehicals();
						 _prev_Q->clear();
						 int queue_ = _aux_queued_vehical.size();
						 for(int i = 0; i < queue_; i++){
							 _prev_Q->push_back(_aux_queued_vehical[i]);
						 }
					 }
					 break;
				}
				
			}
		
		}
		break;
	}
}
//
void KrazyUNTanglers::_unqueue_prev_vehical_after_turn(KrazyVehical* _vehical)
{
#if 0
	KrazyVehicalList* ptr_prev_Q = _vehical->get_QueuedPreviousVehicals();
	int count = ptr_prev_Q->size();
	for(int i = 0; i < count; i++){
		KrazyVehical* ptr_prev = ptr_prev_Q->at(i);
		switch(ptr_prev->waitstate()){
		case KRAZY_VEHICAL_WAIT_STATE_Queued:ptr_prev->decrement_qcounter();break;
		default:
			{
				int i = 0;
			}
			break;
		}
	}
	ptr_prev_Q->clear();
#endif
	//..
	_vehical->on_default();
}
//..,
KrazyBool KrazyUNTanglers::_lost_track(KrazyVehical* _vehical)
{
	KrazyLaneNode* _source_lane = _vehical->PTRCurrentAssignedSingleLane();
	if(!_source_lane)
		return KRAZY_BOOL_False;
	//..,
	glm::vec2* _vec_flip = _source_lane->getFlipTrafficIndicator()->Vector();
	glm::vec2* _vec_front_bumper = _vehical->VectorFrontBumper();
	switch(_source_lane->getProperties()->get_LaneDirection())
	{
	case KRAZY_LANE_DIRECTION_UP_HORZS   :
		if(_vec_front_bumper->y != _vec_flip->y || _vec_front_bumper->x < _vec_flip->x)
		{
			return KRAZY_BOOL_True;	
		}
		break;
	case KRAZY_LANE_DIRECTION_DOWN_HORZS :
		if(_vec_front_bumper->y != _vec_flip->y || _vec_front_bumper->x > _vec_flip->x)
		{
			return KRAZY_BOOL_True;	
		}
		break;
	case KRAZY_LANE_DIRECTION_UP_VERTS   :
		if(_vec_front_bumper->x != _vec_flip->x || _vec_front_bumper->y > _vec_flip->y)
		{
			return KRAZY_BOOL_True;	
		}
		break;
	case KRAZY_LANE_DIRECTION_DOWN_VERTS :
		if(_vec_front_bumper->x != _vec_flip->x  || _vec_front_bumper->y < _vec_flip->y)
		{
			return KRAZY_BOOL_True;	
		}
		break;
	}
	//..,
	return KRAZY_BOOL_False;	

}
