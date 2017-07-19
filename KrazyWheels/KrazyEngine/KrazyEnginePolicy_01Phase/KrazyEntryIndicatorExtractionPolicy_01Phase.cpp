#include "StdAfx.h"
#include "KrazyLevelCityNetwork.h"
#include "KrazyEntryIndicatorCache.h"
#include "KrazyVehicalCache.h"
#include "KrazyLevelThriver.h"
#include "KrazyTutorialThriver.h"
#ifndef _KrazyTutorials
#define _KrazyTutorials
#endif
#define _WAITING 1
//
KrazyEntryIndicatorExtractionPolicy_01Phase::KrazyEntryIndicatorExtractionPolicy_01Phase() :
_Wait(0) //removal .., 
,_LastIndex(0)
,_PTRPrev(0)
,_Counter(0)
{
}
//
KrazyEntryIndicatorExtractionPolicy_01Phase::~KrazyEntryIndicatorExtractionPolicy_01Phase(void)
{
}
//
bool KrazyEntryIndicatorExtractionPolicy_01Phase::onExecute()
{
#if 1
	KrazyTrafficIndicator* _exit = KrazyLevelThriver::get_default()->get_exit();
	vector<KrazyTrafficIndicator*>* _entry_indicator = getEntryIndicators();
	int count = _entry_indicator->size();
	for(int i = 0; i < count; i++){
		KrazyTrafficIndicator* ptr_entry_indc = _entry_indicator->at(i);
		KrazyEntranceContext*  ptr_entry_ctxt =  ptr_entry_indc->get_EntranceContext();
		KrazyIntList*		   ptr_name_list  =  ptr_entry_ctxt->getNameList();
		int count_type_list  = ptr_name_list->size();
		for(int ctxt = 0; ctxt < count_type_list;ctxt++){
				KrazyVehical*  ptr_new_vehical = KrazyVehicalCache::allocate();
				ptr_new_vehical->on_setIndex(_Counter++);
				*ptr_new_vehical->Vector() = *ptr_entry_indc->Vector();
				ptr_new_vehical->on_set_exit(_exit);
				ptr_new_vehical->route_mode() = KRAZY_ROUTEMODE_ByVehical;
				ptr_new_vehical->on_setName(KrazyVehicalName(ptr_name_list->at(ctxt)));
				ptr_new_vehical->Accelaration()  = (ptr_new_vehical->Type() == 1) ? 0.00500f/*0.00030f*/ : 0.00080f;
				ptr_new_vehical->movestate() = KRAZY_VEHICAL_STATE_Entering;
				ptr_new_vehical->on_laneChange(ptr_entry_indc);
			}
	}
	//Adding/Translating Sherrif Car if Any in to the game;
	KrazyNetworkCity_01Phase* ptr_parent_network = KrazyNetworkCity_01Phase::getNetwork();
	vector<KrazyWheelNetwork_01Phase*>* ptr_network_nodes = ptr_parent_network->getNodeCache();
	int nodes = ptr_network_nodes->size();
	for(int node = 0; node < nodes; node++)
	{
		KrazyLaneNode* _lane = ((KrazyNetworkCity_01Phase*)ptr_network_nodes->at(node))->getOwner();
		int Kids = 2;
		for(int kid = 0; kid < Kids; kid++)
		{
			KrazyLaneNode* _kid_lane = _lane->getLane(kid);
			KrazyTrafficIndicator* _sherrif_bay = _kid_lane->get_sherrifBay();
			if(_sherrif_bay)
			{
				KrazyVehical*  ptr_new_vehical = KrazyVehicalCache::allocate();
				ptr_new_vehical->on_setIndex(_Counter++);
				*ptr_new_vehical->Vector() = *_sherrif_bay->Vector();
				ptr_new_vehical->Accelaration()  = 0.00500f;
				ptr_new_vehical->on_setSherrif(_sherrif_bay);
				ptr_new_vehical->was_in_sherrifBay() = KRAZY_BOOL_True;
				ptr_new_vehical->visibilitystate() = KRAZY_BOOL_True;
				ptr_new_vehical->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Waiting;
				ptr_new_vehical->disablemove() = KRAZY_BOOL_False;
				KrazyIntList* _lane_index_list = _sherrif_bay->get_assignedLanes_for_Sherrif();
				int lane_indices = _lane_index_list->size();
				for(int lane_index = 0; lane_index < lane_indices; lane_index += 3)
				{
					KrazyLaneNode* _lane = ((KrazyNetworkCity_01Phase*)ptr_network_nodes->at(_lane_index_list->at(lane_index)))->getOwner();
					int _index = _lane_index_list->at(lane_index + 1);
					if(_index > -1 && _index < 2)
					{
						_lane->getLane(_index)->on_assign_sherrif(ptr_new_vehical);
					}
					_index = _lane_index_list->at(lane_index + 2);
					if(_index > -1 && _index < 2)
					{
						_lane->getLane(_index)->on_assign_sherrif(ptr_new_vehical);
					}
					
				}
				
			}
		}

	}
#ifdef _KrazyTutorials
	KrazyTutorialThriver::get_default()->on_set_level();
#endif
#else
	int count = _PTREntryIndicators->size();
	_PTRPrev = 0;
	for(int i = 0; i < count; i++){
		KrazyTrafficIndicator* ptr_entry_indc = _PTREntryIndicators->at(_LastIndex++);
		KrazyEntranceContext*  ptr_entry_ctxt =  ptr_entry_indc->get_EntranceContext();
		KrazyIntList*          ptr_shade_list =  ptr_entry_ctxt->getShadeList();  
		KrazyIntList*          ptr_type_list  =  ptr_entry_ctxt->getTypeList();
		KrazyIntList*		   ptr_name_list  =  ptr_entry_ctxt->getNameList();
		KrazyIntList*          ptr_shadetype_list = ptr_entry_ctxt->getShadeTypeList();
		int count_type_list  = ptr_type_list->size();
		//int count_shade_list = ptr_shade_list->size();

		//if(count_type_list > 0 && count_shade_list > 0 && count_type_list == count_shade_list){
			for(int ctxt = 0; ctxt < count_type_list;ctxt++){
				KrazyVehical*  ptr_new_vehical = KrazyVehicalCache::allocate();
				ptr_new_vehical->on_setIndex(_Counter++);
				ptr_new_vehical->Type()    =  ptr_type_list->at(ctxt);
				*ptr_new_vehical->Vector() = *ptr_entry_indc->Vector();
				ptr_new_vehical->Accelaration()  = (ptr_new_vehical->Type() == 1) ? 0.00030f/*0.00030f*/ : 0.00080f;
				ptr_new_vehical->on_setShade(ptr_shade_list->at(ctxt) , ptr_shadetype_list->at(ctxt));
			//	ptr_new_vehical->vehical_name() = KrazyVehicalName(ptr_name_list->at(ctxt));
				ptr_new_vehical->movestate() = KRAZY_VEHICAL_STATE_Entering;
				ptr_new_vehical->on_laneChange(ptr_entry_indc);
			}
		//}
	}
#endif
	return true;
}
//..
vector<KrazyTrafficIndicator*>* KrazyEntryIndicatorExtractionPolicy_01Phase::getEntryIndicators()
{
	KrazyEntryIndicatorCache* ptr_entry_cache = KrazyEntryIndicatorCache::getCache();
	return ptr_entry_cache->get_EntryIndicators();
}

