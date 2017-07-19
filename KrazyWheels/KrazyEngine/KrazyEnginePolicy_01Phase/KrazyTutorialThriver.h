#pragma once 
#include "KrazyEnums.h"
#include "KrazyDutyKueue.h"
#include "KrazyVehical.h"
#include "KrazyTrafficIndicator.h"
#include "KrazyLaneNode.h"
#include <queue>
#include <string.h>
using namespace std;
//..,
class KrazyDoubleLaneRendrer;
class KrazyGameRenderingPolicy_01Phase;
//..
class KrazyPointData 
{
public:
	KrazyPointData() : 
	 _Point(0) ,
	 _Vehical(0)
	{
	}
	//..,
	void on_set_point(KrazyTrafficIndicator* _point) { _Point = _point; }
	void on_set_vehical(KrazyVehical* _vehical) { _Vehical = _vehical; }
	//..,
	KrazyVehical* vehical(){ return _Vehical; }
	KrazyTrafficIndicator* point() { return _Point; }
	//..,
private:
	KrazyTrafficIndicator*  _Point;
	KrazyVehical*		    _Vehical;
	//..,
};
//..,
class KrazyTurnData
{
public:
	KrazyTurnData() : 
	  _Vehical(0) ,
	 _Lane_to_Turn(0)
	{

	}
	//..,
	void on_set_vehical(KrazyVehical* _vehical) { _Vehical = _vehical; }
	void on_set_turninglane(KrazyLaneNode* _lane) { _Lane_to_Turn = _lane; }
	//..,
	KrazyVehical*  vehical(){ return _Vehical; }
	KrazyLaneNode* lane_to_turn() { return _Lane_to_Turn; }
	//..,
private:
	KrazyVehical*		    _Vehical;
	KrazyLaneNode*			_Lane_to_Turn;
};
//..,
class KrazyDistanceData
{
public :
	KrazyDistanceData() : _Distance(0.0f)
	{
	}

