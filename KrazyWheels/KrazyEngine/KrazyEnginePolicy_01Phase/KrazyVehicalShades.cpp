#include "stdafx.h"
#include "KrazyVehicalShades.h"
#include "KrazyUtilityMacros.h"
KrazyVehicalShadeFactory* KrazyVehicalShadeFactory::_PTRMe = 0;
//
KrazyVehicalShadeFactory* KrazyVehicalShadeFactory::getDefault()
{
	if(!_PTRMe)
	{
		_PTRMe = new KrazyVehicalShadeFactory;
	}

	return _PTRMe;
}
//
void KrazyVehicalShadeFactory::releaseDefault()
{
	if(_PTRMe)
	{
		delete _PTRMe;_PTRMe = 0;
	}
}
//
KrazyVehicalShadeFactory::KrazyVehicalShadeFactory() : 
	   _KrazyShade_Red(0)
	   ,_KrazyShade_Blue(0)
	   ,_KrazyShade_Green(0)
	   ,_KrazyShade_Yellow(0)
	   ,_KrazyShade_Orange(0)
	   ,_KrazyShade_Pink(0)
	   ,_KrazyShade_Brown(0)
	   ,_KrazyShade_Purple(0)
	   ,_KrazyShade_Turquoisblue(0)
	   ,_KrazyShade_Sherrif(0)
	{
	}
	
