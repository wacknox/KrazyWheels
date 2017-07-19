#pragma once
#include "KrazyEntryIndicatorExtractionPolicy_01Phase.h"
#include "KrazyMovingPlayerCarPolicy_01Phase.h"
#include "KrazyRedundentClassDeclarationMacros.h"
//#include "KrazyVehicalRendrer.h"
#include "KrazyShadedVehicalRendrer.h"
// To Do Later take this Enum to different file 
// MPLC .., Meaning Moving Player Car
// MPOC .., Meaning Moving Sherrif Car.,
// EIEX .., Meaning Entry Indicator Extractor..,
enum KRAZY_ALGO_POLICY_ORDER_01Phase
{
   KRAZY_ALGO_POLICY_ORDER_01Phase_Default		  = 0x00000000,
   KRAZY_ALGO_POLICY_ORDER_01Phase_MPLC           = 0x0000000F,
   KRAZY_ALGO_POLICY_ORDER_01Phase_MPOC           = 0x000000F0,
   KRAZY_ALGO_POLICY_ORDER_01Phase_EIEX			  = 0x00000F00,
   KRAZY_ALGO_POLICY_ORDER_01Phase_EIEX_MPLC      = 0x00000F0F,
};
//
struct IKrazyAppCallBack;
class KrazyGameRenderingPolicy_01Phase 
{
	_KRAZY_DATA_POLICY_ENCAPSULATION(KrazyEntryIndicatorExtraction , KrazyEntryIndicatorExtractionPolicy_01Phase)
	_KRAZY_DATA_POLICY_ENCAPSULATION(KrazyMovingPlayerCar          , KrazyMovingPlayerCarPolicy_01Phase)
public:
	KrazyGameRenderingPolicy_01Phase();
	virtual ~KrazyGameRenderingPolicy_01Phase(void);
	
	bool onExecute();
	void reset();
	void reset_rendrer();
	void create_rendrer();
	void on_resize(int width , int height);
	void on_attachappcallback(IKrazyAppCallBack* _callback);
	void render_vehical();
	KrazyShadedVehicalRendrer* get_vehical_rendrer();
	//..,
	KrazyShadedVehicalRendrer					_VehicalRendrer;
	KrazyEntryIndicatorExtraction		     _AlgoEntryIndicatorExtraction;
	KrazyMovingPlayerCar				     _AlgoMovingPlayer;
	KrazyALGODiagnostic						 _AlgoDiagnosticList;
	KRAZY_ALGO_POLICY_ORDER_01Phase			 _CurrentAlgoPolicy;
};
///

