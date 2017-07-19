#pragma once
#include"KrazyPrimitiveDataCollections.h"
#include"KrazyTrafficIndicator.h"
#include"KrazyEnums.h"
#include"Vectors.h"
#include"KrazyVehicalShades.h"
//////////////////////////////////////////////
#define _KRAZY_VEHICAL_BUMPER_FACTOR    float(0.1f)//float(0.0001f)
#if 1
#define _KRAZY_STD_GAP					float( 0.15f)
#else
#define _KRAZY_STD_GAP					float( 0.25f)
#endif
/////////////////////////////////////////////
#define _KRAZY_TOONABROTHERS_TURN_LIMIT            1
#define _KRAZY_GANGUNI_GAMEPLAY_TURN_LIMIT		   2
#define _KRAZY_UNABROTHERS_GAMEPLAY_TURN_LIMIT     2
//
static const unsigned int _KRAZY_WAIT_TIME_ON_NOENTRY = 200;
/////////////////////////////////////////////
#if 0
static const float _top_left[]     = {	-0.020f ,  0.085f };
static const float _bottom_right[] = {	 0.040f , -0.085f };
#else
static const float _top_left[]     = {	-0.030f ,  0.075f };
static const float _bottom_right[] = {	 0.030f , -0.075f };
#endif
//
class KrazyLaneNode;
class KrazyTrafficIndicatorList;
enum KrazyVehicalType
{
  KRAZY_VEHICAL_TYPE_DEFAULT		= 0x00000000,
  KRAZY_VEHICAL_TYPE_NORMAL			= KRAZY_VEHICAL_TYPE_DEFAULT + 1,
  KRAZY_VEHICAL_TYPE_BLOCKING_CAR	= KRAZY_VEHICAL_TYPE_DEFAULT + 2,
  KRAZY_VEHICAL_TYPE_FAST_CAR		= KRAZY_VEHICAL_TYPE_NORMAL  + 3,
};
/////////////////////////////////////////////
enum KrazyVehicalName
{
	KRAZY_VEHICAL_in_GAMEPLAY_Nobody = 0x0,
	//Tuna Family
	KRAZY_VEHICAL_in_GAMEPLAY_RangToona,
	KRAZY_VEHICAL_in_GAMEPLAY_GangToona,
	KRAZY_VEHICAL_in_GAMEPLAY_BangToona, //AKA "Bangua"
	//Unga Family
	KRAZY_VEHICAL_in_GAMEPLAY_Panguna,
	KRAZY_VEHICAL_in_GAMEPLAY_Nanguna,
	KRAZY_VEHICAL_in_GAMEPLAY_Ganungi, //AKA Gangi
	//Piston Family..
	KRAZY_VEHICAL_in_GAMEPLAY_Pastunua, //AKA Piston
	KRAZY_VEHICAL_in_GAMEPLAY_Bhunsi,
	KRAZY_VEHICAL_in_GAMEPLAY_Bhanush, //AKA Tadank

