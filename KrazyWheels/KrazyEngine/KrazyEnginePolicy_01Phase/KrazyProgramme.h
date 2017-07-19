#pragma once
#include "KrazyShader.h"
////////////////////////////
class KrazyProgramme
{
public:
	KrazyProgramme(void);
	virtual ~KrazyProgramme(void);
	bool create(KrazyShader& shader , KrazyShader& fragmentShader);
	void reset();
	bool compile();
	operator unsigned int(){ return _Programme;}
private:
	unsigned int		_Programme;
	unsigned int        _VertexShader;
	unsigned int        _FragmentShader;
	bool                _Created;
};

