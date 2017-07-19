#include "stdafx.h"
#include "KrazyError.h"
#include "GLeish.h"
//..,
KrazyGLError::KrazyGLError()
{

}
//..,
string KrazyGLError::getErrorText()
{
   switch(glGetError()){
   case GL_INVALID_ENUM			:	return string("GL_INVALID_ENUM");
   case GL_INVALID_VALUE		:	return string("GL_INVALID_VALUE");
   case GL_INVALID_OPERATION	:	return string("GL_INVALID_OPERATION");
   case GL_NO_ERROR				:	return string("GL_NO_ERROR");
   case GL_OUT_OF_MEMORY		:	return string("GL_NO_ERROR");
   }
   //..,
   return string("");
}