	KRAZY_VEHICAL_in_GAMEPLAY_Sherrif,
};
////////////////////////////////////////////
enum  KrazyVehicalMoveCommand
{
	KRAZY_VEHICALMOVECMD_Default,
	KRAZY_VEHICALMOVECMD_MoveandRotate,
	KRAZY_VEHICALMOVECMD_Flip,
};
////////////////////////////////////
enum  KrazyVehicalMoveState
{
	KRAZY_VEHICAL_STATE_Default  ,
	KRAZY_VEHICAL_STATE_Entering ,
	KRAZY_VEHICAL_STATE_Moving   ,
};
//////////////////////////////////////////
enum KrazyVehicalGamePlayMovingState
{
	KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Default,
	KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_at_Block,
	KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_at_NoEntry,
	KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_after_Touch,
	KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Moving_after_Violation,
	KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Moving_Busted,
	KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Moving_after_Touch,
	KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_after_TurnsOver,
	KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_Before_Exit,
	KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_Cro_Exit,
	KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_Before_OnSpotFlip,
	KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_for_OnSpotFlip,
	KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Waiting_After_OnSpotFlip,

};
//////////////////////////////////////////
enum KrazySherrifGamePlayMovingState
{
	KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Default,
	KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Waiting,
	KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Chasing,
	KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Passed_in_OppositeLane,
	KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_ForceFlip,
	KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_DetectPath,
	KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_DetectSherrifBay,
	KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Going_to_SherrifBay,
	KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Move_to_SherrifBay,
	KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Move_to_NabViolator_in_OppositeLane,
	KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Move_to_Check_prev_SherrifBayDetect,
	KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Move_to_Check_prev_SherrifPathDetect,
}
;
//////////////////////////////////////////
enum KrazyVehicalWaitState
{
	KRAZY_VEHICAL_WAIT_STATE_Default          ,
	KRAZY_VEHICAL_WAIT_STATE_Turning		  ,
	KRAZY_VEHICAL_WAIT_STATE_Waiting_to_Turn  ,
	KRAZY_VEHICAL_WAIT_STATE_Turned           ,
	KRAZY_VEHICAL_WAIT_STATE_Queued           ,
}
;
enum KrazyVehicalScaleOrientation
{
	KrazyScale_Default = 0x0,
	KrazyScale_X        ,
	KrazyScale_Y        , 
};
enum KrazyVehicalFlipMode
{
	KRAZY_VEHICAL_FLIP_MODE_None		,
	KRAZY_VEHICAL_FLIP_MODE_Normal		,
	KRAZY_VEHICAL_FLIP_MODE_Spot		,
};
enum KrazyRouteMode
{
	KRAZY_ROUTEMODE_Default    ,
	KRAZY_ROUTEMODE_Normal     ,  //  just take the indicators in the map
	KRAZY_ROUTEMODE_ByVehical  ,  //  randomize by mutually excluding the traffic indicators
	KRAZY_ROUTEMODE_Gameplay   ,  //  game play route evolved for gangi , and una brothers.., or later may be bhunsi..,
	KRAZY_ROUTEMODE_Reckoning  ,  //  runing with calculated route either short or long


};
///////////////////////  ////////////////////////////////////////////
class KrazyDefaultAlgoMovingVehical_01Phase;
class KrazyVehicalMoveStatePolicy
{
public:
	KrazyVehicalMoveStatePolicy(unsigned short _state_id) :  _Id(0){}
	unsigned short get_Id() { return _Id; }
	KrazyBool execute(KrazyVehical* ptr_vehical){ return KRAZY_BOOL_False; }
private:
	unsigned short _Id;
};
//
class KrazyRouteThriver;
class KrazyVehicalList;
class KrazyViolatorRegister;
//
class KrazyVehical 
{
public :
	unsigned int&				Type();
	KrazyVehicalMoveState&      movestate();//change all  to lower later..,
	KrazyVehicalWaitState&      waitstate();
	KrazyBool&					visibilitystate();
	int							index();
	void                        increment_qcounter();
	void                        decrement_qcounter();
	string						to_string();
	float&						Angle();
	float&						Accelaration();
	float*                      Color();
	float&						gap();
	KrazyVehical*				PTRFrontVehical();
	KrazyVehical*				PTRRearVehical();
	glm::vec2*					Vector();
	glm::vec2*					VectorFrontBumper();
	glm::vec2*					VectorRearBumper();
	KrazyLaneNode*				PTRCurrentAssignedSingleLane();
	KrazyLaneNode*              previousassignedlane();
	int&						IndexInTheLaneList();
	KrazyBool&	                disablemove();
	void						on_setColor(KrazyFloatList& color);
	void						on_setIndex(int index);
	void						on_setNextTurnIndex(int index);
	int							getNextTurnIndex();
	KrazyTrafficIndicatorList*	get_TrafficIndicatorMap();
	KrazyTrafficIndicator*      get_NextTurn();
	KrazyVehical*               get_PreviousRearVehical();
	KrazyVehicalList*			get_QueuedPreviousVehicals();
	string                      get_IndexName();
	void						on_laneChange(KrazyTrafficIndicator* ptr_turn_indc);
	void                        on_attachFrontVehical(KrazyVehical* vehical);
	void                        on_detachFrontVehical();
	void                        on_attachRearVehical(KrazyVehical* vehical);
	void                        on_detachRearVehical();
	void						on_attachCurrentLane(KrazyLaneNode* node);	
	void						on_move();
	void						on_moveandrotate();
	void                        on_flip();
	void						on_skip_turn();
	void                        on_setPreviousRearVehical(KrazyVehical* ptr_vehical);
	void						on_setShade(int shader , int shader_type);
	void						on_setName(KrazyVehicalName vehical_name);
	void						on_setIndexAfterWaiting();
	void                        on_setIndexAfterTouch();
	void                        on_setSherrif(KrazyTrafficIndicator* bay);
	void                        on_setOnSpotFlipIndicator(KrazyTrafficIndicator* indicator);
	void						on_set_blocking_indicator(KrazyTrafficIndicator* _blocking);
	void						on_setDestinationLane(KrazyLaneNode* lane);
	void						swap_lights();
	KrazyBool					on_touch(float x , float y);
	KrazyBool					on_touch_reflection(float x , float y);
	void                        on_touched();
	void						on_set_exit(KrazyTrafficIndicator* _indicator);
	void						on_turning(KrazyTrafficIndicator* _indicator);
	void                        on_default();

