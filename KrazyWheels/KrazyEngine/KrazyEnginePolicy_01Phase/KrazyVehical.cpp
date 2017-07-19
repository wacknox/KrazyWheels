#include "StdAfx.h"
#include "KrazyVehical.h"
#include "KrazyLaneNode.h"
#include "KrazyViewPortContext.h"
#include "KrazyRouteThriver.h"
//
KrazyVehical::KrazyVehical() :
/*
	only intialized throughout the cache session never reset to null
	deleted by the respective KrazyLaneNode by which it was created..,
*/
_PTROnSpotFlipDestination(0) 
,_ViolationRegister(0)
{
	_PTRRearQueuedVehicals = new KrazyVehicalList;
	reset();
}
	
KrazyVehical::~KrazyVehical(void)
{
	cleanUp();
}
/////////////////////////////////////////// Properties ///////////////////////////////////////////////////////
unsigned int&				KrazyVehical::Type()								{return _Type;}
KrazyVehicalMoveState&      KrazyVehical::movestate()							{return _MoveState;}
KrazyVehicalWaitState&      KrazyVehical::waitstate()							{return _WaitState;}
KrazyBool&					KrazyVehical::visibilitystate()						{return _VisibilityState;}
int							KrazyVehical::index()								{return _Index;}
float&						KrazyVehical::Angle()								{return _Angle;}
float&						KrazyVehical::Accelaration()						{return _Accelaration;}
KrazyVehical*				KrazyVehical::PTRFrontVehical()						{return _PTRFrontVehical;}
KrazyVehical*				KrazyVehical::PTRRearVehical()						{return _PTRRearVehical;}
glm::vec2*					KrazyVehical::Vector()								{return &_Vector;}
glm::vec2*					KrazyVehical::VectorFrontBumper()                   {return &_FrontBumperVector;}
glm::vec2*					KrazyVehical::VectorRearBumper()                    {return & _RearBumperVector;}
KrazyLaneNode*				KrazyVehical::PTRCurrentAssignedSingleLane()		{return _PTRCurrentAssignedSingleLane;}
KrazyLaneNode*              KrazyVehical::previousassignedlane()                {return _PTRPrevAssignedSingleLane;}
KrazyBool&					KrazyVehical::disablemove()							{return _DisableMove;}
float*                      KrazyVehical::Color()								{return &_Color[0];}
int&						KrazyVehical::wait()								{return _WaitPoliceLights;}
KrazyVehicalShade*			KrazyVehical::shade()								{return _PTRShade;}
KrazyVehicalShaderType&     KrazyVehical::shadertype()							{return _ShaderType;}
KrazyVehicalShade*			KrazyVehical::policelight_1()						{return _PTRPoliceLight_1; }
KrazyVehicalShade*			KrazyVehical::policelight_2()						{return _PTRPoliceLight_2; }
KrazyVehicalScaleOrientation& KrazyVehical::scaleorientation()					{return _ScaleOrientation; }
KrazyVehicalGamePlayMovingState& KrazyVehical::gameplay_movingstate()			{return _GamePlayMovingState; }
KrazyBool&					KrazyVehical::touch_detection_enbaled()				{return _TouchDetectionEnabled; }
KrazyBool&                  KrazyVehical::reflection_touch_detection_enabled()  {return _ReflectionTouchDetectionEnabled;}
KrazyBool&					KrazyVehical::touched()								{return _Touched;}
KrazyBool&					KrazyVehical::reflection_touched()					{return _TouhedReflection;}
KrazyViolatorRegister*		KrazyVehical::get_violator_register()				{return _ViolationRegister;}
float&						KrazyVehical::gap()									{return _Gap;}
KrazySherrifGamePlayMovingState& KrazyVehical::sherrif_state()					{return _SherrifState;}
KrazyBool&					KrazyVehical::was_in_sherrifBay()				    {return _Was_in_SherrifBay;}
KrazyBool&					KrazyVehical::sherrif_was_waiting_for_violator_in_front() {return _Sherrif_was_Waiting_for_Violator_in_Front;}
int&						KrazyVehical::wait_time_for_sherrif()				{return _Wait_time_for_sherrif;}
KrazyVehicalFlipMode&       KrazyVehical::flip_mode()							{return _FlipMode;}
KrazyBool&                  KrazyVehical::reached_slider_point()				{return _Reached_Slider_Point;}
KrazyRouteMode&             KrazyVehical::route_mode()							{return _RouteMode;}
unsigned int				KrazyVehical::gameplay_turn_limit()					{return _Gameplay_turn_Limit;}
unsigned int&				KrazyVehical::gameplay_turns()						{return _Gameplay_Turns;}
unsigned int&               KrazyVehical::required_wait()						{return _Required_wait_at_NEntry;}
KrazyBool&                  KrazyVehical::in_violation()						{return _In_Violation;}
KrazyBool&                  KrazyVehical::was_in_violation()					{return _Was_in_Violation;}
KrazyBool&                  KrazyVehical::moving_after_touch()					{return _Moving_after_Touch;}
KrazyBool&                  KrazyVehical::add_to_dutyQ()						{return _Add_to_DutyQ;}
//////////////////////////////////////////// Metohods /////////////////////////////////////////////////////////////////////
KrazyTrafficIndicatorList*	KrazyVehical::get_TrafficIndicatorMap()
{
	if(!_PTRCurrentAssignedSingleLane)
		return  0;
	
	return &_TrafficIndicatorMap;
}
/////
void KrazyVehical::on_setColor(KrazyFloatList& color)
{
	int count = color.size();
	for(int i = 0; count == 4 && i < count ; i++){
		_Color.push_back(color[i]);
	}
}
//
void KrazyVehical::on_attachFrontVehical(KrazyVehical* vehical)
{
	if(vehical && vehical == _PTRRearVehical){
		_PTRRearVehical = 0;
		int a = 0;
	}
	_PTRFrontVehical = vehical;		
}
//
void KrazyVehical::on_detachFrontVehical()
{
	_PTRFrontVehical = 0;
}
//
void KrazyVehical::on_attachRearVehical(KrazyVehical* vehical)
{
	if(vehical && vehical == _PTRFrontVehical){
		_PTRFrontVehical = 0;
		int a = 0;
	}
	_PTRRearVehical = vehical;
}
//
void KrazyVehical::on_detachRearVehical()
{
	_PTRRearVehical = 0;
}
//
void KrazyVehical::on_attachCurrentLane(KrazyLaneNode* node)
{
	if(!node)
		return;

	_PTRPrevAssignedSingleLane      = _PTRCurrentAssignedSingleLane;
	_PTRCurrentAssignedSingleLane   = node;
	KrazyLaneProperties* properties = _PTRCurrentAssignedSingleLane->getProperties();
	switch(properties->get_LaneDirection()){
	case KRAZY_LANE_DIRECTION_UP_HORZS       :
		_FrontBumperVector[0] = (_Vector.x  - _BumperFactor); _FrontBumperVector[1] = _Vector.y; 
		_RearBumperVector[0] = ( _Vector.x  + _BumperFactor); _RearBumperVector[1] =  _Vector.y;
		break;
	case KRAZY_LANE_DIRECTION_DOWN_HORZS     :
		_FrontBumperVector[0] = (_Vector.x  + _BumperFactor); _FrontBumperVector[1] = _Vector.y; 
		_RearBumperVector[0] = ( _Vector.x  - _BumperFactor); _RearBumperVector[1] = _Vector.y;
		break;
	case KRAZY_LANE_DIRECTION_UP_VERTS       :
		_FrontBumperVector[0] = _Vector.x ; _FrontBumperVector[1] = (_Vector.y + _BumperFactor); 
		_RearBumperVector[0] =  _Vector.x ;  _RearBumperVector[1] = (_Vector.y - _BumperFactor);
		break;
	case KRAZY_LANE_DIRECTION_DOWN_VERTS     : 
		_FrontBumperVector[0] = _Vector.x  ; _FrontBumperVector[1] = (_Vector.y - _BumperFactor); 
		_RearBumperVector[0] =  _Vector.x  ; _RearBumperVector[1] = (_Vector.y + _BumperFactor);
		break;
	}

}
//
void KrazyVehical::reset()
{
	// To Do Check all reset where ever it is ;

	_Type							= KRAZY_VEHICAL_TYPE_DEFAULT;
	_Angle							= 0;
	_Accelaration					= 0;
	_BumperFactor			        = _KRAZY_VEHICAL_BUMPER_FACTOR;
	_PTRFrontVehical				= 0;
	_PTRRearVehical					= 0;
	_PTRPrevRearVehical             = 0;
	_Vector							= glm::vec2(0.0f,0.0f);
	_PTRCurrentAssignedSingleLane	= 0;
	_PTRPrevAssignedSingleLane		= 0;
	_DisableMove                    = KRAZY_BOOL_True;
	_PTRNextTurn                    = 0;  
	_NextTurnIndex                  = 0;
	_MoveState						= KRAZY_VEHICAL_STATE_Default;
	_WaitState                      = KRAZY_VEHICAL_WAIT_STATE_Default;
	_Index                          = -1;
	_WaitQCounter                   = 0;
	_VisibilityState                = KRAZY_BOOL_False;
	_WaitPoliceLights				= 0;
	_PTRShade						= 0;
	_PTRPoliceLight_1				=  KrazyVehicalShadeFactory::getDefault()->getShade(KrazyShade_Red);
	_PTRPoliceLight_2				=  KrazyVehicalShadeFactory::getDefault()->getShade(KrazyShade_Blue);
	_ScaleOrientation				= KrazyScale_Default;
	_VehicalName                    = KRAZY_VEHICAL_in_GAMEPLAY_Nobody;
	_GamePlayMovingState            = KRAZY_VEHICAL_GAMEPLAY_MOVINGSTATE_Default;
	_PTRSherrifBay					= 0;
	_TouchDetectionEnabled			 = KRAZY_BOOL_False;
	_ReflectionTouchDetectionEnabled = KRAZY_BOOL_False;
	_Touched						= KRAZY_BOOL_False;
	_TouhedReflection               = KRAZY_BOOL_False;
	_Gap							= _KRAZY_STD_GAP;
	_SherrifState                   = KRAZY_SHERRIF_GAMEPLAY_MOVINGSTATE_in_Default;
	_PTRDestinationSingleLane       = 0;
	_Was_in_SherrifBay              = KRAZY_BOOL_False;
	_Sherrif_was_Waiting_for_Violator_in_Front = KRAZY_BOOL_False;
	_Wait_time_for_sherrif			= 0;
	_Reached_Slider_Point			= KRAZY_BOOL_False;
	_FlipMode						= KRAZY_VEHICAL_FLIP_MODE_None;
	_RouteMode						= KRAZY_ROUTEMODE_Default;
	_PTRBlocking					= 0;
	_Gameplay_turn_Limit			= 0;
	_Gameplay_Turns					= 0;
	_Required_wait_at_NEntry        = _KRAZY_WAIT_TIME_ON_NOENTRY;
	_PTRExit					    = 0;
	_In_Violation                   = KRAZY_BOOL_False;
	_Moving_after_Touch             = KRAZY_BOOL_False;
	_Was_in_Violation               = KRAZY_BOOL_False;
	_RouteThriver                   = 0;
	_Add_to_DutyQ					= KRAZY_BOOL_False;
	_Vehical_z_Name.clear();
	_Color.clear();
	_PTRRearQueuedVehicals->clear();
		
}
//
void KrazyVehical::cleanUp()
{
	if(_PTRRearQueuedVehicals){
		_PTRRearQueuedVehicals->clear();
		delete _PTRRearQueuedVehicals;_PTRRearQueuedVehicals = 0;
	}
	//
	if(_ViolationRegister){
		delete _ViolationRegister;_ViolationRegister = 0;
	}
}
//
void KrazyVehical::on_move()
{
	if(_DisableMove)
		return;

	if(!_PTRCurrentAssignedSingleLane)
		return;

	KrazyLaneProperties* properties = _PTRCurrentAssignedSingleLane->getProperties();
	switch(properties->get_LaneDirection()){
		case KRAZY_LANE_DIRECTION_UP_HORZS     :
			_Vector.x  -=  _Accelaration;
			_FrontBumperVector[0] = (_Vector.x  - _BumperFactor) ; _FrontBumperVector[1] = _Vector.y; 
			_RearBumperVector[0] =  (_Vector.x  + _BumperFactor) ; _RearBumperVector[1]  = _Vector.y;
			break;
		case KRAZY_LANE_DIRECTION_DOWN_HORZS   :
			_Vector.x  +=  _Accelaration;
			_FrontBumperVector[0] = (_Vector.x  + _BumperFactor) ; _FrontBumperVector[1] = _Vector.y; 
			_RearBumperVector[0]  = ( _Vector.x  - _BumperFactor) ; _RearBumperVector[1] = _Vector.y;
			break;
		case KRAZY_LANE_DIRECTION_UP_VERTS     :
			_Vector.y  +=  _Accelaration;
			_FrontBumperVector[0] = _Vector.x  ; _FrontBumperVector[1] = (_Vector.y + _BumperFactor); 
			_RearBumperVector[0] =  _Vector.x  ; _RearBumperVector[1]  = (_Vector.y - _BumperFactor);
			break; 
		case KRAZY_LANE_DIRECTION_DOWN_VERTS   :
			_Vector.y  -=  _Accelaration;
			_FrontBumperVector[0] = _Vector.x  ; _FrontBumperVector[1] = (_Vector.y - _BumperFactor); 
			_RearBumperVector[0]  =  _Vector.x ; _RearBumperVector[1]  = (_Vector.y + _BumperFactor);
			break; 
	}

}
//
void KrazyVehical::on_moveandrotate()
{
	if(!_PTRCurrentAssignedSingleLane)
		return;

	KrazyLaneDirection   curr_properties = _PTRCurrentAssignedSingleLane->getProperties()->get_LaneDirection();
	if(_PTRPrevAssignedSingleLane)
	{
		KrazyLaneDirection   prev_properties = _PTRPrevAssignedSingleLane->getProperties()->get_LaneDirection();
		switch(prev_properties)
		{
		case KRAZY_LANE_DIRECTION_UP_HORZS     : 
		case KRAZY_LANE_DIRECTION_DOWN_HORZS   : 
			switch(curr_properties)
			{
			case KRAZY_LANE_DIRECTION_UP_VERTS     : _Angle = 180.0f;_ScaleOrientation = KrazyScale_X;break;
			case KRAZY_LANE_DIRECTION_DOWN_VERTS   : _Angle = 360.0f;_ScaleOrientation = KrazyScale_X;break;
			}
			break;
		case KRAZY_LANE_DIRECTION_UP_VERTS     :
		case KRAZY_LANE_DIRECTION_DOWN_VERTS   :
			switch(curr_properties)
			{
			case KRAZY_LANE_DIRECTION_UP_HORZS     : _Angle = 270.0f;_ScaleOrientation = KrazyScale_Y;break;
			case KRAZY_LANE_DIRECTION_DOWN_HORZS   : _Angle = 90.0f; _ScaleOrientation = KrazyScale_Y;break;
			}
			break; 
		}
	}
	else{
		switch(curr_properties)
		{
				case KRAZY_LANE_DIRECTION_UP_HORZS     : _Angle = 270.0f;_ScaleOrientation = KrazyScale_Y;break;
				case KRAZY_LANE_DIRECTION_DOWN_HORZS   : _Angle = 90.0f; _ScaleOrientation = KrazyScale_Y;break;
				case KRAZY_LANE_DIRECTION_UP_VERTS     : _Angle = 180.0f;_ScaleOrientation = KrazyScale_X;break;
				case KRAZY_LANE_DIRECTION_DOWN_VERTS   : _Angle = 360.0f;_ScaleOrientation = KrazyScale_X;break;
		}
	}
	on_move();
}
//
void KrazyVehical::on_flip()
{
	KrazyLaneDirection   curr_properties = _PTRCurrentAssignedSingleLane->getProperties()->get_LaneDirection();
	switch(curr_properties)
	{
	case KRAZY_LANE_DIRECTION_UP_HORZS     : _Angle = 270.0f;_ScaleOrientation = KrazyScale_Y;break;
	case KRAZY_LANE_DIRECTION_DOWN_HORZS   : _Angle = 90.0f;_ScaleOrientation = KrazyScale_Y;break;
	case KRAZY_LANE_DIRECTION_UP_VERTS     : _Angle = 180.0f;_ScaleOrientation = KrazyScale_X;break;
	case KRAZY_LANE_DIRECTION_DOWN_VERTS   : _Angle = 360.0f;_ScaleOrientation = KrazyScale_X;break;
	}
	on_move();
}
//
void KrazyVehical::on_skip_turn()
{
	KrazyLaneDirection   curr_properties = _PTRCurrentAssignedSingleLane->getProperties()->get_LaneDirection();
	switch(curr_properties)
	{
	case KRAZY_LANE_DIRECTION_UP_HORZS     : _Vector.x -= 0.2f;break;
	case KRAZY_LANE_DIRECTION_DOWN_HORZS   : _Vector.x += 0.2f;break;
	case KRAZY_LANE_DIRECTION_UP_VERTS     : _Vector.y += 0.2f;break;
	case KRAZY_LANE_DIRECTION_DOWN_VERTS   : _Vector.y -= 0.2f;break;
	}
	on_move();

}
//
#define ON_SWITCH_TOUCH() \
	switch(_Touched) \
	{ \
	case KRAZY_BOOL_True :_Touched = KRAZY_BOOL_False;break; \
	case KRAZY_BOOL_False:_Touched = KRAZY_BOOL_True;break; \
	}\

