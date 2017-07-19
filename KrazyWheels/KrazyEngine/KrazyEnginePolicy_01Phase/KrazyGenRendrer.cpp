#include "stdafx.h"
#include "KrazyGenRendrer.h"
#include "GLeish.h"
//
KrazyGenRendrer::KrazyGenRendrer() :
 _Projection(0)
,_View(0)
,_Translation(0)
,_Rotation(0)
,_ScreenScale(0)
,_Width(0.0f)
,_Height(0.0f)
{
}
//
KrazyGenRendrer::~KrazyGenRendrer(void)
{
}
//
bool KrazyGenRendrer::initialize()
{
	return onCreateProgramme();
}
//
bool KrazyGenRendrer::onCreateProgramme()
{
	return true;
}