	int&						wait();
	int&						wait_time_for_sherrif();

	unsigned int				gameplay_turn_limit();
	unsigned int&				gameplay_turns();
	unsigned int&               required_wait(); 
	
	KrazyBool&					was_in_sherrifBay();
	KrazyBool&					touch_detection_enbaled();
	KrazyBool&                  reflection_touch_detection_enabled();
	KrazyBool&					touched();
	KrazyBool&					reflection_touched();
	KrazyBool&                  reached_slider_point();
	KrazyBool&                  on_spot_flipped();
	KrazyBool&					sherrif_was_waiting_for_violator_in_front();
	KrazyBool&                  in_violation();
	KrazyBool&                  was_in_violation();
	KrazyBool&                  moving_after_touch();
	KrazyBool&                  add_to_dutyQ();
	
	KrazyTrafficIndicator*      get_sherrifBay();
	KrazyTrafficIndicator*		get_onSpotFlipIndicator();
	KrazyTrafficIndicator*		get_exit();
	
	KrazyVehicalShade*			shade();
	KrazyVehicalShaderType&     shadertype();
	KrazyVehicalShade* policelight_1();
	KrazyVehicalShade* policelight_2();
		
	KrazyVehicalName&           vehical_name();
	KrazyVehicalScaleOrientation& scaleorientation();
	KrazyVehicalGamePlayMovingState& gameplay_movingstate();
		
	KrazyVehicalFlipMode&       flip_mode();
	KrazySherrifGamePlayMovingState& sherrif_state();
	KrazyLaneNode* get_destinationLane();
		
	KrazyViolatorRegister*	    get_violator_register();
	KrazyTrafficIndicator*      get_blocking_indicator();
	KrazyRouteMode&             route_mode();

	void                        on_set_reflection();
	glm::vec2*					get_reflection();
	void				        on_set_route_thriver(KrazyRouteThriver* _route_thriver);
	KrazyRouteThriver*          get_route_thriver();

