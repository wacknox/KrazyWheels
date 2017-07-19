#include "stdafx.h"
#include "KrazyLaneNode.h"
#include "KrazyEnums.h"
#include "KrazyViewPortContext.h"
#define KRAZY_GET_SINGLE_LANE_BY_DIRECTION(direction , ptr_lane)\
	if(!_PTRParent && _Properties.get_LaneType() == KRAZY_LANE_Type_Double && _Kids.size() == 2){\
		switch(direction << 8){\
		case KRAZY_LANE_DIRECTION_UP	: ptr_lane = _Kids[0];\
		case KRAZY_LANE_DIRECTION_DOWN  : ptr_lane = _Kids[1];\
	    }\
	}\
//
/////////////////////////////////////////////////////// KrazyLaneProperties ////////////////////////////////////////////////
KrazyLaneProperties::KrazyLaneProperties(KrazyLaneType lane_type , KrazyLaneDirection krazy_lane_direction ,  
	float x_0/* = 0.0f*/ , float y_0 /*= 0.0f*/ ,float x_1 /*= 0.0f*/ , float y_1/*= 0.0f*/,float px1 /*= 0.0f*/,float py1 /*= 0.0f*/) :
	_LaneType(lane_type),
    _LaneDirection(krazy_lane_direction),
	_x1(x_0),
	_y1(y_0),
	_x2(x_1),
	_y2(y_1),
	_px1(px1),
	_py1(py1)
	{}
//
KrazyLaneProperties::KrazyLaneProperties() :
	_LaneType(KRAZY_LANE_Type_Default),
	_LaneDirection(KRAZY_LANE_DIRECTION_DEFAULT),
	_x1(0),
	_y1(0),
	_x2(0),
	_y2(0),
	_px1(0),
	_py1(0)
	{}