KrazyVehicalShadeFactory::~KrazyVehicalShadeFactory()
{
	cleanUp();
}
//
KrazyVehicalShade* KrazyVehicalShadeFactory::getShade(KrazyShade shade)
{
		switch(shade){
		case KrazyShade_Red				:
			if(!_KrazyShade_Red)  
				_KrazyShade_Red = new KrazyVehicalShade(glm::vec4(1.0f , 1.0f , 0.5f , 0.5f),
				/*LS*/
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				glm::vec4(1.0f,  1.0f,  1.0f, 1.0f),
				glm::vec4(1.0f,  1.0f,  1.0f, 1.0f),
				/*MS*/
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
				glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
				glm::vec4(1.2f, 0.0f, 0.0f, 1.0f));

			return _KrazyShade_Red;

		case KrazyShade_Blue			:
			if(!_KrazyShade_Blue) 
				_KrazyShade_Blue = new KrazyVehicalShade(glm::vec4(1.0f , 1.0f , 0.5f , 0.5f),
				//LS
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				glm::vec4(1.0f,  1.0f,  1.0f, 1.0f),
				glm::vec4(1.0f,  1.0f,  1.0f, 1.0f),
				//MS
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
				glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
				glm::vec4(0.0f, 0.0f, 0.2f, 1.0f));

			return _KrazyShade_Blue;

		case KrazyShade_Green			:
			if(!_KrazyShade_Green)
				_KrazyShade_Green = new KrazyVehicalShade(glm::vec4(1.0f , 1.0f , 0.5f , 0.5f),
				//LS
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				glm::vec4(1.0f,  1.0f,  1.0f, 1.0f),
				glm::vec4(1.0f,  1.0f,  1.0f, 1.0f),
				//MS
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
				glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
				glm::vec4(0.0f, 0.2f, 0.0f, 1.0f));

			return _KrazyShade_Green;

		case KrazyShade_Yellow			:
			if(!_KrazyShade_Yellow)
				_KrazyShade_Yellow = new KrazyVehicalShade(glm::vec4(1.0f , 1.0f , 0.5f , 0.5f),
				//LS
				glm::vec4(0.2f, 0.0f, 0.0f, 1.0f),
				glm::vec4(1.0f,  1.0f,  1.0f, 1.0f),
				glm::vec4(1.0f,  1.0f,  1.0f, 1.0f),
				//MS
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
				glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
				glm::vec4(0.2f, 0.2f, 0.0f, 1.0f));

			return _KrazyShade_Yellow;

		case KrazyShade_Orange			:
			if(!_KrazyShade_Orange)
				_KrazyShade_Orange = new KrazyVehicalShade(	glm::vec4(1.0f , 1.0f , 0.5f , 0.5f),
				//LS
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				glm::vec4(1.0f,  1.0f,  1.0f, 1.0f),
				glm::vec4(1.0f,  1.0f,  1.0f, 1.0f),
				//MS
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
				glm::vec4(0.92549019607843137254901960784314f, 0.4357f, 0.075f, 1.0f),
				glm::vec4(0.2f, 0.0f, 0.0f, 1.0f));

			return _KrazyShade_Orange;

		case KrazyShade_Pink			:
			if(!_KrazyShade_Pink)
				_KrazyShade_Pink = new KrazyVehicalShade(glm::vec4(1.0f , 1.0f , 0.5f , 0.5f),
				//LS
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				glm::vec4(1.0f,  1.0f,  1.0f, 1.0f),
				glm::vec4(1.0f,  1.0f,  1.0f, 1.0f),
				//MS
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
				glm::vec4(0.9f, 0.32f, 0.35f, 1.0f),
				glm::vec4(0.2f, 0.0f, 0.0f, 1.0f));

			return _KrazyShade_Pink;

		case KrazyShade_Brown			:
			if(!_KrazyShade_Brown)
				_KrazyShade_Brown = new KrazyVehicalShade(	glm::vec4(1.0f , 1.0f , 0.5f , 0.5f),
				//LS
				glm::vec4(0.1f, 0.0f, 0.0f, 1.0f) , 
				glm::vec4(1.0f,  1.0f,  1.0f, 1.0f) , 
				glm::vec4(1.0f,  1.0f,  1.0f, 1.0f) ,
				//MS
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) , 
				glm::vec4(0.5f, 0.25f, 0.25f, 1.0f) , 
				glm::vec4(0.2f, 0.0f, 0.0f, 1.0f));

			return _KrazyShade_Brown;

		case KrazyShade_Purple			:
			if(!_KrazyShade_Purple)
				_KrazyShade_Purple = new KrazyVehicalShade(	glm::vec4(1.0f , 1.0f , 0.5f , 0.5f),
				//LS
				glm::vec4(0.1f, 0.0f, 0.0f, 1.0f),
				glm::vec4(1.0f,  1.0f,  1.0f, 1.0f),
				glm::vec4(1.0f,  1.0f,  1.0f, 1.0f),
				//MS
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
				glm::vec4(0.25f, 0.25f, 0.6f, 1.0f),
				glm::vec4(0.2f, 0.0f, 0.0f, 1.0f));

			return _KrazyShade_Purple;  

		case KrazyShade_Turquoisblue    :
			if(!_KrazyShade_Turquoisblue)
				_KrazyShade_Turquoisblue = new KrazyVehicalShade(glm::vec4(1.0f , 1.0f , 0.5f , 0.5f),
				//LS
				glm::vec4(0.5f, 0.0f, 0.0f, 1.0f) , 
				glm::vec4(1.0f,  1.0f,  1.0f, 1.0f) , 	
				glm::vec4(1.0f,  1.0f,  1.0f, 1.0f),
				//MS
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
				glm::vec4(0.078f, 0.90f, 0.91f, 1.0f),
				glm::vec4(0.2f, 0.0f, 0.0f, 1.0f));

			return _KrazyShade_Turquoisblue;  

		case KrazyShade_Sherrif			:
			if(!_KrazyShade_Sherrif) 
				_KrazyShade_Sherrif = new KrazyVehicalShade(	glm::vec4(1.0f , 1.0f , 0.5f , 0.5f),
				//LS.
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				glm::vec4(1.0f,  1.0f,  1.0f, 1.0f),
				glm::vec4(1.0f,  1.0f,  1.0f, 1.0f),
				//MS
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
				glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
				glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

			return _KrazyShade_Sherrif; 
		}
		return 0;
}
//..
void KrazyVehicalShadeFactory::cleanUp()
{
	_KRAZY_deallocate(_KrazyShade_Red);
	_KRAZY_deallocate(_KrazyShade_Blue);
	_KRAZY_deallocate(_KrazyShade_Green);
	_KRAZY_deallocate(_KrazyShade_Yellow);
	_KRAZY_deallocate(_KrazyShade_Orange);
	_KRAZY_deallocate(_KrazyShade_Pink);
	_KRAZY_deallocate(_KrazyShade_Brown);
	_KRAZY_deallocate(_KrazyShade_Purple);
	_KRAZY_deallocate(_KrazyShade_Turquoisblue);
	_KRAZY_deallocate(_KrazyShade_Sherrif);
}