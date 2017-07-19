#include "StdAfx.h"
#include "KrazyShader.h"
#include "GLeish.h"
#include <malloc.h>
//
KrazyShader::KrazyShader(unsigned int shaderType , const char* szSource) : 
_Shader(0)
{
	LoadShader(shaderType , szSource);
}
//
KrazyShader::KrazyShader() : 
_Shader(0)
,_Created(false)
{
	
}
//
KrazyShader::~KrazyShader(void)
{
	release();
}
//
void KrazyShader::LoadShader(unsigned int uShaderType , const char* szSource)
{
	_Shader = glCreateShader(uShaderType);
	if (_Shader){
		_Created = true;
		glShaderSource(_Shader, 1, &szSource, 0);
	    glCompileShader(_Shader);
	    GLint compiled = 0;
	    glGetShaderiv(_Shader, GL_COMPILE_STATUS, &compiled);
	    if (!compiled){
			GLint infoLen = 0;
	        glGetShaderiv(_Shader, GL_INFO_LOG_LENGTH, &infoLen);
	        if (infoLen){
				char* buf = (char*) malloc(infoLen);
	            if (buf) {
	                glGetShaderInfoLog(_Shader, infoLen, 0, buf);
	                free(buf);
	            }
	           release();
	        }
	    }
	}
}
//
void KrazyShader::release()
{
	if(_Created){
		_Created = false;
	    glDeleteShader(_Shader);
	    _Shader = 0;
	}
}
//