	void on_set_vehical(KrazyVehical* _vehical) { _Vehical = _vehical; }
	void on_set_startingvector(glm::vec2& _vec){ _Starting = glm::vec2(_vec.x ,_vec.y);}
	void on_set_distance(float _distance){ _Distance = _distance; }
	//..,
	KrazyVehical* vehical() { return _Vehical; }
	glm::vec2* starting()   { return &_Starting;}
	float distance(){ return _Distance; }
	//..,
private:
	glm::vec2      _Starting;
	float          _Distance;
	KrazyVehical*  _Vehical;

};
//..,
class KrazyTouchData
{
public :
	KrazyTouchData(){}
	void on_set_vehical(KrazyVehical* _vehical) { _Vehical = _vehical; }
	KrazyVehical* vehical() { return _Vehical; }
    //..,
private:
	KrazyVehical*  _Vehical;
};
//..,
class KrazyFlipData
{
public:
	KrazyFlipData() {}
	void on_set_vehical(KrazyVehical* _vehical) { _Vehical = _vehical; }
	KrazyVehical* vehical() { return _Vehical; }
    //..,
private:
	KrazyVehical*  _Vehical;
};
//..,
class KrazyExitData
{
public:
	KrazyExitData(){}
	void on_set_vehical(KrazyVehical* _vehical)  { _Vehical = _vehical; }
	KrazyVehical* vehical() { return _Vehical; }
    //..,
private:
	KrazyVehical*  _Vehical;
};
//..,
enum KrazyTutorialState
{
	KRAZY_TUT_STATE_EngineHelping = 0x00,
	KRAZY_TUT_STATE_SelfTry = 0x01,
};
//..,
enum KrazyTTSequenceId
{
	KRAZY_TTSEQ_Waiting_for_Point			   = 0x0000, 
	KRAZY_TTSEQ_Waiting_for_Touch			   = 0x0001,
	KRAZY_TTSEQ_Waiting_for_Turns			   = 0x0002,
	KRAZY_TTSEQ_Waiting_for_Distance		   = 0x0003,
	KRAZY_TTSEQ_Waiting_for_Flip			   = 0x0004,
	KRAZY_TTSEQ_Waiting_for_Exit			   = 0x0005,
	KRAZY_TTSEQ_Waiting_before_thepoint		   = 0x0006,
	KRAZY_TTSEQ_Waiting_after_vehical_start    = 0x0007,
	
};
//..,
class KrazySequence
{
public:
	KrazySequence(KrazyTTSequenceId _id , KrazyBool _all_vehical_stop) : 
	  _Id(_id)
	 ,_AllVehicalStop(_all_vehical_stop)
	  {}
	virtual ~KrazySequence() {}
	KrazyTTSequenceId seq_id()  { return _Id; }
	KrazyBool all_vehical_stop(){ return _AllVehicalStop; }
	//..,
	void add_to_clearwaitlist(KrazyIntList& _clear_map)
	{
		_ClearWaitList.clear();
		int _count = _clear_map.size();
		for(int i = 0; i < _count; i++){
			_ClearWaitList.push_back(_clear_map[i]);
		}
	}
	//..,
	void add_to_addwaitlist(KrazyIntList& _add_map)
	{
		_AddWaitList.clear();
		int _count = _add_map.size();
		for(int i = 0; i < _count; i++){
			_AddWaitList.push_back(_add_map[i]);
		}
	}
	//..,
	KrazyIntList* get_clearwaitlist() { return &_ClearWaitList; }
	KrazyIntList* get_addwaitlist()   { return &_AddWaitList; }
	//..,
private:
	KrazyTTSequenceId _Id;
	KrazyBool _AllVehicalStop;
	KrazyIntList _ClearWaitList;
	KrazyIntList _AddWaitList;
};
//..,
template<typename SEQ_DATA>
class KrazyTTSequence : public KrazySequence{

public:
	KrazyTTSequence(KrazyTTSequenceId _id , KrazyBool _all_vehical_stop) : KrazySequence(_id , _all_vehical_stop)
	{
	}
	virtual ~KrazyTTSequence()
	{
		delete _Data;_Data = 0;
	}