	//
protected:
	KrazyVehical() ;
	virtual ~KrazyVehical(void);
	void reset();
	void cleanUp();
	string                      _IndexName;
	KrazyVehicalList*           _PTRRearQueuedVehicals;            
	KrazyVehicalMoveState       _MoveState;
	KrazyVehicalWaitState       _WaitState;
	KrazyBool					_VisibilityState;
	KrazyBool					_DisableMove;
	unsigned int				_WaitQCounter;
	int							_Index;
	int                         _NextTurnIndex;
	unsigned int				_Type;
	float						_Angle;
	float						_Accelaration;
	float						_BumperFactor;
	float                       _Gap;
	KrazyTrafficIndicator*      _PTRNextTurn;
	KrazyTrafficIndicator*      _PTRSherrifBay;
	KrazyTrafficIndicator*      _PTRBlocking;
	KrazyTrafficIndicator*      _PTRExit;
	KrazyFloatList              _Color;
	KrazyVehical*				_PTRFrontVehical;
	KrazyVehical*				_PTRRearVehical;
	KrazyVehical*               _PTRPrevRearVehical;
	glm::vec2					_Vector;
	glm::vec2  					_FrontBumperVector;
	glm::vec2                   _RearBumperVector;
	KrazyLaneNode*				_PTRCurrentAssignedSingleLane;
	KrazyLaneNode*				_PTRPrevAssignedSingleLane;
	KrazyLaneNode*              _PTRDestinationSingleLane;
	KrazyTrafficIndicatorList   _TrafficIndicatorMap;
	int							_WaitPoliceLights;
	KrazyVehicalShade*			_PTRShade;
	KrazyVehicalShade*			_PTRPoliceLight_1;
	KrazyVehicalShade*			_PTRPoliceLight_2;
	KrazyVehicalShaderType		_ShaderType;
	KrazyVehicalScaleOrientation _ScaleOrientation;
	KrazyVehicalName					_VehicalName;
	KrazyVehicalGamePlayMovingState		_GamePlayMovingState;
	KrazyTrafficIndicator*              _PTROnSpotFlipDestination;
	string								_Vehical_z_Name;
	KrazyBool							_TouchDetectionEnabled;
	KrazyBool                           _ReflectionTouchDetectionEnabled;
	KrazyBool							_Touched;
	KrazyBool							_TouhedReflection;
	KrazyBool                           _Moving_after_Touch;
	KrazySherrifGamePlayMovingState		_SherrifState;
	KrazyBool							_Was_in_SherrifBay;
	KrazyBool                           _Sherrif_was_Waiting_for_Violator_in_Front;
	int									_Wait_time_for_sherrif;	
	KrazyVehicalFlipMode				_FlipMode;
	KrazyBool							_Reached_Slider_Point;
	KrazyViolatorRegister*              _ViolationRegister;
	KrazyRouteMode                      _RouteMode;
	unsigned int						_Gameplay_turn_Limit;
	unsigned int                        _Gameplay_Turns;
	unsigned int                        _Required_wait_at_NEntry;
	KrazyBool							_In_Violation;
	KrazyBool							_Was_in_Violation;
	glm::vec2							_ReflectionVec;
	KrazyRouteThriver*                  _RouteThriver;
	KrazyBool							_Add_to_DutyQ;
};
/////////////////////////////////////////////////////////////////
class KrazyViolatorRegister 
{
	//
public:
	KrazyViolatorRegister(KrazyVehical* sherrif) :
	  _sherrif(sherrif)
	{
	}
	//
	void setSherrif(KrazyVehical* sherrif)
	{
		if(_sherrif)
			return;

		_sherrif = sherrif;
	}
	//
	void write(KrazyVehical* _vehical)
	{
		//
		if(_violated_vehicals.size() > 0)
		{
			KrazyVehical* _violated_vehical = _violated_vehicals[0];
			if(_violated_vehical != _vehical){
				_violated_vehical->was_in_violation() = KRAZY_BOOL_True;
				_violated_vehical->in_violation() = KRAZY_BOOL_False;
			}
			else{
				_sherrif->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Waiting;
				return;
			}
		}
		_violated_vehicals.clear();
		_violated_vehicals.push_back(_vehical);
		_sherrif->sherrif_state() = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Waiting;
		//
	}
	//
	KrazyVehical* get_violatedVehical()
	{
		//
		if(_violated_vehicals.size() > 0){
			return _violated_vehicals[0];
		}
		//
		return 0;
	}
	//
	int get_violatedVehicals()
	{
		return _violated_vehicals.size();
	}
	//
	void on_clear()
	{
		_violated_vehicals.clear();
	}
	//
private:
	vector<KrazyVehical*>	 _violated_vehicals;
	KrazyVehical*			 _sherrif;
};
//
class KrazyVehicalUtility 
{
public:
	static string _convert_vehical_name(KrazyVehicalName _vehical_name);
};
