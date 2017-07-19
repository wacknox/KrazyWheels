#pragma once
class KrazyShader
{
public:
	KrazyShader(unsigned int shaderType , const char* pSource);
	KrazyShader();
	virtual ~KrazyShader(void);

public:
	 void LoadShader(unsigned int uShaderType , const char* szSource);
	 void release();
	 operator unsigned int() {return _Shader;}
	 unsigned operator = (unsigned int _new_shader){ _Shader = _new_shader;} 

private:
	  unsigned int _Shader;
	  bool         _Created;
};