KrazyBool KrazyVehical::on_touch(float x , float y)
{
	if(!_PTRCurrentAssignedSingleLane)
		return KRAZY_BOOL_False;

	glm::vec2 top_left(_top_left[0] , _top_left[1]);
	glm::vec2 bottom_right(_bottom_right[0] , _bottom_right[1]);
	float width  = float(KrazyViewPortContext::_Width);
	float height = float(KrazyViewPortContext::_Height);
	float aspect = width/height;
	float left   = -1.0f * aspect;
	float right  =  1.0f * aspect;
	float bottom = -1.0f;
	float top    =  1.0f;
	float near_p   = -1.0f;
	float far_p    =  1.0f;
	int count = 2;
	for(int i = 0; i < count; i++)
	{
		glm::vec2* _vec = 0;
		switch(i)
		{
		case 0:_vec = &top_left;    break;
		case 1:_vec = &bottom_right;break;
		}
		glm::mat4 _scale = glm::mat4((aspect), 0.0f , 0.0f , 0.0f, 
			0.0f , 1.0f , 0.0f , 0.0f,
			0.0f , 0.0f , 1.0f , 0.0f,
			0.0f   , 0.0f , 0.0f , 1.0f);
		
		glm::mat4 _projection = glm::ortho(left , right , bottom , top , near_p , far_p);
		
		glm::mat4 view = glm::mat4( 1.0f , 0.0f , 0.0f , 0.0f,
									0.0f , 1.0f , 0.0f , 0.0f,
									0.0f , 0.0f , 1.0f , 0.0f,
								   _Vector.x , _Vector.y , 0.0f , 1.0f);

		float angle = _Angle;
		angle *= DEG_2_RAD;
		glm::mat4 model( cos(angle) , sin(angle) ,	0.0f , 0.0f ,
		        	-sin(angle) , cos(angle) ,  0.0f , 0.0f ,
					0.0f , 0.0f , 1.0f , 0.0f,
					0.0f , 0.0f , 0.0f , 1.0f);

			float scalex(1.0f);float scaley(1.0f);
			switch(_ScaleOrientation)
			{
			case KrazyScale_X: scalex = 1.0f;scaley = aspect;break;
			case KrazyScale_Y: scalex = aspect;scaley = 1.0f;break;
			}

			glm::mat4 scale = glm::mat4( scalex , 0.0f , 0.0f , 0.0f,
					0.0f , scaley , 0.0f , 0.0f,
					0.0f , 0.0f , 1.0f , 0.0f,
					0.0f , 0.0f , 0.0f , 1.0f);

			glm::vec4 position = _projection * _scale * view * model * scale * glm::vec4(*_vec , 0.0f , 1.0f);
			*_vec = glm::vec2(position);
	}
	//
	switch(_PTRCurrentAssignedSingleLane->getProperties()->get_LaneDirection())
	{
	case KRAZY_LANE_DIRECTION_UP_HORZS     :
		if((x >= bottom_right.x && y >= bottom_right.y) && (x <= top_left.x && y <= top_left.y))
		{
#if 0
			ON_SWITCH_TOUCH();
#endif
			return KRAZY_BOOL_True;
			//printf("Touched %s On %s\n." , to_string().c_str() , "KRAZY_LANE_DIRECTION_UP_HORZS");
		}
		break;
	case KRAZY_LANE_DIRECTION_DOWN_HORZS   :
		if((x >= top_left.x && y >= top_left.y) && (x <= bottom_right.x && y <= bottom_right.y))
		{
#if 0
			ON_SWITCH_TOUCH();
#endif
			return KRAZY_BOOL_True;
			//printf("Touched %s On %s\n." , to_string().c_str() , "KRAZY_LANE_DIRECTION_DOWN_HORZS");
		}
		break;
	case KRAZY_LANE_DIRECTION_UP_VERTS     :
		if((x >= bottom_right.x && x <= top_left.x) && (y >= top_left.y && y <= bottom_right.y))
		{
#if 0
			ON_SWITCH_TOUCH();
#endif
			return KRAZY_BOOL_True;
			//printf("Touched %s On %s\n." , to_string().c_str() , "KRAZY_LANE_DIRECTION_UP_VERTS");
		}
		break;
	case KRAZY_LANE_DIRECTION_DOWN_VERTS   :
		if((x >= top_left.x && x <= bottom_right.x) && (y >= bottom_right.y && y <= top_left.y))
		{
#if 0
			ON_SWITCH_TOUCH();
#endif
			return KRAZY_BOOL_True;
			//printf("Touched %s On %s\n." , to_string().c_str() , "KRAZY_LANE_DIRECTION_DOWN_VERTS");
		}
		break;
	}
	//..,
	return KRAZY_BOOL_False;
}
//..,
void KrazyVehical::on_touched()
{
	ON_SWITCH_TOUCH();
}
//...,
#define ON_SWITCH_TOUCH_ON_REFLECTION() \
	switch(_TouhedReflection) \
	{ \
	case KRAZY_BOOL_True :_TouhedReflection = KRAZY_BOOL_False; break; \
	case KRAZY_BOOL_False:_TouhedReflection = KRAZY_BOOL_True;  break; \
	}\