	SEQ_DATA* data(){ return _Data; }
	//..,
protected:
	SEQ_DATA* _Data;

};
//..,
class KrazyTTSequencePoint : public KrazyTTSequence<KrazyPointData>
{
public : 
	KrazyTTSequencePoint(KrazyBool _all_vehical_stop) : KrazyTTSequence(KRAZY_TTSEQ_Waiting_for_Point , _all_vehical_stop) {}
	virtual ~KrazyTTSequencePoint(){ delete _Data;_Data = 0; }
	void on_attach_pointdata(KrazyPointData* _point_data) { _Data = _point_data; }

};
//..,
class KrazyTTSequenceTouch : public KrazyTTSequence<KrazyTouchData>
{
public : 
	KrazyTTSequenceTouch(KrazyBool _all_vehical_stop) : KrazyTTSequence(KRAZY_TTSEQ_Waiting_for_Touch , _all_vehical_stop) {}
	virtual ~KrazyTTSequenceTouch(){}
	void on_attach_touchdata(KrazyTouchData* _touch_data) { _Data = _touch_data; }

};
//..,
class KrazyTTSequenceTurns : public KrazyTTSequence<KrazyTurnData>
{
public : 
	KrazyTTSequenceTurns(KrazyBool _all_vehical_stop) : KrazyTTSequence(KRAZY_TTSEQ_Waiting_for_Turns , _all_vehical_stop) {}
	virtual ~KrazyTTSequenceTurns(){}
	void on_attach_turndata(KrazyTurnData* _turn_data) { _Data = _turn_data; }

};
//..,
class KrazyTTSequenceDistance : public KrazyTTSequence<KrazyDistanceData>
{
public : 
	KrazyTTSequenceDistance(KrazyBool _all_vehical_stop) : KrazyTTSequence(KRAZY_TTSEQ_Waiting_for_Distance , _all_vehical_stop) {}
	virtual ~KrazyTTSequenceDistance(){}
	void on_attach_distancedata(KrazyDistanceData* _distance_data) { _Data = _distance_data; }
};
//..,
class KrazyTTSequenceFlip : public KrazyTTSequence<KrazyFlipData>
{
public : 
	KrazyTTSequenceFlip(KrazyBool _all_vehical_stop) : KrazyTTSequence(KRAZY_TTSEQ_Waiting_for_Flip , _all_vehical_stop) {}
	virtual ~KrazyTTSequenceFlip(){}
	void on_attach_flipdata(KrazyFlipData* _flip_data){ _Data = _flip_data;}

};
//..,
class KrazyTTSequenceExit : public KrazyTTSequence<KrazyExitData>
{
public:
	KrazyTTSequenceExit(KrazyBool _all_vehical_stop) :  KrazyTTSequence(KRAZY_TTSEQ_Waiting_for_Exit , _all_vehical_stop) {}
	virtual ~KrazyTTSequenceExit(){}
	void on_attach_exitdata(KrazyExitData* _exit_data){ _Data = _exit_data; }
};
//..,
class KrazySeqKueue : public queue<KrazySequence*>{};
class KrazySeqList  : public vector<KrazySequence*>{};
//..,
 class KrazyTutorialThriver 
{
private:
	KrazyTutorialThriver();
	~KrazyTutorialThriver();
	void init();
	void destroy();
	//..,
public:
	static KrazyTutorialThriver* get_default();
	static void release_default();
	void on_point(KrazyVehical* _vehical , KrazyTrafficIndicator* _point);
	void on_turned(KrazyVehical* _vehical);
	void on_moved(KrazyVehical* _vehical);
	bool on_touched(KrazyVehical* _vehical);
	bool on_reflection_touched(KrazyVehical* _vehical);
	void on_exit(KrazyVehical* _vehical);
	void on_waiting_before_point(KrazyVehical* _vehical);
	void on_waiting_after_vehicle_start(KrazyVehical* _vehical);
	void execute();
	void reset();
	//..,
	void on_attach_doublelanerendrer(KrazyDoubleLaneRendrer* _double_lane_rendrer);
	void on_attach_gamerendrer(KrazyGameRenderingPolicy_01Phase* _game_rendrer);
	void on_set_level();
	//..,
	KrazyBool& mode();
	KrazyBool  all_vehical_stopped();
	int& level();
	//..,
	bool should_wait(KrazyVehical* _vehical);
private:
	void clean_for_thriving();
	void on_set_currentseq();
	void on_set_data_for_level();
	void on_add_to_waitlist(KrazySequence* _seq);
	void on_clear_waitlist(KrazySequence* _seq);
	void clean_seq_kueue();
	void clean_seq_list();
	void erase_from_seq_list(KrazySequence* _seq);
	KrazyLaneNode* get_Lane(int _parent_index , int _kid_index);
	KrazyTrafficIndicator* get_Point(int _indc_index , KrazyLaneNode* _lane);
	KrazyVehical* get_Vehicle(KrazyVehicalName _vehicle_name);
	//..,
	void on_level_1();
	void on_level_2();
	void on_level_6();
	void on_level_8();
	void on_level_9();
	void on_level_11();
	void on_level_15(); 

private:
   //..
	KrazyBool						   _AllVehicalStopped;
	string							   _XML;
	int                                _Level;
	KrazySequence*                     _CurrentSequence; 
	KrazyBool						   _Mode;
	KrazySeqKueue					   _SeqKueue;
	KrazyTutorialState				   _CurrentState;
	KrazyDoubleLaneRendrer*			   _DoubleLaneRendrer;
	KrazyGameRenderingPolicy_01Phase*  _GameRendrer;
	__KrazyVehicalList				   _VehicalWaitList;
	KrazySeqList					   _SeqList;
	//..,
	static KrazyTutorialThriver*	_Me;
};