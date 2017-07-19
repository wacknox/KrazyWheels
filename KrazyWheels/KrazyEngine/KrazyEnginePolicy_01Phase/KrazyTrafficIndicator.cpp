#include "stdafx.h"
#include "KrazyTrafficIndicator.h"
#include "KrazyLaneNode.h"
#include "KrazyViewPortContext.h"
#include "KrazyColorDefinations.h"
//
KrazyTrafficIndicator::KrazyTrafficIndicator() :
_PTRParentLaneNode(0)
, _Type(0)
, _RotationAngle(0)
, _ParentIndex(0)
, _Taken(false)
,_PTRSource(0)
,_Index(0)
,_Wait(0)
,_PTRWaitingVehical(0)
,_Green(KRAZY_BOOL_False)
,_PTRBay(0)
,_VehicalWaiting(KRAZY_VEHICAL_WAITING_Default)
,_EntanceContext(0)
{
	set_lights();
}
//
KrazyTrafficIndicator::KrazyTrafficIndicator(KrazyLaneNode* node) :
_PTRParentLaneNode(node)
, _Type(0)
, _RotationAngle(0)
, _ParentIndex(0)
, _Taken(false)
,_PTRSource(0)
,_Index(0)
,_Wait(0)
,_PTRWaitingVehical(0)
,_Green(KRAZY_BOOL_False)
,_PTRBay(0)
,_VehicalWaiting(KRAZY_VEHICAL_WAITING_Default)
,_EntanceContext(0)
{
	set_lights();
}
//
KrazyTrafficIndicator::~KrazyTrafficIndicator()
{
	cleanUp();
}
//
void KrazyTrafficIndicator::cleanUp()
{
	if(_EntanceContext){
		delete _EntanceContext;_EntanceContext = 0;
	}
}
//
void KrazyTrafficIndicator::set_lights()
{
	for(int i = 0; i < 4; i++)
	{
		_GreenLight.push_back(COLOR_GREEN[i]);
		_YellowLight.push_back(COLOR_YELLOW[i]);
	}
	_CurrentLight = &_YellowLight[0];
}
//////////////////////////////////////////////////////////////// Properties ///////////////////////////////////////////////////////////////////////////////
unsigned int&	KrazyTrafficIndicator::Type()				{ return _Type;}
unsigned int    KrazyTrafficIndicator::index()				{ return _Index;}
KrazyLaneNode*	KrazyTrafficIndicator::PTRParentLaneNode()  { return _PTRParentLaneNode;}
glm::vec2*		KrazyTrafficIndicator::Vector()				{ return &_Vector;} 
float&			KrazyTrafficIndicator::RotationAngle()		{ return _RotationAngle;}	
int&			KrazyTrafficIndicator::ParentIndex()		{ return _ParentIndex;}
//
void KrazyTrafficIndicator::set_VectorByIndices(KrazyFloatList& indices_f){
	int count = indices_f.size();
	if(count < 2)
		return;

	for(int i = 0; i < 2; i++)
		_Vector[i] = indices_f[i];
}
//
void KrazyTrafficIndicator::set_ParentLaneNode(KrazyLaneNode* node)
{
	if(!_PTRParentLaneNode){
		_PTRParentLaneNode = node;
	}
}
//
void KrazyTrafficIndicator::set_RectByIndices(KrazyFloatList& indices_f){
	int count = indices_f.size();
	if(count < 4)
		return;
}
//
void KrazyTrafficIndicator::add_DestinationIndc(KrazyTrafficIndicator* traffic_indc)
{
	_DestinationIndcList.push_back(traffic_indc);
}
//
KrazyEntranceContext* KrazyTrafficIndicator::get_EntranceContext()
{
	if(!_EntanceContext){
		_EntanceContext = new KrazyEntranceContext;
	}
	return _EntanceContext;
}
//
int KrazyTrafficIndicator::get_DestinationIndcCount()
{
	return _DestinationIndcList.size();
}
//
KrazyTrafficIndicator* KrazyTrafficIndicator::get_DestinationIndc(int index)
{
	return _DestinationIndcList[index];
}
//
bool KrazyTrafficIndicator::is_alreadyTaken()
{
	return _Taken;
}
//
void KrazyTrafficIndicator::updateTaken()
{
	_Taken = (_Taken) ?  false : true;
}
//
void KrazyTrafficIndicator::set_Source(KrazyTrafficIndicator* source)
{
	if(!_PTRSource)
		_PTRSource = source;
}
//
KrazyTrafficIndicator* KrazyTrafficIndicator::get_Source()
{
	return _PTRSource;
}
//
void KrazyTrafficIndicator::set_Index(int index)
{
	_Index = index;
}
//
void KrazyTrafficIndicator::on_resize()
{
		glm::vec2* _vec = &_Vector;
		glm::mat4 projection = KrazyViewPortContext::_ProjectionMatrix;
		glm::mat4 window_scale = KrazyViewPortContext::_ScreenScale;
		glm::vec4 _vec_pos = projection *  window_scale * glm::vec4(*_vec , 0.0f , 1.0f );
		*_vec = glm::vec2(_vec_pos);
}
//
void KrazyTrafficIndicator::swap_lights()
{
	if(_CurrentLight == &_YellowLight[0]){
		_CurrentLight = &_GreenLight[0];
		_Green = KRAZY_BOOL_True;
	}
	else if(_CurrentLight == &_GreenLight[0]){
		_CurrentLight = &_YellowLight[0];
	    _Green = KRAZY_BOOL_False;
	}
}
//
float* KrazyTrafficIndicator::get_CurrentLight()
{
	return _CurrentLight;
}
//
int& KrazyTrafficIndicator::wait()
{
	return _Wait;
}
//
KrazyVehical* KrazyTrafficIndicator::waiting_vehical()
{
	return _PTRWaitingVehical;
}
//
void KrazyTrafficIndicator::set_WaitingVehical(KrazyVehical* vehical)
{
	_PTRWaitingVehical = vehical;
}
//
KrazyBool KrazyTrafficIndicator::is_green()
{
	return _Green;
}
//
KrazyVehicalWaiting& KrazyTrafficIndicator::waiting_vehical_state()
{
	return _VehicalWaiting;
}
//
void KrazyTrafficIndicator::on_reset(glm::vec2& vec , KrazyLaneNode* curr_lane)
{
	_Vector = vec;
	_PTRParentLaneNode = curr_lane;
}
//
void KrazyTrafficIndicator::set_assignedLanes_for_Sherrif(KrazyIntList& list)
{
	int count = list.size();
	for(int i = 0; i < count; i++)
	{
		_AssignedLaneList_for_Sherrif.push_back(list[i]);
	}
}
//
KrazyIntList* KrazyTrafficIndicator::get_assignedLanes_for_Sherrif()
{
	return &_AssignedLaneList_for_Sherrif;
}
//
KrazyTrafficIndicator* KrazyTrafficIndicator::get_bay()
{
	return _PTRBay;
}
//
void KrazyTrafficIndicator::on_set_bay(glm::vec2& vec , KrazyTrafficIndicator* bay)
{
	_PTRBay = bay;
	KrazyLaneDirection _lane_direction = _PTRParentLaneNode->getProperties()->get_LaneDirection();
	switch(_lane_direction)
	{
		case KRAZY_LANE_DIRECTION_UP_HORZS     : 
		case KRAZY_LANE_DIRECTION_DOWN_HORZS   : 
			{
				_Vector = glm::vec2(vec.x , _PTRParentLaneNode->getFlipTrafficIndicator()->Vector()->y);
			}
			break;
		
		case KRAZY_LANE_DIRECTION_UP_VERTS     :
		case KRAZY_LANE_DIRECTION_DOWN_VERTS   :
			{
				_Vector = glm::vec2(_PTRParentLaneNode->getFlipTrafficIndicator()->Vector()->x , vec.y);
			}
			break;
	}
	
}