//
//#define _REFLECTION_LOG_ON
KrazyBool KrazyVehical::on_touch_reflection(float x , float y)
{
	if(!_PTRCurrentAssignedSingleLane)
		return KRAZY_BOOL_False;
	//
	on_set_reflection();
	glm::vec2 top_left(_top_left[0] , _top_left[1]);
	glm::vec2 bottom_right(_bottom_right[0] , _bottom_right[1]);
	float width  = float(KrazyViewPortContext::_Width);
	float height = float(KrazyViewPortContext::_Height);
	float aspect = width/height;
	float left   = -1.0f * aspect;
	float right  =  1.0f * aspect;
	float bottom = -1.0f;
	float top    =  1.0f;
	float near_p   = -1.0f;
	float far_p    =  1.0f;
	int count = 2;
	for(int i = 0; i < count; i++){
		glm::vec2* _vec = 0;
		switch(i){
		case 0:_vec = &top_left;    break;
		case 1:_vec = &bottom_right;break;
		}
		glm::mat4 _scale = glm::mat4((aspect), 0.0f , 0.0f , 0.0f, 
			0.0f , 1.0f , 0.0f , 0.0f,
			0.0f , 0.0f , 1.0f , 0.0f,
			0.0f   , 0.0f , 0.0f , 1.0f);
		
		glm::mat4 _projection = glm::ortho(left , right , bottom , top , near_p , far_p);
		
		glm::mat4 view = glm::mat4( 1.0f , 0.0f , 0.0f , 0.0f,
									0.0f , 1.0f , 0.0f , 0.0f,
									0.0f , 0.0f , 1.0f , 0.0f,
								   _ReflectionVec.x , _ReflectionVec.y , 0.0f , 1.0f);

		float angle = _Angle;
		angle *= DEG_2_RAD;
		glm::mat4 model( cos(angle) , sin(angle) ,	0.0f , 0.0f ,
		        	-sin(angle) , cos(angle) ,  0.0f , 0.0f ,
					0.0f , 0.0f , 1.0f , 0.0f,
					0.0f , 0.0f , 0.0f , 1.0f);

			float scalex(1.0f);float scaley(1.0f);
			switch(_ScaleOrientation)
			{
			case KrazyScale_X: scalex = 1.0f;scaley = aspect;break;
			case KrazyScale_Y: scalex = aspect;scaley = 1.0f;break;
			}

			glm::mat4 scale = glm::mat4( scalex , 0.0f , 0.0f , 0.0f,
					0.0f , scaley , 0.0f , 0.0f,
					0.0f , 0.0f , 1.0f , 0.0f,
					0.0f , 0.0f , 0.0f , 1.0f);

			glm::vec4 position = _projection * _scale * view * model * scale * glm::vec4(*_vec , 0.0f , 1.0f);
			*_vec = glm::vec2(position);
	}
	//
	switch(_PTRCurrentAssignedSingleLane->getProperties()->get_LaneDirection())
	{
	case KRAZY_LANE_DIRECTION_UP_HORZS     :
		if((x >= bottom_right.x && y >= bottom_right.y) && (x <= top_left.x && y <= top_left.y))
		{
			ON_SWITCH_TOUCH_ON_REFLECTION();
#ifdef _REFLECTION_LOG_ON
			printf("Reflection of %s Touched On %s\n." , to_string().c_str() , "KRAZY_LANE_DIRECTION_DOWN_HORZS");
#endif
			return KRAZY_BOOL_True;
		}
		break;
	case KRAZY_LANE_DIRECTION_DOWN_HORZS   :
		if((x >= top_left.x && y >= top_left.y) && (x <= bottom_right.x && y <= bottom_right.y))
		{
			ON_SWITCH_TOUCH_ON_REFLECTION();
#ifdef _REFLECTION_LOG_ON
			printf("Reflection of %s Touched On %s\n." , to_string().c_str() , "KRAZY_LANE_DIRECTION_UP_HORZS");
#endif
			return KRAZY_BOOL_True;
		}
		break;

	case KRAZY_LANE_DIRECTION_UP_VERTS     :
		if((x >= bottom_right.x && x <= top_left.x) && (y >= top_left.y && y <= bottom_right.y))
		{
			ON_SWITCH_TOUCH_ON_REFLECTION();
#ifdef _REFLECTION_LOG_ON
			printf("Reflection of %s Touched On %s\n." , to_string().c_str() , "KRAZY_LANE_DIRECTION_DOWN_VERTS");
#endif
			return KRAZY_BOOL_True;
			
		}
		break;
	case KRAZY_LANE_DIRECTION_DOWN_VERTS   :
		if((x >= top_left.x && x <= bottom_right.x) && (y >= bottom_right.y && y <= top_left.y))
		{
			ON_SWITCH_TOUCH_ON_REFLECTION();
#ifdef _REFLECTION_LOG_ON
			printf("Reflection of %s Touched On %s\n." , to_string().c_str() , "KRAZY_LANE_DIRECTION_UP_VERTS");
#endif
			return KRAZY_BOOL_True;
		}
		break;
	}
	//
	return KRAZY_BOOL_False;
}
//
void KrazyVehical::on_setNextTurnIndex(int index)
{
	_NextTurnIndex = index;
}
//
int	 KrazyVehical::getNextTurnIndex()
{
	return _NextTurnIndex;
}
//
KrazyTrafficIndicator* KrazyVehical::get_NextTurn()
{
	return _PTRNextTurn;
}
//
void KrazyVehical::on_laneChange(KrazyTrafficIndicator* ptr_turn_indc)
{
	_PTRNextTurn = ptr_turn_indc;
}
//
void KrazyVehical::on_setIndex(int index)
{
	_Index = index;
	char text[10];
	sprintf(text , "%d" , _Index);
	_IndexName = text;
}
//
void KrazyVehical::increment_qcounter()
{
	_WaitQCounter++;
	_WaitState = KRAZY_VEHICAL_WAIT_STATE_Queued;
}
//
void KrazyVehical::decrement_qcounter()
{
	if(_WaitQCounter > 0){
		if( --_WaitQCounter == 0)
			_WaitState = KRAZY_VEHICAL_WAIT_STATE_Default;
	}
}
//
void KrazyVehical::on_setPreviousRearVehical(KrazyVehical* ptr_vehical)
{
	_PTRPrevRearVehical = ptr_vehical;
}
//
KrazyVehical* KrazyVehical::get_PreviousRearVehical()
{
	return _PTRPrevRearVehical;
}
//
KrazyVehicalList* KrazyVehical::get_QueuedPreviousVehicals()
{
	return _PTRRearQueuedVehicals;
}
//
string KrazyVehical::get_IndexName()
{
	return _IndexName;
}
//
void KrazyVehical::swap_lights()
{
		KrazyVehicalShade* aux = _PTRPoliceLight_1;
		_PTRPoliceLight_1 = _PTRPoliceLight_2;
		_PTRPoliceLight_2 = aux;
}
//
void KrazyVehical::on_setShade(int shader , int shader_type)
{
	_ShaderType = KrazyVehicalShaderType(shader_type);
	_PTRShade   = KrazyVehicalShadeFactory::getDefault()->getShade(KrazyShade(shader));
}
//
KrazyVehicalName& KrazyVehical::vehical_name()
{
	return _VehicalName;
}
//
void KrazyVehical::on_setName(KrazyVehicalName vehical_name)
{
	_VehicalName = vehical_name;
	switch(_VehicalName)
	{
	case KRAZY_VEHICAL_in_GAMEPLAY_RangToona:
		_Type = KRAZY_VEHICAL_TYPE_NORMAL;
		_ShaderType = KRAZY_SHADER_Normal;
		_PTRShade   = KrazyVehicalShadeFactory::getDefault()->getShade(KrazyShade_Red);
		_RouteMode = KRAZY_ROUTEMODE_Normal;
		_TouchDetectionEnabled = KRAZY_BOOL_True;
		_ReflectionTouchDetectionEnabled = KRAZY_BOOL_True;
		_Gameplay_turn_Limit = _KRAZY_TOONABROTHERS_TURN_LIMIT;
		break;
	case KRAZY_VEHICAL_in_GAMEPLAY_GangToona:
		_Type = KRAZY_VEHICAL_TYPE_NORMAL;
		_ShaderType = KRAZY_SHADER_Normal;
		_PTRShade   =  KrazyVehicalShadeFactory::getDefault()->getShade(KrazyShade_Green);
		_TouchDetectionEnabled = KRAZY_BOOL_True;
		_ReflectionTouchDetectionEnabled = KRAZY_BOOL_True;
		_Gameplay_turn_Limit = _KRAZY_TOONABROTHERS_TURN_LIMIT;
		break;
	case KRAZY_VEHICAL_in_GAMEPLAY_BangToona:
		_Type = KRAZY_VEHICAL_TYPE_NORMAL;
		_ShaderType = KRAZY_SHADER_Normal;
		_PTRShade   =  KrazyVehicalShadeFactory::getDefault()->getShade(KrazyShade_Blue);
		_RouteMode = KRAZY_ROUTEMODE_ByVehical;
		_TouchDetectionEnabled = KRAZY_BOOL_True;
		_ReflectionTouchDetectionEnabled = KRAZY_BOOL_True;
		_Gameplay_turn_Limit = _KRAZY_TOONABROTHERS_TURN_LIMIT;
		break; //AKA "Bangua"
	//Unga Family
	case KRAZY_VEHICAL_in_GAMEPLAY_Panguna:
		_Type = KRAZY_VEHICAL_TYPE_NORMAL;
		_ShaderType = KRAZY_SHADER_Normal;
		_PTRShade   =  KrazyVehicalShadeFactory::getDefault()->getShade(KrazyShade_Yellow);
		_TouchDetectionEnabled = KRAZY_BOOL_True;
		_ReflectionTouchDetectionEnabled = KRAZY_BOOL_False;
		_Gameplay_turn_Limit = _KRAZY_UNABROTHERS_GAMEPLAY_TURN_LIMIT;
		_RouteThriver = KrazyRouteThriverFactory::getDefault()->getNewRouteThriver(KRAZY_ROUTE_THRIVER_UnaFamily);
		((KrazyUnaBrotherRouteThriver*)_RouteThriver)->on_attach_vehical(this);
		break;
	case KRAZY_VEHICAL_in_GAMEPLAY_Nanguna:
		_Type = KRAZY_VEHICAL_TYPE_NORMAL;
		_ShaderType = KRAZY_SHADER_Normal;
		_PTRShade   =  KrazyVehicalShadeFactory::getDefault()->getShade(KrazyShade_Orange);
		_TouchDetectionEnabled = KRAZY_BOOL_True;
		_ReflectionTouchDetectionEnabled = KRAZY_BOOL_False;
		_Gameplay_turn_Limit = _KRAZY_UNABROTHERS_GAMEPLAY_TURN_LIMIT;
		_RouteThriver = KrazyRouteThriverFactory::getDefault()->getNewRouteThriver(KRAZY_ROUTE_THRIVER_UnaFamily);
		((KrazyUnaBrotherRouteThriver*)_RouteThriver)->on_attach_vehical(this);
		break;
	case KRAZY_VEHICAL_in_GAMEPLAY_Ganungi:
		_Type = KRAZY_VEHICAL_TYPE_NORMAL;
		_ShaderType = KRAZY_SHADER_Normal;
		_PTRShade   =  KrazyVehicalShadeFactory::getDefault()->getShade(KrazyShade_Pink);
		_TouchDetectionEnabled = KRAZY_BOOL_True;
		_ReflectionTouchDetectionEnabled = KRAZY_BOOL_False;
		_RouteMode = KRAZY_ROUTEMODE_ByVehical;
		_Gameplay_turn_Limit = _KRAZY_GANGUNI_GAMEPLAY_TURN_LIMIT;
		_Add_to_DutyQ = KRAZY_BOOL_True;
		break; //AKA Gangi
	//Piston Family..
	case KRAZY_VEHICAL_in_GAMEPLAY_Pastunua:
		_Type = KRAZY_VEHICAL_TYPE_NORMAL;
		_ShaderType = KRAZY_SHADER_Normal;
		_PTRShade   =  KrazyVehicalShadeFactory::getDefault()->getShade(KrazyShade_Purple);
		_TouchDetectionEnabled = KRAZY_BOOL_False;
		_ReflectionTouchDetectionEnabled = KRAZY_BOOL_False;
		break; //AKA Piston
	case KRAZY_VEHICAL_in_GAMEPLAY_Bhunsi:
		_Type = KRAZY_VEHICAL_TYPE_NORMAL;
		_ShaderType = KRAZY_SHADER_Normal;
		_PTRShade   =  KrazyVehicalShadeFactory::getDefault()->getShade(KrazyShade_Brown);
		_TouchDetectionEnabled = KRAZY_BOOL_False;
		_ReflectionTouchDetectionEnabled = KRAZY_BOOL_False;
		break;
	case KRAZY_VEHICAL_in_GAMEPLAY_Bhanush:
		_Type = KRAZY_VEHICAL_TYPE_NORMAL;
		_ShaderType = KRAZY_SHADER_Normal;
		_PTRShade   =  KrazyVehicalShadeFactory::getDefault()->getShade(KrazyShade_Turquoisblue);
		_TouchDetectionEnabled = KRAZY_BOOL_False;
		_ReflectionTouchDetectionEnabled = KRAZY_BOOL_False;
		break;//AKA Tadank
	case KRAZY_VEHICAL_in_GAMEPLAY_Sherrif:
		_Type = KRAZY_VEHICAL_TYPE_BLOCKING_CAR;
		_ShaderType = KRAZY_SHADER_Sherrif;
		_PTRShade   =  KrazyVehicalShadeFactory::getDefault()->getShade(KrazyShade_Sherrif);
		_TouchDetectionEnabled = KRAZY_BOOL_False;
		_ReflectionTouchDetectionEnabled = KRAZY_BOOL_False;
		_ViolationRegister = new KrazyViolatorRegister(this);
		break;
	}
}
//
void KrazyVehical::on_setIndexAfterWaiting()
{
	if(_PTRBlocking)
	{
		int indicators = _TrafficIndicatorMap.size();
		for(int indicator = 0; indicator < indicators; indicator++)
		{
			KrazyTrafficIndicator* _indicator = _TrafficIndicatorMap[indicator];
			if(_indicator == _PTRBlocking)
			{
				_NextTurnIndex = indicator + 1;
				break;
			}
		}
	}
}
//
void KrazyVehical::on_setSherrif(KrazyTrafficIndicator* bay)
{
	//This function is to manouver the sherrif vehical in the sherrif bay.., once the vehical is on the assigned lane.., or in constant entry mode..
	_PTRSherrifBay = bay;
	on_setName(KRAZY_VEHICAL_in_GAMEPLAY_Sherrif);
	KrazyLaneDirection   curr_properties = bay->PTRParentLaneNode()->getProperties()->get_LaneDirection();
	switch(curr_properties)
	{
	case KRAZY_LANE_DIRECTION_UP_HORZS     : _Angle = 270.0f;_ScaleOrientation = KrazyScale_Y;break;
	case KRAZY_LANE_DIRECTION_DOWN_HORZS   : _Angle = 90.0f;_ScaleOrientation  = KrazyScale_Y;break;
	case KRAZY_LANE_DIRECTION_UP_VERTS     : _Angle = 180.0f;_ScaleOrientation = KrazyScale_X;break;
	case KRAZY_LANE_DIRECTION_DOWN_VERTS   : _Angle = 360.0f;_ScaleOrientation = KrazyScale_X;break;
	}
}
//
KrazyTrafficIndicator* KrazyVehical::get_sherrifBay()
{
	return _PTRSherrifBay;
}
//
void KrazyVehical::on_setOnSpotFlipIndicator(KrazyTrafficIndicator* indicator)
{
	_PTROnSpotFlipDestination = indicator;
}
//
KrazyTrafficIndicator* KrazyVehical::get_onSpotFlipIndicator()
{
	if(!_PTROnSpotFlipDestination){
		// creating on spot indicator only once ..,
		_PTROnSpotFlipDestination = _PTRCurrentAssignedSingleLane->createOnSpotFlipIndicator();
		_PTROnSpotFlipDestination->Type() = KRAZY_TRAFFIC_INDC_OnSpotFlip;
	}
	return _PTROnSpotFlipDestination;
}
//
string KrazyVehical::to_string()
{
	if(_Vehical_z_Name.empty())
	{
		switch(_VehicalName)
		{
		case KRAZY_VEHICAL_in_GAMEPLAY_RangToona : _Vehical_z_Name = "RangToona";break;
		case KRAZY_VEHICAL_in_GAMEPLAY_GangToona : _Vehical_z_Name = "GangToona";break;
		case KRAZY_VEHICAL_in_GAMEPLAY_BangToona : _Vehical_z_Name = "BangToona";break; //AKA "Bangua"
			//Unga Family
		case KRAZY_VEHICAL_in_GAMEPLAY_Panguna  : _Vehical_z_Name = "Panguna";break;
		case KRAZY_VEHICAL_in_GAMEPLAY_Nanguna  : _Vehical_z_Name = "Nanguna";break;
		case KRAZY_VEHICAL_in_GAMEPLAY_Ganungi  : _Vehical_z_Name = "Ganungi";break; //AKA Gangi
			//Piston Family..
		case KRAZY_VEHICAL_in_GAMEPLAY_Pastunua : _Vehical_z_Name = "Pastunua";break; //AKA Piston
		case KRAZY_VEHICAL_in_GAMEPLAY_Bhunsi   : _Vehical_z_Name = "Bhunsi";break;
		case KRAZY_VEHICAL_in_GAMEPLAY_Bhanush  : _Vehical_z_Name = "Bhanush";break;//AKA Tadank
		case KRAZY_VEHICAL_in_GAMEPLAY_Sherrif  : _Vehical_z_Name = "Sherrif";break;
		}
	}

	return _Vehical_z_Name;
}
//
void KrazyVehical::on_setIndexAfterTouch()
{
	switch(_TrafficIndicatorMap[_NextTurnIndex]->Type())
	{
		case KRAZY_TRAFFIC_INDC_NoEntry:
		case KRAZY_TRAFFIC_INDC_Wait:
			return;
	}
	unsigned int index = _NextTurnIndex + 1;
	if(index < _TrafficIndicatorMap.size()){
		_NextTurnIndex = index;
	}
}
//
void KrazyVehical::on_setDestinationLane(KrazyLaneNode* lane)
{
	_PTRDestinationSingleLane = lane;
}
//
KrazyLaneNode* KrazyVehical::get_destinationLane()
{
	return _PTRDestinationSingleLane;
}
//
void KrazyVehical::on_set_blocking_indicator(KrazyTrafficIndicator* _blocking)
{
	_PTRBlocking = _blocking;
}
//
KrazyTrafficIndicator* KrazyVehical::get_blocking_indicator()
{
	return _PTRBlocking;
}
//
void KrazyVehical::on_set_exit(KrazyTrafficIndicator* _indicator)
{
	_PTRExit = _indicator;
}
//
KrazyTrafficIndicator* KrazyVehical::get_exit()
{
	return _PTRExit;
}
//
void KrazyVehical::on_set_reflection()
{
	float _x = 0.0f , _y = 0.0f;
	if(_PTRCurrentAssignedSingleLane){
		KrazyLaneNode* _opposit_lane = _PTRCurrentAssignedSingleLane->getOppositeSingleLane();
		switch(_opposit_lane->getProperties()->get_LaneDirection()){
			case KRAZY_LANE_DIRECTION_UP_HORZS     :	
			case KRAZY_LANE_DIRECTION_DOWN_HORZS   :	
				_x = _Vector.x;_y = _opposit_lane->getFlipTrafficIndicator()->Vector()->y;
				 break;
			case KRAZY_LANE_DIRECTION_UP_VERTS     :	
			case KRAZY_LANE_DIRECTION_DOWN_VERTS   :	
				_x = _opposit_lane->getFlipTrafficIndicator()->Vector()->x; _y = _Vector.y;
				 break;
		}
	}
	//
	_ReflectionVec = glm::vec2(_x,_y);
}
//
glm::vec2* KrazyVehical::get_reflection()
{
	return &_ReflectionVec;
}
//
void KrazyVehical::on_turning(KrazyTrafficIndicator* _indicator)
{
	waitstate() = KRAZY_VEHICAL_WAIT_STATE_Turning;
	on_laneChange(_indicator);	
}
//
void KrazyVehical::on_default()
{
	waitstate() = KRAZY_VEHICAL_WAIT_STATE_Default;
	on_laneChange(0);
}
//
void KrazyVehical::on_set_route_thriver(KrazyRouteThriver* _route_thriver)
{
	_RouteThriver = _route_thriver;
}
//
KrazyRouteThriver* KrazyVehical::get_route_thriver()
{
	return _RouteThriver;
}
//
string KrazyVehicalUtility::_convert_vehical_name(KrazyVehicalName _vehical_name)
{
	string _res;
	switch(_vehical_name)
	{
	case KRAZY_VEHICAL_in_GAMEPLAY_RangToona : _res = "RangToona";break;
	case KRAZY_VEHICAL_in_GAMEPLAY_GangToona : _res = "GangToona";break;
	case KRAZY_VEHICAL_in_GAMEPLAY_BangToona : _res = "BangToona";break; //AKA "Bangua"
		//Unga Family
	case KRAZY_VEHICAL_in_GAMEPLAY_Panguna  : _res = "Panguna";break;
	case KRAZY_VEHICAL_in_GAMEPLAY_Nanguna  : _res = "Nanguna";break;
	case KRAZY_VEHICAL_in_GAMEPLAY_Ganungi  : _res = "Ganungi";break; //AKA Gangi
		//Piston Family..
	case KRAZY_VEHICAL_in_GAMEPLAY_Pastunua : _res = "Pastunua";break; //AKA Piston
	case KRAZY_VEHICAL_in_GAMEPLAY_Bhunsi   : _res = "Bhunsi";break;
	case KRAZY_VEHICAL_in_GAMEPLAY_Bhanush  : _res = "Bhanush";break;//AKA Tadank
	case KRAZY_VEHICAL_in_GAMEPLAY_Sherrif  : _res = "Sherrif";break;
	}
	return _res;

}