//
KrazyLaneProperties::KrazyLaneProperties(const KrazyLaneProperties& properties)
{
	copy(properties);
}
//
KrazyLaneProperties& KrazyLaneProperties::operator=(const KrazyLaneProperties& properties)
{
	copy(properties);
	return *this;
}
//
void KrazyLaneProperties::set_RectByIndices(KrazyFloatList& indices_f)
{
	int count = indices_f.size();
	if(count < 4)
		return;

	_x1 = indices_f[0];_y1 = indices_f[1];
	_x2 = indices_f[2];_y2 = indices_f[3];

	_VertexCoords.push_back(_x1);_VertexCoords.push_back(_y1);_VertexCoords.push_back(0.0f);
	_VertexCoords.push_back(_x1);_VertexCoords.push_back(_y2);_VertexCoords.push_back(0.0f);
	_VertexCoords.push_back(_x2);_VertexCoords.push_back(_y1);_VertexCoords.push_back(0.0f);

	_VertexCoords.push_back(_x2);_VertexCoords.push_back(_y1);_VertexCoords.push_back(0.0f);
	_VertexCoords.push_back(_x1);_VertexCoords.push_back(_y2);_VertexCoords.push_back(0.0f);
	_VertexCoords.push_back(_x2);_VertexCoords.push_back(_y2);_VertexCoords.push_back(0.0f);

	///............................ Borders ..............................................//////////
	switch(_LaneDirection)
	{
	case KRAZY_LANE_DIRECTION_HORZS:
		{
			float diff   = abs((_y1 - _y2)/2.0f);
			float height = diff - 0.005f;
			//Up..,
			float bottom_for_up_lane = _y1 - height;
			 _BorderCoords.push_back(_x1);_BorderCoords.push_back(_y1);
			 _BorderCoords.push_back(_x1);_BorderCoords.push_back(bottom_for_up_lane);

			 _BorderCoords.push_back(_x1);_BorderCoords.push_back(bottom_for_up_lane);
			 _BorderCoords.push_back(_x2);_BorderCoords.push_back(bottom_for_up_lane);

			 _BorderCoords.push_back(_x2);_BorderCoords.push_back(bottom_for_up_lane);
			 _BorderCoords.push_back(_x2);_BorderCoords.push_back(_y1);

			 _BorderCoords.push_back(_x2);_BorderCoords.push_back(_y1);
			 _BorderCoords.push_back(_x1);_BorderCoords.push_back(_y1);
			 // Down
			 float top_for_down_lane = bottom_for_up_lane - 0.01f;
			 _BorderCoords.push_back(_x1);_BorderCoords.push_back(top_for_down_lane);
			 _BorderCoords.push_back(_x1);_BorderCoords.push_back(_y2);

			 _BorderCoords.push_back(_x1);_BorderCoords.push_back(_y2);
			 _BorderCoords.push_back(_x2);_BorderCoords.push_back(_y2);

			 _BorderCoords.push_back(_x2);_BorderCoords.push_back(_y2);
			 _BorderCoords.push_back(_x2);_BorderCoords.push_back(top_for_down_lane);

			 _BorderCoords.push_back(_x2);_BorderCoords.push_back(top_for_down_lane);
			 _BorderCoords.push_back(_x1);_BorderCoords.push_back(top_for_down_lane);
		}
		break;
	case KRAZY_LANE_DIRECTION_VERTS:
		 {
			 float diff  = abs((_x1 - _x2)/2.0f);
			 float width = (0.17f/2.0f) - 0.005f;//diff - 0.005f;
			 // Left..,
			 float right_for_left_lane = _x1 + width;
			 _BorderCoords.push_back(_x1);_BorderCoords.push_back(_y1);
			 _BorderCoords.push_back(_x1);_BorderCoords.push_back(_y2);

			 _BorderCoords.push_back(_x1);_BorderCoords.push_back(_y2);
			 _BorderCoords.push_back(right_for_left_lane);_BorderCoords.push_back(_y2);

			 _BorderCoords.push_back(right_for_left_lane);_BorderCoords.push_back(_y2);
			 _BorderCoords.push_back(right_for_left_lane);_BorderCoords.push_back(_y1);

			 _BorderCoords.push_back(right_for_left_lane);_BorderCoords.push_back(_y1);
			 _BorderCoords.push_back(_x1);_BorderCoords.push_back(_y1);
			 // Right...,
			 float left_for_right = right_for_left_lane + 0.01f;
			 _BorderCoords.push_back(left_for_right);_BorderCoords.push_back(_y1);
			 _BorderCoords.push_back(left_for_right);_BorderCoords.push_back(_y2);

			 _BorderCoords.push_back(left_for_right);_BorderCoords.push_back(_y2);
			 _BorderCoords.push_back(_x2);_BorderCoords.push_back(_y2);

			 _BorderCoords.push_back(_x2);_BorderCoords.push_back(_y2);
			 _BorderCoords.push_back(_x2);_BorderCoords.push_back(_y1);

			 _BorderCoords.push_back(_x2);_BorderCoords.push_back(_y1);
			 _BorderCoords.push_back(left_for_right);_BorderCoords.push_back(_y1);

		 }
		 break;
	}

}
//
void KrazyLaneProperties::set_PointByIndices(KrazyFloatList& indices_f)
{
	int count = indices_f.size();
	if(count < 2)
		return;

	_px1 = indices_f[0];_y1 = indices_f[1];
}
//
void KrazyLaneProperties::set_BorderCoords(KrazyFloatList& coords)
{
#if 1
	return;
#else
	int count = coords.size();
	for(int i = 0; i < count; i++)
	{
		_BorderCoords.push_back(coords[i]);
	}
#endif
}
/////////////////////////////////////////////////////// KrazyLaneNode /////////////////////////////////////////////////////////
KrazyLaneNode::KrazyLaneNode() : 
_PTRParent(0)
,_CurrentTrafficIndicatorIndex(0)
,_PTRLastEnteredVehical(0)
,_PTRFlipTrafficIndicator(0)
,_PTRLaneEntryIndicator(0)
,_FlipisAddedtoTrafficIndc(false)
,_PTRSearchOrigin(0)
,_PTRDestinationEnd(0)
,_VehicalCount(0)
,_PTRSherrifBay(0)
,_PTRSherrif(0)
,_PTRSliderPoint(0)
,_PTROnspot_flip(0)
{
}
//
KrazyLaneNode::KrazyLaneNode(KrazyLaneNode* parent) :
 _PTRParent(parent) 
