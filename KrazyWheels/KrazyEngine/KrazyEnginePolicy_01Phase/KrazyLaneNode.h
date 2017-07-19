#pragma once
#include "KrazyPrimitiveDataCollections.h"
#include "KrazyTrafficIndicator.h"
#include "KrazyVehical.h"
#include "KrazyRedundentClassDeclarationMacros.h"
//
// deletion with release
enum KrazyLaneType
{
  KRAZY_LANE_Type_Default  = 0x00,
  KRAZY_LANE_Type_Single   = 0x0F,
  KRAZY_LANE_Type_Double   = 0xF0,
  
};
//
enum KrazyLaneDirection
{
	KRAZY_LANE_DIRECTION_DEFAULT      = 0x0000,
	KRAZY_LANE_DIRECTION_HORZS	      = 0x000F,
	KRAZY_LANE_DIRECTION_VERTS	      = 0x00F0,
	KRAZY_LANE_DIRECTION_UP		      = 0x0F00, 
	KRAZY_LANE_DIRECTION_DOWN		  = 0xF000,
	KRAZY_LANE_DIRECTION_UP_HORZS     = 0x0F0F,
	KRAZY_LANE_DIRECTION_DOWN_HORZS   = 0xF00F,
	KRAZY_LANE_DIRECTION_UP_VERTS     = 0x0FF0,
	KRAZY_LANE_DIRECTION_DOWN_VERTS   = 0xF0F0,
};
//
struct KrazyShortPathRoutingIndexer
	{
		KrazyShortPathRoutingIndexer(){ reset(); }
		bool initialized(){ return _Initialized; }
		void initialize(int size , KrazyLaneNode* ptr_prev){ _Initialized = true;_Size = size;_PTRPrev = ptr_prev; }
		KrazyLaneNode* getPrev(){ return _PTRPrev; }
		int& getCursor(){ return _Cursor; }
		int  getSize()  { return _Size; }
		bool exhausted(){ return !(_Size > 0 && _Cursor < _Size); }
		bool& skip(){ return _Skip; }
		bool& processing(){ return _Processing; }
		void reset()
		{
			_Cursor     = 0;
		    _Size       = 0;
		   _PTRPrev     = 0;
		   _Initialized = false;
		   _Skip        = false;
		   _Processing  = false;
		}
	private:
		bool _Initialized;
		int  _Cursor;
		int  _Size;
		KrazyLaneNode* _PTRPrev;
		bool  _Skip;
		bool  _Processing;
};
//
class KrazyLaneProperties
{
public:
	KrazyLaneProperties(KrazyLaneType lane_type , KrazyLaneDirection krazy_lane_direction ,  float x_0 = 0.0f , float y_0 = 0.0f ,float x_1 = 0.0f , float y_1= 0.0f,float px1 = 0.0f,float py1 = 0.0f) ;
	KrazyLaneProperties() ;
	KrazyLaneProperties(const KrazyLaneProperties& properties);
	KrazyLaneProperties& operator=(const KrazyLaneProperties& properties);
public:
	void set_LaneType(KrazyLaneType laneType){_LaneType = laneType;}
	void set_LaneDirection(KrazyLaneDirection laneDirection){ _LaneDirection = laneDirection; }
	void set_Rect(float x1 , float y1 , float x2 , float y2)
	{ 
		_x1 = x1;_y1 = y1;_x2 = x2;_y2 = y2;
	}
	void set_Point(float x1 , float y2){_px1 = x1;_py1 = y2;}
	void set_RectByIndices(KrazyFloatList& indices_f);
	void set_PointByIndices(KrazyFloatList& indices_f);
	void set_BorderCoords(KrazyFloatList& coords);
	KrazyFloatList& get_BorderCoords(){ return _BorderCoords; }
	KrazyFloatList& get_VertexCoords(){ return _VertexCoords; }
	KrazyLaneType	   get_LaneType(){return _LaneType;}
	KrazyLaneDirection get_LaneDirection(){ return _LaneDirection;}
	void			   get_Rect(float& x1 , float&y1 , float& x2 , float& y2){ x1 = _x1;y1 = _y1;x2 = _x2;y2 = _y2;}
	void			   get_Point(float& x1 , float&y1){x1 = _px1;y1 = _py1;}
protected:
	void reset(){}
	void copy(const KrazyLaneProperties& properties)
	{
		
		_LaneType = properties._LaneType;
		_LaneDirection = properties._LaneDirection;
		_x1  = properties._x1;
		_y1  = properties._y1;
		_x2  = properties._x2;
		_y2  = properties._y2;
		_px1 = properties._px1;
		_py1 = properties._py1;
		_BorderCoords.clear();
		_VertexCoords.clear();
		int count = properties._BorderCoords.size();
		for(int i = 0; i < count; i++)
		{
			_BorderCoords.push_back(properties._BorderCoords[i]);
		}
		count = properties._VertexCoords.size();
		for(int i = 0; i < count; i++)
		{
			_VertexCoords.push_back(properties._VertexCoords[i]);
		}

	}
	KrazyLaneType				_LaneType;
	KrazyLaneDirection			_LaneDirection;
	KrazyFloatList				_VertexCoords;
	KrazyFloatList              _BorderCoords;
	float _x1,_y1,_x2,_y2;
	float _px1 , _py1;
};
//
class KrazyLaneNode;
class KrazyLaneNodeList : public vector<KrazyLaneNode*>{}; 
class KrazyVehicalList  : public vector<KrazyVehical*> {};
//
class KrazyLaneNode 
{
public:
	KrazyLaneNode();
	KrazyLaneNode(KrazyLaneNode* parent);
	virtual~KrazyLaneNode();
public:
	// Traffic Indicator..,
	void on_resize();
	KrazyTrafficIndicator*     createTrafficIndicator();
	KrazyTrafficIndicatorList* get_TrafficIndicators(){ return &_TrafficIndicators; }
	int get_TrafficIndicatorCount(){ return _TrafficIndicators.size(); }
	KrazyTrafficIndicator* get_TrafficIndicator(int index){ return _TrafficIndicators[index];}
	// For Single Lane..,
	KrazyTrafficIndicator* createOnSpotFlipIndicator();
	KrazyLaneNode* getParent(){ return _PTRParent;}
	void on_assign_sherrif(KrazyVehical* _sherrif){ _PTRSherrif = _sherrif; }
	KrazyVehical* get_Sherrif(){ return _PTRSherrif; }
	KrazyTrafficIndicator* get_slider_point();
protected:
	KrazyVehical* _PTRSherrif;
public:
	KrazyVehicalList* getVehicalList()
	{ 
		return &_Vehicals; 
	}
	void on_attachVehical(KrazyVehical* ptr_vehical)
	{ 
		_Vehicals.push_back(ptr_vehical);
	}
	void on_detachVehical(KrazyVehical* ptr_vehical)
	{
		int count = _Vehicals.size();
		for(int i = 0; i < count; i++){
			KrazyVehical* ptr_match = _Vehicals[i];
			if(ptr_match == ptr_vehical){
				for(int j = i; j < count - 1; j++){
					_Vehicals[j] = _Vehicals[j+1];
				}
				_Vehicals.pop_back();
				break;
			}
		}
	}
	void on_attachLastVehical(KrazyVehical* ptr_vehical);
	void on_detachLastVehical(KrazyVehical* ptr_vehical);
	KrazyVehical*  get_LastVehical();
	void sort_allTrafficIndicators();
	void setFlipTrafficIndicator(KrazyFloatList&);
	KrazyTrafficIndicator* getFlipTrafficIndicator();
	void createSherrifBay();
	KrazyTrafficIndicator* get_sherrifBay(){ return _PTRSherrifBay; }
	KrazyTrafficIndicatorList* getDestinationIndicators(){ return &_DestinationIndicators; }
	KrazyLaneNode* getOppositeSingleLane() 
	{
		for(int i = 0; i < 2; i++){
			KrazyLaneNode* ptr_opposite_lane = _PTRParent->_Kids[i];
			if(ptr_opposite_lane != this)
			{
				return ptr_opposite_lane;
			}
		}
		return 0;
		 
	}
	KrazyTrafficIndicator* getEntryIndicator(){ return _PTRLaneEntryIndicator;}
	void setSearchOrigin(KrazyTrafficIndicator* ptr_source_indicator){_PTRSearchOrigin = ptr_source_indicator;}
	KrazyTrafficIndicator* getSearchOrigin() { return _PTRSearchOrigin; }
	void setDestinationEnd(KrazyTrafficIndicator* ptr_destination_indicator){_PTRDestinationEnd = ptr_destination_indicator;}
	KrazyTrafficIndicator* getDestinationEnd() { return _PTRDestinationEnd; }
	void setNextTrafficIndicatorIndex(int index){ _CurrentTrafficIndicatorIndex = index; }
	int getNextTrafficIndicatorIndex(int index){ return _CurrentTrafficIndicatorIndex; }
	//for double lanes onli
	KrazyLaneNode* createKid();
	KrazyTrafficIndicator* createDestinationIndicator();
	void appendConnectedNode(KrazyLaneNode* node){ _ConnectedLaneNodes.push_back(node);}
	KrazyLaneNodeList* getConnectedLanes(){ return &_ConnectedLaneNodes;}
	int	getKidCount(){return _Kids.size();}
	KrazyLaneNode* getLane(int index){return _Kids[index];}
	KrazyLaneNode* getLaneByDirection(int single_lane_direction);
	KrazyBool  foundSingleLane(KrazyLaneNode* node);
	KrazyShortPathRoutingIndexer* getRoutingIndexer(){ return &_RoutingIndexer; }
	//properties for Network node..,
	bool    isParent()   {return false;}
	bool    hasChildren(){return false;}
	KrazyLaneProperties* getProperties(){ return &_Properties; }
	void setProperties(KrazyLaneProperties& properties){*((KrazyLaneProperties*)&_Properties) = properties;}
private:
	void cleanUp();
	//
	 KRAZY_DATA_ENCAPSULATION(KrazyLaneNodeProperties        , KrazyLaneProperties)
	_KRAZY_DATA_ENCAPSULATION_PARAM_INIT_(KrazyLaneNodeTrafficIndicator  , KrazyTrafficIndicator , KrazyLaneNode)
    //
	KrazyVehicalList								_Vehicals;
	int												_VehicalCount;
	KrazyShortPathRoutingIndexer				    _RoutingIndexer;
	KrazyTrafficIndicator*							_PTRSearchOrigin;
	KrazyTrafficIndicator*							_PTRDestinationEnd;
	KrazyLaneNodeTrafficIndicator*					_PTRFlipTrafficIndicator;
    KrazyLaneNodeTrafficIndicator*				    _PTRLaneEntryIndicator;
	KrazyLaneNodeProperties						    _Properties;
	KrazyTrafficIndicatorList						_TrafficIndicators;
	KrazyTrafficIndicatorList						_OnSpotFlipIndicators;
	KrazyLaneNode*                                  _PTRParent;
	KrazyLaneNodeList								_Kids;
	KrazyLaneNodeList								_ConnectedLaneNodes;// onli for parent;
	int												_CurrentTrafficIndicatorIndex;
	KrazyVehical*									_PTRLastEnteredVehical;
	KrazyTrafficIndicatorList						_DestinationIndicators;
	bool											_FlipisAddedtoTrafficIndc;
	KrazyLaneNodeTrafficIndicator*					_PTRSherrifBay;
	KrazyLaneNodeTrafficIndicator*					_PTRSliderPoint;
	KrazyLaneNodeTrafficIndicator*					_PTROnspot_flip;
};

