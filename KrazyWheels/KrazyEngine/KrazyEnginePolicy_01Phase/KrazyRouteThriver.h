#pragma once
#include "KrazyEnums.h"
#include "KrazyDutyKueue.h"
#include "KrazyTrafficIndicator.h"
#include "KrazyVehical.h"
//...,
enum KrazyRouteThriverState_UnaFamily
{
	KRAZY_ROUTETHRIVER_STATE_MovingNormal           , 
	KRAZY_ROUTETHRIVER_STATE_InSearchShortPath		, 
	KRAZY_ROUTETHRIVER_STATE_RunningWithShortPath   , 
	KRAZY_ROUTETHRIVER_STATE_Stop_Respective_Vehical_if_Found    , 
};
//..,
enum KrazyRouteThriverName
{
	KRAZY_ROUTE_THRIVER_None       ,
	KRAZY_ROUTE_THRIVER_UnaFamily  ,
};
//..,
class KrazyVehical;
class KrazyVehicalGameplayStateMovingPolicy_01Phase;
//..,
enum KrazyBrotherDutyType
{
	KRAZY_BROTHER_DUTY_Search_for_block   = 0x0001, 
	KRAZY_BROTHER_DUTY_Search_for_vehical = 0x0002,
};
//..,
template<typename _ARG_DUTY_DATA>
class KrazyUnaBrotherDuty : public KrazyDuty
{
public:
	KrazyUnaBrotherDuty(unsigned short _duty_id) : KrazyDuty(_duty_id)
	{
	}
	virtual ~KrazyUnaBrotherDuty(){}
	_ARG_DUTY_DATA data() { return _Data; }
	//..,
protected:
	_ARG_DUTY_DATA _Data;
};
//..,
class KrazyRouteThriver
{
	//..,
public:
	KrazyRouteThriver(KrazyRouteThriverName _name) :
    _Name(_name)
	{
	}
	//..
	virtual ~KrazyRouteThriver(){}
	KrazyRouteThriverName name() { return _Name; }
	//..,
protected:
	KrazyRouteThriverName _Name;
};
//..,
class KrazyUnaBrotherDuty_for_SearchingBlocks : public KrazyUnaBrotherDuty<KrazyTrafficIndicator*>
{
public:
	KrazyUnaBrotherDuty_for_SearchingBlocks(KrazyTrafficIndicator* _indicator) : 
	  KrazyUnaBrotherDuty(KRAZY_BROTHER_DUTY_Search_for_block) 
	{
		_Data = _indicator;
	}
	virtual  ~KrazyUnaBrotherDuty_for_SearchingBlocks(){}
};
//..,
class KrazyUnaBrotherDuty_for_StoppingVehical : public KrazyUnaBrotherDuty<KrazyVehical*>
{
public:
	KrazyUnaBrotherDuty_for_StoppingVehical(KrazyVehical* _vehical) :
	  KrazyUnaBrotherDuty(KRAZY_BROTHER_DUTY_Search_for_vehical) 
	  {
		  _Data = _vehical;
	  }

	  virtual ~KrazyUnaBrotherDuty_for_StoppingVehical(){}
};
//..,
class KrazyUnaBrotherRouteThriver : public KrazyRouteThriver
{
public:
	KrazyUnaBrotherRouteThriver(void);
	virtual ~KrazyUnaBrotherRouteThriver(void);
	//...,
	void on_attach_vehical(KrazyVehical* _vehical);
	//..,
	KrazyBool execute(KrazyVehicalGameplayStateMovingPolicy_01Phase* _policy);
	void reset();
	void reset_for_gameplay();
	//...,
	//event like;
	KrazyBool on_vehical_turned(KrazyVehicalGameplayStateMovingPolicy_01Phase* _policy);
	///..,
protected:
	void clean_dutylist();
	//..,
	KrazyBool onExecuteMovingNormal(KrazyVehicalGameplayStateMovingPolicy_01Phase* _policy);
	KrazyBool onExecuteInSearchShortPath(KrazyVehicalGameplayStateMovingPolicy_01Phase* _policy);
	KrazyBool onExecuteRunningWithShortPath(KrazyVehicalGameplayStateMovingPolicy_01Phase* _policy);
	KrazyBool onExecuteRunningStopRespectiveVehical(KrazyVehicalGameplayStateMovingPolicy_01Phase* _policy);
	//..,
	KrazyBool onExecuteSearch_for_Blocks(KrazyVehicalGameplayStateMovingPolicy_01Phase* _policy , KrazyUnaBrotherDuty_for_SearchingBlocks* _duty_for_search_block);
	KrazyBool short_route_required_for_block(KrazyLaneNode* _source_lane , KrazyLaneNode* _destination_lane , KrazyTrafficIndicator* _blocking);
protected:
	KrazyRouteThriverState_UnaFamily			_State;
	KrazyVehical*								_AssignedVehical;
	KrazyVehical*                               _RespectiveVehical_to_Stop;
	KrazyDutyList								_DutyList;
};
//
class KrazyRouteThriverFactory 
{
public:
	static KrazyRouteThriverFactory* getDefault();
	static void releaseDefault();
	KrazyRouteThriver* getNewRouteThriver(KrazyRouteThriverName _thriver_name);
	//..,
protected:
	KrazyRouteThriverFactory();
	~KrazyRouteThriverFactory();
	//..,
	vector<KrazyRouteThriver*>		   _RouteThrivers;
	static KrazyRouteThriverFactory*	_Me;
};


 