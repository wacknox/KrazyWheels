#pragma once
#include "KrazyProgramme.h"
#include "KrazyShader.h"
/////////////////////////////////////////////////
class KrazyGenRendrer
{
public:
	KrazyGenRendrer();
	virtual bool initialize();
	virtual bool onCreateProgramme();
	~KrazyGenRendrer(void);
protected:
	KrazyProgramme		_Programme;
	unsigned int 		_Position;
	unsigned int		_Color;
	unsigned int        _Projection;
	unsigned int        _View;
	unsigned int        _Translation;
	unsigned int        _Rotation;
	unsigned int        _ScreenScale;
	
	float				_Width;
	float				_Height;
};
////////////////////////////////////////////////////



