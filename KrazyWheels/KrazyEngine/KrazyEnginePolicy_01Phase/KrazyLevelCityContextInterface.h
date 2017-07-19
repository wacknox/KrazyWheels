#pragma once
#include "KrazyPrimitiveDataCollections.h"
#include "KrazyTrafficIndicator.h"
//
#define _ENABLE_SPLIT_SCALED_LANE
//
class KrazyLevelCityContext
{
public:
	KrazyLevelCityContext();
	virtual ~KrazyLevelCityContext();
	//...,
	 string getXML(){return _XML;}
#ifdef _ENABLE_SPLIT_SCALED_LANE
	 KrazyFloatList* getHorzsVertexList() { return &_HorzsRoadBuffer; }
	 KrazyFloatList* getHorzsBorderList() { return &_HorzsBorderBuffer; }
	 KrazyFloatList* getVertsVertexList() { return &_VertsRoadBuffer; }
	 KrazyFloatList* getVertsBorderList() { return &_VertsBorderBuffer; }
	 //..,
	 void onSetHorzsVertexList( KrazyFloatList* _list) ;
	 void onSetHorzsBorderList( KrazyFloatList* _list) ;
	 void onSetVertsVertexList( KrazyFloatList* _list) ;
	 void onSetVertsBorderList( KrazyFloatList* _list) ;
#else
	 KrazyFloatList* getVertexList() { return &_RoadBuffer; }
	 KrazyFloatList* getBorderList() { return &_BorderBuffer; }
#endif
	 KrazyTrafficIndicatorList* getBlockingTI(){ return &_BlockingTI; }
	 void on_set_blockingTI(KrazyTrafficIndicatorList* _list);
	 void on_refresh(int level);
	 void on_refresh_level_onli(int level);
	 void release() { delete this;}
	 
     //..,
protected:
	void clean_up();
	void setLayoutVertices();
#ifdef _ENABLE_SPLIT_SCALED_LANE
	KrazyFloatList					_HorzsRoadBuffer;
	KrazyFloatList					_HorzsBorderBuffer;
	KrazyFloatList					_VertsRoadBuffer;
	KrazyFloatList					_VertsBorderBuffer;
#else
	KrazyFloatList					_RoadBuffer;
	KrazyFloatList					_BorderBuffer;
#endif
	KrazyFloatList					_RoadBuffer;
	KrazyFloatList					_BorderBuffer;
	KrazyTrafficIndicatorList		_BlockingTI;
	string                          _XML;
private:
	void onLevel_1();
	void onLevel_2();
	void onLevel_3();
	void onLevel_4();
	void onLevel_5();
	void onLevel_6();
	void onLevel_7();
#if 0
	void onLevel_8();
	void onLevel_9();
	void onLevel_10();
	void onLevel_11();
	void onLevel_12();
	void onLevel_13();
	void onLevel_14();
	void onLevel_15();
	void onLevel_16();
#endif
};