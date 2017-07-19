#pragma once
#include "KrazyPrimitiveDataCollections.h"
#include "Vectors.h"
#include "KrazyVehicalShades.h"
#include "KrazyGLMInclusion.h"
#include "KrazyEnums.h"
//
// deletion with release
class KrazyLaneNode;
class KrazyVehical;
enum KrazyTurnIndicatorType 
{
	KRAZY_TRAFFIC_INDC_Default = 0x00000000,
	KRAZY_TRAFFIC_INDC_Turn ,
	KRAZY_TRAFFIC_INDC_Dest  ,
	KRAZY_TRAFFIC_INDC_Entry ,
	KRAZY_TRAFFIC_INDC_Exit  ,
	KRAZY_TRAFFIC_INDC_FlipPoint ,
	KRAZY_TRAFFIC_INDC_EntryPoint ,
	KRAZY_TRAFFIC_INDC_NoEntry ,
	KRAZY_TRAFFIC_INDC_Wait ,
	KRAZY_TRAFFIC_INDC_SherrifBay ,
	KRAZY_TRAFFIC_INDC_OnSpotFlip ,
	KRAZY_TRAFFIC_INDC_SliderPoint,
};
//
enum KrazyVehicalWaiting
{
	KRAZY_VEHICAL_WAITING_Default ,
	KRAZY_VEHICAL_WAITING_Start,
	KRAZY_VEHICAL_WAITING_End,
};
//
class KrazyEntranceContext
{
public:
	void on_setContextList(KrazyIntList& type_list, KrazyListToFloatList& color_list)
	{	
		int count_color_list = color_list.size();
		int count_type_list  = type_list.size();
		if(count_type_list != count_color_list)
			return;

		_VehicalTypeList.clear();
		_VehicalColorList.clear();
	
		for(int i = 0; i < count_type_list; i++){
			_VehicalTypeList.push_back(type_list[i]);
			int index = _VehicalColorList.size();
			_VehicalColorList.push_back(KrazyFloatList());
			KrazyFloatList* ptr_color_list_s = &color_list[i];
			KrazyFloatList* ptr_color_list_d = &_VehicalColorList[index];
			int s_list_count = ptr_color_list_s->size();
			for(int elem = 0; s_list_count == 4 && elem < s_list_count; elem++){
				ptr_color_list_d->push_back(ptr_color_list_s->at(elem));
			}
		}
	}
	void on_setTypeList(KrazyIntList& list)
	{
		int count = list.size();
		for(int i = 0; i < count; i++){
			_VehicalTypeList.push_back(list[i]);
		}
	}
	void on_setNameList(KrazyIntList& list)
	{
		int count = list.size();
		for(int i = 0; i < count; i++)
		{
			_VehicalNameList.push_back(list[i]);
		}
	}
	void on_setShadeList(KrazyIntList& list)
	{
		int count = list.size();
		for(int i = 0; i < count; i++){
			_VehicalShadeList.push_back(list[i]);
		}
	}
	void on_setShadeTypeList(KrazyIntList& list)
	{
		int count = list.size();
		for(int i = 0; i < count; i++)
		{
			_VehicalShadeTypeList.push_back(list[i]);
		}
	}
	KrazyIntList* getTypeList(){ return &_VehicalTypeList; }
	KrazyIntList* getNameList(){ return &_VehicalNameList; }
	KrazyIntList* getShadeList(){ return &_VehicalShadeList; }
	KrazyIntList* getShadeTypeList(){ return &_VehicalShadeTypeList; }
	void getContextList(KrazyIntList** ptr_type_list, KrazyListToFloatList** ptr_color_list)
	{
	    *ptr_type_list = &_VehicalTypeList;*ptr_color_list = &_VehicalColorList;
	}
private:
	KrazyIntList           _VehicalTypeList;
	KrazyListToFloatList  _VehicalColorList;
	KrazyIntList		  _VehicalNameList;
	KrazyIntList          _VehicalShadeList;
	KrazyIntList          _VehicalShadeTypeList;
};
//
class KrazyTrafficIndicator
{ 
public:
	unsigned int&				Type();
	unsigned int				index();
	KrazyLaneNode*				PTRParentLaneNode();
	//
	glm::vec2*					Vector();
	//
	float&						RotationAngle();	// remove
	int&						ParentIndex();
	void                set_Index(int index);
	void                set_ParentLaneNode(KrazyLaneNode* node);
	void				set_VectorByIndices(KrazyFloatList& indices_f);
	void				set_RectByIndices(KrazyFloatList& indices_f);
	void				set_Source(KrazyTrafficIndicator* source);
	void				set_assignedLanes_for_Sherrif(KrazyIntList& list);
	KrazyIntList*       get_assignedLanes_for_Sherrif();
	void								   add_DestinationIndc(KrazyTrafficIndicator* traffic_indc);
	KrazyTrafficIndicator*                 get_Source();
	KrazyEntranceContext*				   get_EntranceContext();
	int									   get_DestinationIndcCount();
	KrazyTrafficIndicator*                 get_DestinationIndc(int index);
	bool								   is_alreadyTaken();	
	void                                   updateTaken();
	void								   on_resize();
	void								   set_lights();
	void								   swap_lights();
	float*                                 get_CurrentLight();
	int&								   wait();
	void                                   set_WaitingVehical(KrazyVehical* vehical);
	KrazyVehical*						   waiting_vehical();
	KrazyVehicalWaiting&                   waiting_vehical_state();
	KrazyBool							   is_green();
	void								   on_reset(glm::vec2& vec , KrazyLaneNode* curr_lane);

	void								   on_set_bay(glm::vec2& vec , KrazyTrafficIndicator* bay);
	KrazyTrafficIndicator*				   get_bay();
protected :
	KrazyTrafficIndicator();
	KrazyTrafficIndicator(KrazyLaneNode* parent);
	virtual ~KrazyTrafficIndicator();
	void reset(){}
	void cleanUp();
	unsigned int					_Type;
	unsigned int                    _Index;
	KrazyLaneNode*					_PTRParentLaneNode;
	KrazyTrafficIndicator*			_PTRSource;
	KrazyTrafficIndicator*			_PTRBay;
	glm::vec2						_Vector;
	float							_RotationAngle;
	int								_ParentIndex;
	vector<KrazyTrafficIndicator*> _DestinationIndcList;
	KrazyEntranceContext*          _EntanceContext;
	bool						   _Taken;
	KrazyFloatList                 _GreenLight;
	KrazyFloatList				   _YellowLight;
	float* 						   _CurrentLight;
	int							   _Wait;
	KrazyVehical*                  _PTRWaitingVehical;
	KrazyBool					   _Green;	
	KrazyVehicalWaiting            _VehicalWaiting;
	KrazyIntList				   _AssignedLaneList_for_Sherrif;
};
//
class KrazyTrafficIndicatorList : public vector<KrazyTrafficIndicator*>{};

