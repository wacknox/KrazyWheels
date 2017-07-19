/*
 * CDoubleLaneRoadRendrer.h
 *
 *  Created on: Dec 14, 2013
 *      Author: Siddharth
      Edited and Renamed :  KrazyDoubleLaneRendrer.h Nov - 2014..,
 */
#ifndef DOUBLELANEROADRENDRER_H_
#define DOUBLELANEROADRENDRER_H_
#include "KrazyGenRendrer.h"
#include "KrazyShader.h"
#include "KrazyProgramme.h"
#include "KrazyLevelCityContextInterface.h"
#ifdef _IS_FOR_SIMONE
class KrazyDisplayTrafficIndicators;
class KrazyTrafficIndicatorList;
#endif
static const float _g_BlockingIndicator[] = {
	0.02f , 0.02f , 0.0f,
   -0.02f , 0.02f , 0.0f,
   -0.02f ,-0.02f , 0.0f,

   -0.02f ,-0.02f , 0.0f,
    0.02f ,-0.02f , 0.0f,
    0.02f , 0.02f , 0.0f
};
static const unsigned int _g_BlockingIndicators = 18;
//
static const float _g_SherrifBay[] = {
	0.1f , 0.15f , 0.0f,
   -0.1f , 0.15f , 0.0f,
   -0.1f ,-0.15f , 0.0f,

   -0.1f ,-0.15f , 0.0f,
    0.1f ,-0.15f , 0.0f,
    0.1f , 0.15f , 0.0f
};
static const unsigned int _g_SherrifBay_s_= 18;
//
class KrazyDoubleLaneRendrer : public KrazyGenRendrer
{
public:
	KrazyDoubleLaneRendrer();
	~KrazyDoubleLaneRendrer();
	void onRefresh(KrazyLevelCityContext* city_context);
	bool render();
	void reset();
	virtual bool		onCreateProgramme();
	//
	void on_resize();
private:

	void cleanBuffers();
	//..,
#ifdef _ENABLE_SPLIT_SCALED_LANE
	unsigned int       _VBOHorzsLaneVertices;
	unsigned int       _VBOHorzsBorderVertices;
	unsigned int       _HorzsVertices;
	unsigned int       _HorzsBorders;
//...,
	unsigned int       _VBOVertsLaneVertices;
	unsigned int       _VBOVertsBorderVertices;
	unsigned int       _VertsVertices;
	unsigned int       _VertsBorders;
#else
	unsigned int       _VBODoubleLaneVertices;
	unsigned int       _VBOBorderVertices;
	unsigned int       _Vertices;
	unsigned int       _Borders;
#endif
	unsigned int	  _AmbientColor;
	unsigned int      _VBOBlockingIndicator;
	unsigned int      _VBOSherrifBay;
	KrazyProgramme    _ProgrammeTI;
	unsigned int      _PositionTI;
	unsigned int      _ProjectionTI;
	unsigned int      _ModelViewTI;
	unsigned int      _ScreenScaleTI;
	unsigned int      _ColorTI;

	KrazyFloatList					_HorzsRoadBuffer;
	KrazyFloatList					_HorzsBorderBuffer;
	KrazyFloatList					_VertsRoadBuffer;
	KrazyFloatList					_VertsBorderBuffer;

	glm::mat4 _ProjectionMatrix;
	glm::mat4 _WindowScaleMatrix;	
	KrazyTrafficIndicatorList*  _PTRBlockingTrafficIndicators;
	bool _created;
	bool _buffer_is_there;
};
#endif /* DOUBLELANEROADRENDRER_H_ */
