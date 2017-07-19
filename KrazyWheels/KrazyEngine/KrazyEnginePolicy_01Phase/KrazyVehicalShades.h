#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\mat4x4.hpp>
#include <glm\mat3x3.hpp>
#include <glm\vec3.hpp>
#include <glm\vec2.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <vector>
using namespace std;
//
enum KrazyShade
{
	KrazyShade_Default			= 0x00000000,
	KrazyShade_Red				= KrazyShade_Default + 1,
	KrazyShade_Blue				= KrazyShade_Default + 2,
	KrazyShade_Green			= KrazyShade_Default + 3,
	KrazyShade_Yellow			= KrazyShade_Default + 4,
	KrazyShade_Orange			= KrazyShade_Default + 5,
	KrazyShade_Pink				= KrazyShade_Default + 6,
	KrazyShade_Brown			= KrazyShade_Default + 7,
	KrazyShade_Purple			= KrazyShade_Default + 8,
	KrazyShade_Turquoisblue     = KrazyShade_Default + 9,
	KrazyShade_Sherrif			= KrazyShade_Default + 10
};
//
enum KrazyVehicalShaderType
{
	KRAZY_SHADER_Default = 0x0,
	KRAZY_SHADER_Normal  = 0x0,
	KRAZY_SHADER_Sherrif = 0x1
};
//
class KrazyVehicalShade
{
public:
	KrazyVehicalShade()
	{}
	KrazyVehicalShade(
		glm::vec4  LSPosition, glm::vec4  LSAmbient,
		glm::vec4  LSDiffuse , glm::vec4  LSSpecular,
		glm::vec4  MaterialAmbient,	glm::vec4  MaterialDiffuse, 
		glm::vec4  MaterialSpecular
		) : 
	 _LSPosition(LSPosition)
	,_LSAmbient(LSAmbient)
	,_LSDiffuse(LSDiffuse) 
	,_LSSpecular(LSSpecular)
	,_MaterialAmbient(MaterialAmbient) 
	,_MaterialDiffuse(MaterialDiffuse)
	,_MaterialSpecular(MaterialSpecular)
	{
	}
	 KrazyVehicalShade(KrazyVehicalShade& shade)
	 {
		 copy(shade);
	 }
	 KrazyVehicalShade& operator=(KrazyVehicalShade& shade)
	 {
		 copy(shade);
		 return *this;
	 }
	~KrazyVehicalShade(){}
public :
	//
	glm::vec4& LSPosition(){ return _LSPosition; }
	glm::vec4& LSAmbient(){ return _LSAmbient; }
	glm::vec4& LSDiffuse(){ return _LSDiffuse; } 
	glm::vec4& LSSpecular(){ return _LSSpecular; }
	glm::vec4& MaterialAmbient(){ return _MaterialAmbient; }
	glm::vec4& MaterialDiffuse(){ return _MaterialDiffuse;}
	glm::vec4& MaterialSpecular(){ return _MaterialSpecular;}
	//
private	:
	void copy(KrazyVehicalShade& shade)
	{
		_LSPosition = shade.LSPosition();
		_LSAmbient = shade.LSAmbient();
		_LSDiffuse = shade.LSDiffuse(); 
		_LSSpecular = shade.LSSpecular();
		_MaterialAmbient = shade.MaterialAmbient(); 
		_MaterialDiffuse = shade.MaterialDiffuse(); 
		_MaterialSpecular = shade.MaterialSpecular();
	}
	//
	glm::vec4 _LSPosition;
	glm::vec4 _LSAmbient;
	glm::vec4 _LSDiffuse; 
	glm::vec4 _LSSpecular;
	glm::vec4 _MaterialAmbient; 
	glm::vec4 _MaterialDiffuse; 
	glm::vec4 _MaterialSpecular;
	//
	
};
//
class KrazyVehicalShadeFactory
{
private:
	KrazyVehicalShadeFactory() ;	
	~KrazyVehicalShadeFactory();
	static KrazyVehicalShadeFactory* _PTRMe;
public:
	static KrazyVehicalShadeFactory* getDefault();
	static void releaseDefault();
	KrazyVehicalShade* getShade(KrazyShade shade);
	//
protected:
	void cleanUp();
	//...................
	KrazyVehicalShade* _KrazyShade_Red;
	KrazyVehicalShade* _KrazyShade_Blue;
	KrazyVehicalShade* _KrazyShade_Green;
	KrazyVehicalShade* _KrazyShade_Yellow;
	KrazyVehicalShade* _KrazyShade_Orange;
	KrazyVehicalShade* _KrazyShade_Pink;
	KrazyVehicalShade* _KrazyShade_Brown;
	KrazyVehicalShade* _KrazyShade_Purple;
	KrazyVehicalShade* _KrazyShade_Turquoisblue;
	KrazyVehicalShade* _KrazyShade_Sherrif;
	

	 
};
//



