#pragma once
#include "KrazyLevelCityContextInterface.h"
#include "KrazyVehical.h"

enum KrazyLevelMode
{
	KRAZY_LEVELMODE_gameon        ,
	KRAZY_LEVELMODE_gameover      ,
	KRAZY_LEVELMODE_levelcomplete ,
};
//
enum KrazyLevelThriverState
{
	KRAZY_LEVELPROGRESS_Start_to_Finish ,
	KRAZY_LEVELPROGRESS_Ganguni_Exiting ,
	KRAZY_LEVELPROGRESS_Gangtoona_Exiting ,
	KRAZY_LEVELPROGRESS_Waiting_for_Rangatoona ,
};
//
class KrazyThriving
{
public:
	KrazyThriving();
	~KrazyThriving();
	
	void reset();
    KrazyBool& wait_further();
	KrazyBool& wait_further_for_Rangtoona();
	KrazyVehicalName& vehical_name();

private:
	KrazyBool				_WaitFurther;
	KrazyBool               _WaitFurther_for_Rangatoona;
	KrazyVehicalName		_ExitSequence;
};
//
class KrazyThrivingList : public vector<KrazyThriving*>{};
//..,
class KrazyLevelThriver
{
public:
	static void create_with_level_context(KrazyLevelCityContext* _level_context);
	static KrazyLevelThriver* get_default();
	static void release_default();
	
	//....................................................................
	KrazyLevelCityContext* get_context(){ return _LevelContext; }
	KrazyTrafficIndicator* get_exit();
	//.....................................................................
	void				   on_set_LastPlayer(KrazyVehicalName _vehical_name);
	void                   on_exit_player(KrazyVehical* _vehical);
	void                   on_set_ExitSequence(KrazyIntList& _exit_sequence);
	void				   on_appendThriving(KrazyThriving* _Thriving);
	//... level progression ..,
	KrazyBool onExecute(KrazyVehical* _vehical);

	KrazyLevelMode&			   level_mode();
	string&					   help_tip();
	string                     derrogation();
	string&                    exit_sequencetext();
	void					   reset();
protected:
	KrazyLevelCityContext* _LevelContext;
	KrazyLevelThriver(KrazyLevelCityContext* _level_context);
	KrazyLevelThriver();
	virtual ~KrazyLevelThriver(void);
	//
	void clear_thrivinglist();
	// level progress executer.....
	KrazyBool onExecute_Start_to_Finish(KrazyVehical* _vehical);
	KrazyBool onExecute_Ganguni_Exiting(KrazyVehical* _vehical);
	KrazyBool onExecute_Gangtoona_Exiting(KrazyVehical* _vehical);
	KrazyBool onExecute_Waiting_for_Rangatoona(KrazyVehical* _vehical);
	//
	static KrazyLevelThriver* _Me;
	KrazyTrafficIndicator*    _Exit;
	KrazyLevelMode			  _LevelMode;
	KrazyVehicalName		  _Next_Player_to_Finish;
	KrazyVehicalName          _LastPlayer_to_Finish;
	KrazyLevelThriverState    _ThriverState;
	KrazyVehical*			  _Ganguni;
	string					  _HelpTip;
	string                    _Derrogations;

	unsigned int			 _RangatoonaWaiting;
	KrazyIntList             _ExitSequence;
	KrazyBool				 _IgnoreDefaultWhileRangatoonaExiting;
	KrazyThrivingList	     _ThrivingList;
	string					 _ExitSequenceText;

};

