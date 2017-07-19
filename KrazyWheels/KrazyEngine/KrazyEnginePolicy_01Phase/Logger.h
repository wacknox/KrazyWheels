#pragma once
#ifndef _LOGGER_H
#define _LOGGER_H
#ifndef _WINDOWS
#include <android/log.h>
#include <android\asset_manager.h>
#define  LOG_TAG    "Krazy_r_Wheels"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
class KrazyLogError{
static bool _logError()
{
	
	switch(glGetError())
	{

	
	case GL_NO_ERROR:
		{
			if(true)
			{
				bool b = true;
				LOGE("GL_NO_ERROR\n");
			}
		}
		return true;


	case GL_INVALID_ENUM:
		{
			if(true)
			{
				bool b = true;
				LOGE("GL_INVALID_ENUM\n");
			}
		}
		return true;

	case GL_INVALID_VALUE:
		{
			if(true)
			{
				bool b = true;
				LOGE("GL_INVALID_VALUE\n");
			}
		}
		return true;



	case GL_INVALID_OPERATION:
		{
			if(true)
			{
				bool b = true;
				LOGE("GL_INVALID_OPERATION\n");
			}
		}
		return true;



	case GL_INVALID_FRAMEBUFFER_OPERATION:
		{
			if(true)
			{
				bool b = true;
				LOGE("GL_INVALID_FRAMEBUFFER_OPERATION\n");
			}
		}
		return true;



	case GL_OUT_OF_MEMORY:
		{
			if(true)
			{
				bool b = true;
				LOGE("GL_OUT_OF_MEMORY\n");
			}
		}
		return true;


	}

	return false;
}
};
#endif

#endif