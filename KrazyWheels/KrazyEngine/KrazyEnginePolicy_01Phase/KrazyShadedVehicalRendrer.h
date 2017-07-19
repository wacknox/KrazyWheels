#pragma once
#include "KrazyVehicalShades.h"
#include "KrazyProgramme.h"
#include "KrazyVehical.h"
//
static const float _police_light_left[] = {
	-0.01f , -0.025f , 0.0f,
	 0.01f , -0.025f , 0.0f,
	 0.01f ,  0.025f , 0.0f
};
//
static const float _police_light_right[] = {
	 -0.01f ,  -0.025f , 0.0f,
	  0.01f ,   0.025f , 0.0f,
	 -0.01f ,   0.025f , 0.0f
};
static const float _police_light[] = {
	-0.01f , -0.01250f , 0.0f,
	 0.01f , -0.01250f , 0.0f,
	 0.01f ,  0.01250f , 0.0f,
	
	 -0.01f ,  -0.01250f , 0.0f,
	  0.01f ,   0.01250f , 0.0f,
	 -0.01f ,   0.01250f , 0.0f
};
static const float _police_light_normals[]={
	0.0f , 0.0f , 0.5f,
	0.0f , 0.0f , 0.5f,
	0.0f , 0.0f , 0.5f,

	0.0f , 0.0f , 0.5f,
	0.0f , 0.0f , 0.5f,
	0.0f , 0.0f , 0.5f
};
#if 1
static const float _vertex_buffer_data[] = { 
    -0.030f,  -0.075f,  0.0f, 
     0.030f,  -0.075f,  0.0f,  
	 0.0f ,   0.0f,  0.0f, 
	
	 0.0f,    0.0f,  0.0f, 
	 0.030f,  -0.075f,  0.0f, 
	 0.030f,   0.075f,  0.0f,

	 0.030f ,  0.075f,  0.0f, 
	 0.0f  ,  0.0f,  0.0f, 
	-0.030f ,  0.075f,  0.0f,

	-0.030f ,  0.075f,  0.0f,
	 0.0f  ,  0.0f,  0.0f,
	-0.030f  , -0.075f,  0.0f
};
#else
static const float _vertex_buffer_data[] = { 
    -0.04f,  -0.1f,  0.0f, 
     0.04f,  -0.1f,  0.0f,  
	 0.0f ,   0.0f,  0.0f, 
	
	 0.0f,    0.0f,  0.0f, 
	 0.04f,  -0.1f,  0.0f, 
	 0.04f,   0.1f,  0.0f,

	 0.04f ,  0.1f,  0.0f, 
	 0.0f  ,  0.0f,  0.0f, 
	-0.04f ,  0.1f,  0.0f,

	-0.04f ,  0.1f,  0.0f,
	 0.0f  ,  0.0f,  0.0f,
	-0.04  , -0.1f,  0.0f
};
#endif

static const float _vert_normal[] = {
	0.1f , 0.5f , 0.5f , 
	0.1f , 0.5f , 0.5f , 
	0.2f , 0.5f , 0.5f , 
	
	0.5f , 0.5f , 0.5f ,
	0.0f , 0.0f , 0.5f ,
	0.0f , 0.0f , 0.5f ,

	0.0f , 0.0f , 1.0f ,
	1.0f , 1.0f , 0.5f ,
	0.0f , 0.0f , 1.0f ,

	0.0f , 0.0f , 1.0f ,
	1.0f , 1.0f , 0.5f ,
	0.0f , 0.0f , 0.5f
};
//
class KrazyShadedVehicalRendrer
{
public:
	KrazyShadedVehicalRendrer(void);
	virtual ~KrazyShadedVehicalRendrer(void);
	void on_resize(int width , int height);
	void create();
	void render(KrazyVehical* _vehical);
	void reset();
	//
private:
	glm::mat4 _ProjectionMatrix;
	glm::mat4 _TranslateMatrix; 
	glm::mat4 _Rotate;
	glm::mat4 _VehicalScaleMatrix;
	glm::mat4 _WindowScaleMatrix;
	//
	unsigned int  _ProjectVehicalBody;
	unsigned int  _WindowScaleVehicalBody;
	unsigned int  _ModelViewVehicalBody;
	unsigned int  _LightPostionVehicalBody;
	unsigned int  _ModelFragVehicalBody;
	unsigned int  _PositionVehicalBody;
	unsigned int  _NormalVehicalBody;
	unsigned int  _LSPositionVehicalBody;
	unsigned int  _LSAmbientVehicalBody;
	unsigned int  _LSDiffuseVehicalBody; 
	unsigned int  _LSSpecularVehicalBody;
	unsigned int  _MaterialAmbientVehicalBody; 
	unsigned int  _MaterialDiffuseVehicalBody; 
	unsigned int  _MaterialSpecularVehicalBody;
	//
	KrazyProgramme	_ProgrammeVehicalBody;
	KrazyProgramme	_ProgrammeVehicalHeadLights;
	KrazyProgramme	_ProgrammeTailLights;

	int _Width;
	int _Height;
};
//