,_CurrentTrafficIndicatorIndex(0)
,_PTRLastEnteredVehical(0)
,_PTRFlipTrafficIndicator(0)
,_PTRLaneEntryIndicator(0)
,_FlipisAddedtoTrafficIndc(false)
,_PTRSearchOrigin(0)
,_PTRDestinationEnd(0)
,_VehicalCount(0)
,_PTRSherrifBay(0)
,_PTRSherrif(0)
,_PTRSliderPoint(0)
,_PTROnspot_flip(0)
{
}
//
KrazyLaneNode::~KrazyLaneNode()
{
	cleanUp();
}
//
void KrazyLaneNode::cleanUp()
{
	//use ptr list;
	int count = _Kids.size();
	for(int i = 0; i < count;i++){
		delete _Kids[i];
	}
	_Kids.clear();
	// deletion with release
	count  = _TrafficIndicators.size();
	for(int i = 0; i < count;i++)
	{
		delete (KrazyLaneNodeTrafficIndicator*)_TrafficIndicators[i];
	}
	_TrafficIndicators.clear();
	
	count  = _DestinationIndicators.size();
	for(int i = 0; i < count;i++){
		delete (KrazyLaneNodeTrafficIndicator*)_DestinationIndicators[i];
	}
	_DestinationIndicators.clear();
	
	count  = _OnSpotFlipIndicators.size();
	for(int i = 0; i < count;i++){
		delete (KrazyLaneNodeTrafficIndicator*)_OnSpotFlipIndicators[i];
	}
	_OnSpotFlipIndicators.clear();
	//
	if(_PTRLaneEntryIndicator){
		delete _PTRLaneEntryIndicator;
		_PTRLaneEntryIndicator = 0;
	}
	// this indicator is already in the traffic indicator list..,
	/*if(_PTRFlipTrafficIndicator){
		delete _PTRFlipTrafficIndicator;
		_PTRFlipTrafficIndicator = 0;
	}*/
	//
	if(_PTRSherrifBay){  
		delete _PTRSherrifBay;
		_PTRSherrifBay = 0;
	}
	//
	if(_PTRSliderPoint){ 
		delete _PTRSliderPoint;
		_PTRSliderPoint = 0;
	}
	//
	if(_PTROnspot_flip){ 
		delete _PTROnspot_flip;
		_PTROnspot_flip = 0;
	}
	//
}
//
void KrazyLaneNode::on_resize()
{
	switch(_Properties.get_LaneType())
	{
	case KRAZY_LANE_Type_Double:
		_Kids[0]->on_resize();
		_Kids[1]->on_resize();
		break;
	case KRAZY_LANE_Type_Single:
		{
			//.............Projecting Traffic Indicators.......................
			int count = _TrafficIndicators.size();
			for(int i = 0; i < count; i++)
			{
				_TrafficIndicators[i]->on_resize();
			
			}
			//.............Projecting _DestinationIndicators Indicators.......................
			count = _DestinationIndicators.size();
			for(int i = 0; i < count; i++)
			{
				_DestinationIndicators[i]->on_resize();
				
			}
			
		}
		break;
	}
}
//
void KrazyLaneNode::on_attachLastVehical(KrazyVehical* ptr_vehical)
{
	_VehicalCount++;
	_PTRLastEnteredVehical = ptr_vehical;
}
//
void KrazyLaneNode::on_detachLastVehical(KrazyVehical* ptr_vehical)
{
	_VehicalCount--;
	if(_PTRLastEnteredVehical == ptr_vehical){
		_PTRLastEnteredVehical = 0;
	}
}
//
KrazyVehical*  KrazyLaneNode::get_LastVehical()
{
	if(!(_VehicalCount > 0))
		return 0;

	return _PTRLastEnteredVehical; 
}
//
KrazyTrafficIndicator* KrazyLaneNode::createTrafficIndicator()
{
	_TrafficIndicators.push_back(new KrazyLaneNodeTrafficIndicator(this));
	int count = _TrafficIndicators.size();
	return _TrafficIndicators[count - 1];
}
//
KrazyTrafficIndicator* KrazyLaneNode::createDestinationIndicator()
{
	int index = _DestinationIndicators.size();
	_DestinationIndicators.push_back(new KrazyLaneNodeTrafficIndicator(this));
	return _DestinationIndicators[index];
}
//
KrazyLaneNode* KrazyLaneNode::createKid()
{
	if(_Properties.get_LaneType() != KRAZY_LANE_Type_Double)
		return 0;
	
	int count = _Kids.size();
	_Kids.push_back(new KrazyLaneNode(this));
	return _Kids[count];
}
//
KrazyLaneNode* KrazyLaneNode::getLaneByDirection(int single_lane_direction)
{
	KrazyLaneNode* ptr_single_lane = 0;
	KRAZY_GET_SINGLE_LANE_BY_DIRECTION(single_lane_direction,ptr_single_lane);
	return ptr_single_lane;
}
//
void KrazyLaneNode::sort_allTrafficIndicators()
{
	if(!_FlipisAddedtoTrafficIndc && _PTRFlipTrafficIndicator){
		_FlipisAddedtoTrafficIndc = true;
		_TrafficIndicators.push_back(_PTRFlipTrafficIndicator);
	}
	KrazyLaneDirection lane_direction = _Properties.get_LaneDirection();
	KrazyBool wenttosorting = KRAZY_BOOL_False;
	int count = _TrafficIndicators.size();

	for(int i = 0; i < count; i++){
		for(int j = i+1; j < count;j++){
	    KrazyTrafficIndicator* ptr_traffic_indc_1 = _TrafficIndicators[i];
		KrazyTrafficIndicator* ptr_traffic_indc_2 = _TrafficIndicators[j];
		glm::vec2* ptr_traffic_indc_vec_1 = ptr_traffic_indc_1->Vector();
		glm::vec2* ptr_traffic_indc_vec_2 = ptr_traffic_indc_2->Vector();
		switch(lane_direction)
		{
		case KRAZY_LANE_DIRECTION_UP_HORZS     : 
			if(ptr_traffic_indc_vec_1->x < ptr_traffic_indc_vec_2->x){
				wenttosorting = KRAZY_BOOL_True;
				_TrafficIndicators[i] = ptr_traffic_indc_2;ptr_traffic_indc_2->set_Index(i);
				_TrafficIndicators[j] = ptr_traffic_indc_1;ptr_traffic_indc_1->set_Index(j);
			}
			break;
		case KRAZY_LANE_DIRECTION_DOWN_HORZS   :
			if(ptr_traffic_indc_vec_1->x > ptr_traffic_indc_vec_2->x){
				wenttosorting = KRAZY_BOOL_True;
				_TrafficIndicators[i] = ptr_traffic_indc_2;ptr_traffic_indc_2->set_Index(i);
				_TrafficIndicators[j] = ptr_traffic_indc_1;ptr_traffic_indc_1->set_Index(j);
			}
			break;
		case KRAZY_LANE_DIRECTION_UP_VERTS     :
			if(ptr_traffic_indc_vec_1->y > ptr_traffic_indc_vec_2->y){
				wenttosorting = KRAZY_BOOL_True;
				_TrafficIndicators[i] = ptr_traffic_indc_2;ptr_traffic_indc_2->set_Index(i);
				_TrafficIndicators[j] = ptr_traffic_indc_1;ptr_traffic_indc_1->set_Index(j);
			}
			break;
		case KRAZY_LANE_DIRECTION_DOWN_VERTS   :
			if(ptr_traffic_indc_vec_1->y < ptr_traffic_indc_vec_2->y){
				wenttosorting = KRAZY_BOOL_True;
				_TrafficIndicators[i] = ptr_traffic_indc_2;ptr_traffic_indc_2->set_Index(i);
				_TrafficIndicators[j] = ptr_traffic_indc_1;ptr_traffic_indc_1->set_Index(j);
			}
			break;
		}
	}
	}
	if(count > 1 && _TrafficIndicators[count - 1]->index() == 0){
		// if flip indicator did not got its index .., assign one;
		_TrafficIndicators[count - 1]->set_Index(count - 1);
	}
	switch(wenttosorting)
	{
	case KRAZY_BOOL_False:
		// already sorted set the index;;
		for(int i = 0; i < count; i++){
			_TrafficIndicators[i]->set_Index(i);
		}
		break;
	}
	count = _DestinationIndicators.size();
	for(int i = 0; i < count; i++){
		for(int j = i+1; j < count;j++){
	    KrazyTrafficIndicator* ptr_traffic_indc_1 = _DestinationIndicators[i];
		KrazyTrafficIndicator* ptr_traffic_indc_2 = _DestinationIndicators[j];
		glm::vec2* ptr_traffic_indc_vec_1 = ptr_traffic_indc_1->Vector();
		glm::vec2* ptr_traffic_indc_vec_2 = ptr_traffic_indc_2->Vector();
		switch(lane_direction)
		{
		case KRAZY_LANE_DIRECTION_UP_HORZS     : 
			if(ptr_traffic_indc_vec_1->x < ptr_traffic_indc_vec_2->x){
				_DestinationIndicators[i] = ptr_traffic_indc_2;ptr_traffic_indc_2->set_Index(i);
				_DestinationIndicators[j] = ptr_traffic_indc_1;ptr_traffic_indc_1->set_Index(j);
			}
			break;
		case KRAZY_LANE_DIRECTION_DOWN_HORZS   :
			if(ptr_traffic_indc_vec_1->x > ptr_traffic_indc_vec_2->x){
				_DestinationIndicators[i] = ptr_traffic_indc_2;ptr_traffic_indc_2->set_Index(i);
				_DestinationIndicators[j] = ptr_traffic_indc_1;ptr_traffic_indc_1->set_Index(j);
			}
			break;
		case KRAZY_LANE_DIRECTION_UP_VERTS     :
			if(ptr_traffic_indc_vec_1->y > ptr_traffic_indc_vec_2->y){
				_DestinationIndicators[i] = ptr_traffic_indc_2;ptr_traffic_indc_2->set_Index(i);
				_DestinationIndicators[j] = ptr_traffic_indc_1;ptr_traffic_indc_1->set_Index(j);
			}
			break;
		case KRAZY_LANE_DIRECTION_DOWN_VERTS   :
			if(ptr_traffic_indc_vec_1->y < ptr_traffic_indc_vec_2->y){
				_DestinationIndicators[i] = ptr_traffic_indc_2;ptr_traffic_indc_2->set_Index(i);
				_DestinationIndicators[j] = ptr_traffic_indc_1;ptr_traffic_indc_1->set_Index(j);
			}
			break;
		}
	}
	}
}
//
void KrazyLaneNode::setFlipTrafficIndicator(KrazyFloatList& list)
{
	int count = list.size();
	if( count < 2 )
		return;

	if(!_PTRFlipTrafficIndicator && !_PTRLaneEntryIndicator){
		_PTRFlipTrafficIndicator = new KrazyLaneNodeTrafficIndicator(this);
		_PTRLaneEntryIndicator = new KrazyLaneNodeTrafficIndicator(this);
		_PTRFlipTrafficIndicator->Type() = KRAZY_TRAFFIC_INDC_FlipPoint;
		_PTRFlipTrafficIndicator->set_VectorByIndices(list);
		_PTRLaneEntryIndicator->Type() = KRAZY_TRAFFIC_INDC_EntryPoint;
		KrazyFloatList list_entry;
		KrazyLaneDirection lane_direction = _Properties.get_LaneDirection();
		float x1(0.0f) , y1(0.0f);
		float x2(0.0f) , y2(0.0f);
		_Properties.get_Rect(x1,y1,x2,y2);
		switch(lane_direction)
		{
		case KRAZY_LANE_DIRECTION_UP_HORZS     :
			 list_entry.push_back(x2);
			 list_entry.push_back(list[1]);
			 break;
		case KRAZY_LANE_DIRECTION_DOWN_HORZS   : 
			 list_entry.push_back(x1);
			 list_entry.push_back(list[1]);
			 break;
		case KRAZY_LANE_DIRECTION_UP_VERTS     :
			 list_entry.push_back(list[0]);
			 list_entry.push_back(y2);
			 break;
		case KRAZY_LANE_DIRECTION_DOWN_VERTS   : 
			 list_entry.push_back(list[0]);
			 list_entry.push_back(y1);
			 break;
		}
		_PTRLaneEntryIndicator->set_VectorByIndices(list_entry);
	}
}
//
KrazyTrafficIndicator* KrazyLaneNode::getFlipTrafficIndicator()
{
	return _PTRFlipTrafficIndicator;
}
//
KrazyBool  KrazyLaneNode::foundSingleLane(KrazyLaneNode* node)
{
	int count = _Kids.size();
	for(int i = 0; i < count; i++){
		KrazyLaneNode* ptr_lane = _Kids[i];
		if(ptr_lane == node){
			return KRAZY_BOOL_True;
		}
	}
	return KRAZY_BOOL_False;
}
//
KrazyTrafficIndicator* KrazyLaneNode::get_slider_point()
{
	if(!_PTRSliderPoint)
	{
		_PTRSliderPoint = new KrazyLaneNodeTrafficIndicator(this);
		_PTRSliderPoint->Type() = KRAZY_TRAFFIC_INDC_SliderPoint;
	}
	return _PTRSliderPoint;
}
//
void KrazyLaneNode::createSherrifBay()
{
	if(_PTRSherrifBay)
		return;

	_PTRSherrifBay = new KrazyLaneNodeTrafficIndicator(this);
	_PTRSherrifBay->Type() = KRAZY_TRAFFIC_INDC_SherrifBay;
	float x1(0.0f) , y1(0.0f), x2(0.0f) , y2(0.0f);
	_Properties.get_Rect(x1 , y1 , x2 , y2);
	KrazyFloatList list;
	switch(_Properties.get_LaneDirection())
	{
	case KRAZY_LANE_DIRECTION_UP_HORZS   : 
		{
			glm::vec2 vec_1(x1,y1);
			glm::vec2 vec_2(x2,y1);
			list.push_back(x1 + abs(glm::distance(vec_1 , vec_2)/2.0f));
			list.push_back(y1 + (12.0f *  KrazyPIXEL));
			_PTRSherrifBay->RotationAngle() = 270.0f;
		}
		break;
	case KRAZY_LANE_DIRECTION_DOWN_HORZS : 
		{
			glm::vec2 vec_1(x1,y2);
			glm::vec2 vec_2(x2,y2);
			list.push_back(x1 + abs(glm::distance(vec_1 , vec_2)/2.0f));
			list.push_back(y2 + (2.0f *  KrazyPIXEL));
			_PTRSherrifBay->RotationAngle() = 90.0f;
		}
		break; 
	case KRAZY_LANE_DIRECTION_UP_VERTS	 :
		{
			glm::vec2 vec_1(x2,y1);
			glm::vec2 vec_2(x2,y2);
			list.push_back(y1 +  abs(glm::distance(vec_1 , vec_2)/2.0f));
			list.push_back(x2 +  (2.0f *  KrazyPIXEL));
			_PTRSherrifBay->RotationAngle() = 180.0f;
		}
		break;  
	case KRAZY_LANE_DIRECTION_DOWN_VERTS :
		{
			glm::vec2 vec_1(x1,y1);
			glm::vec2 vec_2(x1,y2);
			list.push_back(y1 +  abs(glm::distance(vec_1 , vec_2)/2.0f));
			list.push_back(x1 -  (2.0f *  KrazyPIXEL));
			_PTRSherrifBay->RotationAngle() = 360.0f;
		}
		break;
	}
	_PTRSherrifBay->set_VectorByIndices(list);
}
//
KrazyTrafficIndicator* KrazyLaneNode::createOnSpotFlipIndicator()
{
	if(!_PTROnspot_flip){
		_PTROnspot_flip = new KrazyLaneNodeTrafficIndicator(this);
	}
	_PTROnspot_flip->Type() = KRAZY_TRAFFIC_INDC_OnSpotFlip;
	return _PTROnspot_flip;
}