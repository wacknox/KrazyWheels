#include "StdAfx.h"
#include "KrazyGameRenderingPolicy_01Phase.h"
#include "KrazyLevelCityContextInterface.h"
#include "KrazyVehicalCache.h"
#include "KrazyDefaultAlgoMovingVehical_01Phase.h"
#include "KrazyLevelThriver.h"
#include "KrazyUtilityMacros.h"
//..
#define _DEDUCT_NEXT_POLICY() switch(_CurrentAlgoPolicy)\
			{	\
				case KRAZY_ALGO_POLICY_ORDER_01Phase_EIEX_MPLC: _CurrentAlgoPolicy = KRAZY_ALGO_POLICY_ORDER_01Phase_MPLC;break;\
			}\

KrazyGameRenderingPolicy_01Phase::KrazyGameRenderingPolicy_01Phase() :
_CurrentAlgoPolicy(KRAZY_ALGO_POLICY_ORDER_01Phase_EIEX_MPLC)
{
	//create_rendrer();		
}
//
KrazyGameRenderingPolicy_01Phase::~KrazyGameRenderingPolicy_01Phase(void)
{
	
}
//
void KrazyGameRenderingPolicy_01Phase::reset()
{
	_CurrentAlgoPolicy = KRAZY_ALGO_POLICY_ORDER_01Phase_EIEX_MPLC;
	_AlgoMovingPlayer.reset();
}
//
void KrazyGameRenderingPolicy_01Phase::reset_rendrer()
{
	_VehicalRendrer.reset();
}
//
void KrazyGameRenderingPolicy_01Phase::create_rendrer()
{
	_VehicalRendrer.create();
}
//
void KrazyGameRenderingPolicy_01Phase::on_resize(int width , int height)
{
	_VehicalRendrer.on_resize(width , height);
}
//
void KrazyGameRenderingPolicy_01Phase::render_vehical()
{
	vector<KrazyVehical*>* ptr_vehical_list = KrazyVehicalCache::getCache()->get_VehicalsInTheNetwork();
	int count = ptr_vehical_list->size();
	for(int i = 0; i < count; i++){
		KrazyVehical* ptr_vehical = ptr_vehical_list->at(i);
		switch(ptr_vehical->disablemove()){
			case KRAZY_BOOL_True:continue;
		}
		_VehicalRendrer.render(ptr_vehical);
	}
}
//
bool KrazyGameRenderingPolicy_01Phase::onExecute()
{
	bool deduct(true);// convert to KrazyBool;..,
	switch(_CurrentAlgoPolicy)
	{
	case KRAZY_ALGO_POLICY_ORDER_01Phase_MPLC  :_AlgoMovingPlayer.onExecute();break;
	case KRAZY_ALGO_POLICY_ORDER_01Phase_EIEX_MPLC:
		deduct = _AlgoEntryIndicatorExtraction.onExecute();
		_AlgoMovingPlayer.onExecute();
		break;
	}
	//
#if 1
	render_vehical();
#else
	vector<KrazyVehical*>* ptr_vehical_list = KrazyVehicalCache::getCache()->get_VehicalsInTheNetwork();
	int count = ptr_vehical_list->size();
	for(int i = 0; i < count; i++){
		KrazyVehical* ptr_vehical = ptr_vehical_list->at(i);
		ptr_vehical->Vector()->x = -0.97f + 0.005f;
		ptr_vehical->Vector()->y = 0.5f;
		ptr_vehical->Angle() = 0.0f;
		ptr_vehical->scaleorientation() = KrazyScale_X;
		_VehicalRendrer.render(ptr_vehical);
	}
#endif
	if(deduct){
		_DEDUCT_NEXT_POLICY();
	}
	return false;
}
//..,
KrazyShadedVehicalRendrer* KrazyGameRenderingPolicy_01Phase::get_vehical_rendrer()
{
	return &_VehicalRendrer;
}

