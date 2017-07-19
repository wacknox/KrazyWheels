#include "StdAfx.h"
#include "KrazyProgramme.h"
#include "KrazyShader.h"
#include "KrazyError.h"
#include "GLeish.h"
//
KrazyProgramme::KrazyProgramme() :
  _Programme(0)
 ,_VertexShader(0)
 ,_FragmentShader(0)
 ,_Created(false)
{

}
//
bool KrazyProgramme::create(KrazyShader& shader , KrazyShader& fragmentShader)
{
	_VertexShader   = shader;
    _FragmentShader = fragmentShader;
	_Programme = glCreateProgram();
	glAttachShader(_Programme , _VertexShader);
	glAttachShader(_Programme , _FragmentShader);
	_Created = true;
	return true;
}
//
void KrazyProgramme::reset()
{
	if(_Created){
		_Created = false;
		glDetachShader(_Programme ,_VertexShader );                                   
		glDetachShader(_Programme , _FragmentShader);
		glDeleteProgram(_Programme);_Programme = 0;
		GLint infoLen = 0;
	    //.., 
		glGetShaderiv(_VertexShader, GL_DELETE_STATUS, &infoLen);
		glGetShaderiv(_FragmentShader, GL_DELETE_STATUS, &infoLen);
		//..,
		glDeleteShader(_VertexShader);_VertexShader = 0;
		glDeleteShader(_FragmentShader);_FragmentShader = 0;
	}

}
//
bool KrazyProgramme::compile()
{
	glLinkProgram(_Programme);
	return true;
}
//
KrazyProgramme::~KrazyProgramme(void)
{
	reset();
}
