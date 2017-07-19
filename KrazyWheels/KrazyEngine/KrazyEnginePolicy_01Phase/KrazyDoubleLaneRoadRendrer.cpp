/*
 * CDoubleLaneRoadRendrer.cpp
 *
 *  Created on: Dec 14, 2013
 *      Author: Siddharth
 */
#include "stdafx.h"
#include "KrazyDoubleLaneRoadRendrer.h"
#include "GLeish.h"
#include "KrazyDoubleLaneRoadRendrer.h"
#include "KrazyColorDefinations.h"
#include "KrazyLevelCityContextInterface.h"
#include "KrazyViewPortContext.h"
#include "KrazyTrafficIndicator.h"
#include "KrazyLevelThriver.h"
#include "Logger.h"
#ifdef _IS_FOR_SIMONE
#include "KrazyLevelCityNetwork.h"
#endif
//
//#define _ENABLE_VBO
#define MAX_WAIT_TI  1000
#define MAX_GO_TI    200
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
KrazyDoubleLaneRendrer::KrazyDoubleLaneRendrer() :
_VBOHorzsLaneVertices(0)
,_VBOHorzsBorderVertices(0)
,_HorzsVertices(0)
,_HorzsBorders(0)
,_VBOVertsLaneVertices(0)
,_VBOVertsBorderVertices(0)
,_VertsVertices(0)
,_VertsBorders(0)
,_PTRBlockingTrafficIndicators(0)
,_created(false)
,_buffer_is_there(false)
{
}
//
KrazyDoubleLaneRendrer::~KrazyDoubleLaneRendrer()
{
	if(_buffer_is_there)
		cleanBuffers();
	//..,
	if(_PTRBlockingTrafficIndicators)
	{
		_PTRBlockingTrafficIndicators->clear();
		delete _PTRBlockingTrafficIndicators;_PTRBlockingTrafficIndicators = 0;
	}
}
//
void KrazyDoubleLaneRendrer::cleanBuffers()
{
#if 1
	if(glIsBuffer(_VBOHorzsLaneVertices))
		glDeleteBuffers(1,&_VBOHorzsLaneVertices);

	if(glIsBuffer(_VBOHorzsBorderVertices))
		glDeleteBuffers(1,&_VBOHorzsBorderVertices);

	if(glIsBuffer(_VBOVertsLaneVertices))
		glDeleteBuffers(1,&_VBOVertsLaneVertices);

	if(glIsBuffer(_VBOVertsBorderVertices))
		glDeleteBuffers(1,&_VBOVertsBorderVertices);

	if(glIsBuffer(_VBOBlockingIndicator))
		glDeleteBuffers(1,&_VBOBlockingIndicator);
#endif

}
//
void KrazyDoubleLaneRendrer::reset()
{
	if(_buffer_is_there)
		cleanBuffers();
	//
	_Programme.reset();
	_Position   = 0;	
	_Projection = 0; 	  
	_View       = 0; 	  
	_ScreenScale = 0; 	  
	_Color       = 0;	  
	_AmbientColor = 0;	 
	//
	_ProgrammeTI.reset();
	_PositionTI   = 0; 
	_ProjectionTI = 0; 
	_ModelViewTI  = 0; 
	_ScreenScaleTI = 0; 
	_ColorTI       = 0; 
	_created = false;
}
//
bool KrazyDoubleLaneRendrer::onCreateProgramme()
{
	char vertex_shader[] =
    "attribute vec3 a_vPosition;\n"
	"uniform mat4   u_mProjection;\n"
	"uniform mat4   u_mView;\n"
	"uniform mat4   u_mScreenScale;\n"
	"uniform vec4   u_vMaterialColor;\n"
	"uniform vec4   u_vAmbientColor;\n"
	"varying vec4   v_vColor;\n"
	"void main()\n"
	"{\n"
		"v_vColor = u_vMaterialColor *  u_vAmbientColor;\n"
		"gl_Position = u_mProjection *  u_mScreenScale * u_mView *  vec4(a_vPosition , 1.0);\n"
	"}\n"
	;
	char fragment_shader[] = 
	"varying lowp vec4   v_vColor;\n"
	"void main()\n"
	"{\n"
		"gl_FragColor = v_vColor;\n"
	"}\n";
	KrazyShader vertexShader;vertexShader.LoadShader(GL_VERTEX_SHADER,vertex_shader);
    KrazyShader fragmentShader;fragmentShader.LoadShader(GL_FRAGMENT_SHADER,fragment_shader);

	_Programme.create(vertexShader,fragmentShader);
	_Programme.compile();

	
	_Position     = glGetAttribLocation (_Programme , "a_vPosition");
	_Projection   = glGetUniformLocation(_Programme , "u_mProjection");
	_View         = glGetUniformLocation(_Programme , "u_mView");
	_ScreenScale  = glGetUniformLocation(_Programme , "u_mScreenScale");
	_Color        = glGetUniformLocation(_Programme , "u_vMaterialColor");
	_AmbientColor = glGetUniformLocation(_Programme , "u_vAmbientColor");

	char vertex_shader_ti[] = 
	"attribute vec3 a_vPosition;\n"
	"uniform mat4   u_mProjection;\n"
	"uniform mat4   u_mModelView;\n"
	"uniform mat4   u_mScreenScale;\n"
	"void main()\n"
	"{\n"
		"gl_Position = u_mProjection *  u_mScreenScale * u_mModelView *  vec4(a_vPosition , 1.0);\n"
	"}\n"
	;
	
	char fragment_shader_ti[] = 
	"uniform lowp vec4   u_vColor;\n"
	"void main()\n"
	"{\n"
		"gl_FragColor = u_vColor;\n"
	"}\n";


	KrazyShader vertexShader_ti;vertexShader_ti.LoadShader(GL_VERTEX_SHADER,vertex_shader_ti);
    KrazyShader fragmentShader_ti;fragmentShader_ti.LoadShader(GL_FRAGMENT_SHADER,fragment_shader_ti);

	_ProgrammeTI.create(vertexShader_ti,fragmentShader_ti);
	_ProgrammeTI.compile();

	_PositionTI     = glGetAttribLocation (_ProgrammeTI , "a_vPosition");
	_ProjectionTI   = glGetUniformLocation(_ProgrammeTI , "u_mProjection");
	_ModelViewTI    = glGetUniformLocation(_ProgrammeTI , "u_mModelView");
	_ScreenScaleTI  = glGetUniformLocation(_ProgrammeTI , "u_mScreenScale");
	_ColorTI        = glGetUniformLocation(_ProgrammeTI , "u_vColor");
	_created = true;
	return true;
}
//
#ifdef _ENABLE_VBO
void KrazyDoubleLaneRendrer::onRefresh(KrazyLevelCityContext* city_context)
{
	
	if(!city_context)
		return;

	if(_buffer_is_there)
		cleanBuffers();
	
	if(!_PTRBlockingTrafficIndicators) 
		_PTRBlockingTrafficIndicators = new KrazyTrafficIndicatorList;
	//..
	_PTRBlockingTrafficIndicators->clear();
		
	city_context->on_set_blockingTI(_PTRBlockingTrafficIndicators);
	KrazyFloatList* _horzs_vertices = city_context->getHorzsVertexList();
	KrazyFloatList* _horzs_borders  = city_context->getHorzsBorderList();
	KrazyFloatList* _verts_vertices = city_context->getVertsVertexList();
	KrazyFloatList* _verts_borders  = city_context->getVertsBorderList();
	//..,
	_HorzsVertices = _horzs_vertices->size();
	_HorzsBorders  = _horzs_borders->size();
	_VertsVertices = _verts_vertices->size();
	_VertsBorders  = _verts_borders->size();
	//...,
	glGenBuffers(1,&_VBOHorzsLaneVertices);
	glBindBuffer(GL_ARRAY_BUFFER, _VBOHorzsLaneVertices);
#ifdef _IS_FOR_SIMONE
	glBufferData(GL_ARRAY_BUFFER, _HorzsVertices * sizeof(float) , _horzs_vertices->data(), GL_STATIC_DRAW);
#else
	glBufferData(GL_ARRAY_BUFFER, _HorzsVertices * sizeof(float) , &_horzs_vertices->at(0), GL_STATIC_DRAW);
#endif
	glGenBuffers(1,&_VBOHorzsBorderVertices);
	glBindBuffer(GL_ARRAY_BUFFER, _VBOHorzsBorderVertices);
	
#ifdef _IS_FOR_SIMONE
	glBufferData(GL_ARRAY_BUFFER, _HorzsBorders * sizeof(float) ,  _horzs_borders->data(), GL_STATIC_DRAW);
#else
	glBufferData(GL_ARRAY_BUFFER, _HorzsBorders * sizeof(float) ,  &_horzs_borders->at(0), GL_STATIC_DRAW);
#endif
	//...,
	glGenBuffers(1,&_VBOVertsLaneVertices);
	glBindBuffer(GL_ARRAY_BUFFER, _VBOVertsLaneVertices);
#ifdef _IS_FOR_SIMONE
	glBufferData(GL_ARRAY_BUFFER, _VertsVertices * sizeof(float) , _verts_vertices->data(), GL_STATIC_DRAW);
#else
	glBufferData(GL_ARRAY_BUFFER, _VertsVertices * sizeof(float) , &_verts_vertices->at(0), GL_STATIC_DRAW);
#endif
	//...,
	glGenBuffers(1,&_VBOVertsBorderVertices);
	glBindBuffer(GL_ARRAY_BUFFER, _VBOVertsBorderVertices);
	
#ifdef _IS_FOR_SIMONE
	glBufferData(GL_ARRAY_BUFFER, _VertsBorders * sizeof(float) , _verts_borders->data(), GL_STATIC_DRAW);
#else
	glBufferData(GL_ARRAY_BUFFER, _VertsBorders * sizeof(float) , &_verts_borders->at(0), GL_STATIC_DRAW);
#endif
	//...,
	glGenBuffers(1,&_VBOBlockingIndicator);
	glBindBuffer(GL_ARRAY_BUFFER, _VBOBlockingIndicator);
	glBufferData(GL_ARRAY_BUFFER, _g_BlockingIndicators * sizeof(float) , _g_BlockingIndicator, GL_STATIC_DRAW);
	//...,
#if 0
	glGenBuffers(1,&_VBOSherrifBay);
	glBindBuffer(GL_ARRAY_BUFFER, _VBOSherrifBay);
	glBufferData(GL_ARRAY_BUFFER, _g_SherrifBay_s_ * sizeof(float) , _g_SherrifBay, GL_STATIC_DRAW);
#endif
	_buffer_is_there = true;

}
//
void KrazyDoubleLaneRendrer::on_resize()
{
	_Width  = float(KrazyViewPortContext::_Width);
	_Height = float(KrazyViewPortContext::_Height);

	float aspect = _Width/_Height;
	float left   = -1.0f * aspect;
	float right  =  1.0f * aspect;
	float bottom = -1.0f;
	float top    =  1.0f;
	float near_p   = -1.0f;
	float far_p    =  1.0f;

	_WindowScaleMatrix = glm::mat4((aspect/* - 0.015f*/), 0.0f , 0.0f , 0.0f, /*make this adjustment from crazy wheel editor*/
							  0.0f , 1.0f , 0.0f , 0.0f,
							  0.0f , 0.0f , 1.0f , 0.0f,
							  0.0f   , 0.0f , 0.0f , 1.0f);
	
	_ProjectionMatrix = glm::ortho(left , right , bottom , top , near_p , far_p);

}
//
bool KrazyDoubleLaneRendrer::render()
{
	glUseProgram(_Programme);
	glUniformMatrix4fv(_Projection, 1, false, glm::value_ptr(_ProjectionMatrix));
	glUniformMatrix4fv(_ScreenScale, 1, false, glm::value_ptr(_WindowScaleMatrix));
	glUniform4fv(_Color, 1 , COLOR_GREY);
	glUniform4fv(_AmbientColor, 1 , COLOR_ROAD_AMBIANCE);
	//..,
	_Width  = float(KrazyViewPortContext::_Width);
	_Height = float(KrazyViewPortContext::_Height);
	float aspect = _Width/_Height;
	float scalex = 1.0f;float scaley = 1.0f;
	//....................................Horizontal Scale .......................................................................,
	glm::mat4 scale = glm::mat4( scalex , 0.0f   , 0.0f , 0.0f,
								 0.0f   , scaley , 0.0f , 0.0f,
								 0.0f	, 0.0f	 , 1.0f , 0.0f,
								 0.0f   , 0.0f   , 0.0f , 1.0f);
	//..,
	glUniformMatrix4fv(_View, 1, false, glm::value_ptr(scale));
	glUniform4fv(_Color, 1 , COLOR_GREY);
	glUniform4fv(_AmbientColor, 1 , COLOR_ROAD_AMBIANCE);
	//.................... Horizontal Lane .....................................................................................,
	glBindBuffer(GL_ARRAY_BUFFER, _VBOHorzsLaneVertices);
	glVertexAttribPointer(_Position, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(_Position);
	glDrawArrays(GL_TRIANGLES, 0 , _HorzsVertices/3);
	glDisableVertexAttribArray(_Position);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//.................... Horizontal Lane Border.................................................................................,
	float amb[] = {1.0f , 1.0f , 1.0f , 1.0f};
	glUniform4fv(_Color, 1 , COLOR_YELLOW);	
	glUniform4fv(_AmbientColor, 1 , amb);
	glBindBuffer(GL_ARRAY_BUFFER, _VBOHorzsBorderVertices);
	glVertexAttribPointer(_Position, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(_Position);
	glDrawArrays(GL_LINES, 0 , _HorzsBorders/2);
	glDisableVertexAttribArray(_Position);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//....................................Vertical Scale .......................................................................,
	scalex = 1.0f;scaley = 1.0;
	scale = glm::mat4( scalex , 0.0f   , 0.0f , 0.0f,
								 0.0f   , scaley , 0.0f , 0.0f,
								 0.0f	, 0.0f	 , 1.0f , 0.0f,
								 0.0f   , 0.0f   , 0.0f , 1.0f);
	//..,
	glUniformMatrix4fv(_View, 1, false, glm::value_ptr(scale));
	glUniform4fv(_Color, 1 , COLOR_GREY);
	glUniform4fv(_AmbientColor, 1 , COLOR_ROAD_AMBIANCE);
	//.................... Vertical Lane .....................................................................................,
	glBindBuffer(GL_ARRAY_BUFFER, _VBOVertsLaneVertices);
	glVertexAttribPointer(_Position, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(_Position);
	glDrawArrays(GL_TRIANGLES, 0 , _VertsVertices/3);
	glDisableVertexAttribArray(_Position);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//.................... Vertical Lane Border.................................................................................,
	//float amb[] = {1.0f , 1.0f , 1.0f , 1.0f};
	glUniform4fv(_Color, 1 , COLOR_YELLOW);	
	glUniform4fv(_AmbientColor, 1 , amb);
	glBindBuffer(GL_ARRAY_BUFFER, _VBOVertsBorderVertices);
	glVertexAttribPointer(_Position, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(_Position);
	glDrawArrays(GL_LINES, 0 , _VertsBorders/2);
	glDisableVertexAttribArray(_Position);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//...,
	glUseProgram(0);
	// Blocking Indicator Display....................................................................................................,
	if(_PTRBlockingTrafficIndicators){
		glUseProgram(_ProgrammeTI);
		glUniformMatrix4fv(_ProjectionTI, 1, false, glm::value_ptr(_ProjectionMatrix));
		glUniformMatrix4fv(_ScreenScaleTI, 1, false, glm::value_ptr(_WindowScaleMatrix));
		int count = _PTRBlockingTrafficIndicators->size();
		float scalex = 1.0f;
		float scaley = 1.0f;
		float aspect = _Width/_Height;
		for(int i = 0; i < count; i++){
			KrazyTrafficIndicator* _indicator = _PTRBlockingTrafficIndicators->at(i);
			glm::vec2* vec  = _indicator->Vector();
			float x = vec->x;float y = vec->y;
			glm::mat4 model_view;
			switch(_indicator->Type())
			{
			case KRAZY_TRAFFIC_INDC_Exit:
				{
					scalex = 1.0f;scaley = aspect;
					glm::mat4 model(
						1.0f , 0.0f , 0.0f ,0.0f,
						0.0f , 1.0f , 0.0f ,0.0f,
						0.0f , 0.0f , 1.0f ,0.0f,
						x	 , y    , 0.0f ,1.0f
						);
					glm::mat4 scale = glm::mat4( scalex , 0.0f , 0.0f , 0.0f,
						0.0f , scaley , 0.0f , 0.0f,
						0.0f , 0.0f , 1.0f , 0.0f,
						0.0f , 0.0f , 0.0f , 1.0f);

					model_view = model * scale;
					glUniform4fv(_ColorTI, 1 , COLOR_WHITE);
					glBindBuffer(GL_ARRAY_BUFFER, _VBOBlockingIndicator);
					glVertexAttribPointer(_PositionTI, 3, GL_FLOAT, GL_FALSE, 0, 0);
				}
				break;
			case KRAZY_TRAFFIC_INDC_NoEntry:
				{
				scalex = 1.0f;scaley = aspect;
				glm::mat4 model(
						1.0f , 0.0f , 0.0f ,0.0f,
						0.0f , 1.0f , 0.0f ,0.0f,
						0.0f , 0.0f , 1.0f ,0.0f,
						x	 , y    , 0.0f ,1.0f
						);
					glm::mat4 scale = glm::mat4( scalex , 0.0f , 0.0f , 0.0f,
						0.0f , scaley , 0.0f , 0.0f,
						0.0f , 0.0f , 1.0f , 0.0f,
						0.0f , 0.0f , 0.0f , 1.0f);
				
					model_view = model * scale;
					glUniform4fv(_ColorTI, 1 , COLOR_RED);
					glBindBuffer(GL_ARRAY_BUFFER, _VBOBlockingIndicator);
					glVertexAttribPointer(_PositionTI, 3, GL_FLOAT, GL_FALSE, 0, 0);
				}
					break;
			case KRAZY_TRAFFIC_INDC_Wait:
				{
#if 0
					switch(_indicator->waiting_vehical_state())
					{
					case KRAZY_VEHICAL_WAITING_Start:
						if(_indicator->waiting_vehical() && _indicator->wait()++ == MAX_WAIT_TI)
						{
							_indicator->wait() = 0;
							_indicator->swap_lights();
						}
						break;
					case KRAZY_VEHICAL_WAITING_End:
						if(_indicator->waiting_vehical() && _indicator->wait()++ ==  MAX_GO_TI)
						{
							_indicator->wait() = 0;
						    _indicator->swap_lights();
							_indicator->set_WaitingVehical(0);
						    _indicator->waiting_vehical_state() = KRAZY_VEHICAL_WAITING_Default;
						} 
						break;
					}
#endif
					scalex = 1.0f;scaley = aspect;
					glm::mat4 model(
						1.0f , 0.0f , 0.0f ,0.0f,
						0.0f , 1.0f , 0.0f ,0.0f,
						0.0f , 0.0f , 1.0f ,0.0f,
						x	 , y    , 0.0f ,1.0f
						);
					glm::mat4 scale = glm::mat4( scalex , 0.0f , 0.0f , 0.0f,
						0.0f , scaley , 0.0f , 0.0f,
						0.0f , 0.0f , 1.0f , 0.0f,
						0.0f , 0.0f , 0.0f , 1.0f);
					model_view = model * scale;
					glUniform4fv(_ColorTI, 1 , _indicator->get_CurrentLight());
					glBindBuffer(GL_ARRAY_BUFFER, _VBOBlockingIndicator);
					glVertexAttribPointer(_PositionTI, 3, GL_FLOAT, GL_FALSE, 0, 0);
				}
				break;
#if 0
			case KRAZY_TRAFFIC_INDC_SherrifBay:
				{
					scalex = 1.0f;scaley = aspect;
					glm::mat4 view(
						1.0f , 0.0f , 0.0f ,0.0f,
						0.0f , 1.0f , 0.0f ,0.0f,
						0.0f , 0.0f , 1.0f ,0.0f,
						x	 , y    , 0.0f ,1.0f
						);
					float angle = _indicator->RotationAngle() * DEG_2_RAD;
					glm::mat4 model( cos(angle) , sin(angle) ,	0.0f , 0.0f ,
		        	-sin(angle) , cos(angle) ,  0.0f , 0.0f ,
					0.0f , 0.0f , 1.0f , 0.0f,
					0.0f , 0.0f , 0.0f , 1.0f);
    
					glm::mat4 scale = glm::mat4( scalex , 0.0f , 0.0f , 0.0f,
						0.0f , scaley , 0.0f , 0.0f,
						0.0f , 0.0f , 1.0f , 0.0f,
						0.0f , 0.0f , 0.0f , 1.0f);
					model_view = view * model * scale;
					glUniform4fv(_ColorTI, 1 , COLOR_GREY);
					glBindBuffer(GL_ARRAY_BUFFER, _VBOSherrifBay);
					glVertexAttribPointer(_PositionTI, 3, GL_FLOAT, GL_FALSE, 0, 0);
				}
				 break;
#endif
			}
			glUniformMatrix4fv(_ModelViewTI, 1, false, glm::value_ptr(model_view));
			glEnableVertexAttribArray(_PositionTI);
			glDrawArrays(GL_TRIANGLES, 0 , 6);
			glDisableVertexAttribArray(_PositionTI);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}
	return true;
}
#else
void KrazyDoubleLaneRendrer::onRefresh(KrazyLevelCityContext* city_context)
{
	
	if(!city_context)
		return;
	
	if(!_PTRBlockingTrafficIndicators) 
		_PTRBlockingTrafficIndicators = new KrazyTrafficIndicatorList;
	//..
	_PTRBlockingTrafficIndicators->clear();
	city_context->on_set_blockingTI(_PTRBlockingTrafficIndicators);
	city_context->onSetHorzsVertexList(&_HorzsRoadBuffer);
	city_context->onSetHorzsBorderList(&_HorzsBorderBuffer);
	city_context->onSetVertsVertexList(&_VertsRoadBuffer);
	city_context->onSetVertsBorderList(&_VertsBorderBuffer);
	//..,
	_HorzsVertices = _HorzsRoadBuffer.size();
	_HorzsBorders  = _HorzsBorderBuffer.size();
	_VertsVertices = _VertsRoadBuffer.size();
	_VertsBorders  = _VertsBorderBuffer.size();
}
//
void KrazyDoubleLaneRendrer::on_resize()
{
	_Width  = float(KrazyViewPortContext::_Width);
	_Height = float(KrazyViewPortContext::_Height);

	float aspect = _Width/_Height;
	float left   = -1.0f * aspect;
	float right  =  1.0f * aspect;
	float bottom = -1.0f;
	float top    =  1.0f;
	float near_p   = -1.0f;
	float far_p    =  1.0f;

	_WindowScaleMatrix = glm::mat4((aspect/* - 0.015f*/), 0.0f , 0.0f , 0.0f, /*make this adjustment from crazy wheel editor*/
							  0.0f , 1.0f , 0.0f , 0.0f,
							  0.0f , 0.0f , 1.0f , 0.0f,
							  0.0f   , 0.0f , 0.0f , 1.0f);
	
	_ProjectionMatrix = glm::ortho(left , right , bottom , top , near_p , far_p);

}
//
bool KrazyDoubleLaneRendrer::render()
{
	glUseProgram(_Programme);
	glUniformMatrix4fv(_Projection, 1, false, glm::value_ptr(_ProjectionMatrix));
	glUniformMatrix4fv(_ScreenScale, 1, false, glm::value_ptr(_WindowScaleMatrix));
	glUniform4fv(_Color, 1 , COLOR_GREY);
	glUniform4fv(_AmbientColor, 1 , COLOR_ROAD_AMBIANCE);
	//..,
	_Width  = float(KrazyViewPortContext::_Width);
	_Height = float(KrazyViewPortContext::_Height);
	float aspect = _Width/_Height;
	float scalex = 1.0f;float scaley = 1.0f;
	//....................................Horizontal Scale .......................................................................,
	glm::mat4 scale = glm::mat4( scalex , 0.0f   , 0.0f , 0.0f,
								 0.0f   , scaley , 0.0f , 0.0f,
								 0.0f	, 0.0f	 , 1.0f , 0.0f,
								 0.0f   , 0.0f   , 0.0f , 1.0f);
	//..,
	glUniformMatrix4fv(_View, 1, false, glm::value_ptr(scale));
	glUniform4fv(_Color, 1 , COLOR_GREY);
	glUniform4fv(_AmbientColor, 1 , COLOR_ROAD_AMBIANCE);
	//.................... Horizontal Lane .....................................................................................,
	glVertexAttribPointer(_Position, 3, GL_FLOAT, GL_FALSE, 0, &_HorzsRoadBuffer[0]);
	glEnableVertexAttribArray(_Position);
	glDrawArrays(GL_TRIANGLES, 0 , _HorzsVertices/3);
	glDisableVertexAttribArray(_Position);
	//.................... Horizontal Lane Border.................................................................................,
	float amb[] = {1.0f , 1.0f , 1.0f , 1.0f};
	glUniform4fv(_Color, 1 , COLOR_YELLOW);	
	glUniform4fv(_AmbientColor, 1 , amb);
	glVertexAttribPointer(_Position, 2, GL_FLOAT, GL_FALSE, 0, &_HorzsBorderBuffer[0]);
	glEnableVertexAttribArray(_Position);
	glDrawArrays(GL_LINES, 0 , _HorzsBorders/2);
	glDisableVertexAttribArray(_Position);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//....................................Vertical Scale .......................................................................,
	scalex = 1.0f;scaley = 1.0;
	scale = glm::mat4( scalex , 0.0f   , 0.0f , 0.0f,
								 0.0f   , scaley , 0.0f , 0.0f,
								 0.0f	, 0.0f	 , 1.0f , 0.0f,
								 0.0f   , 0.0f   , 0.0f , 1.0f);
	//..,
	glUniformMatrix4fv(_View, 1, false, glm::value_ptr(scale));
	glUniform4fv(_Color, 1 , COLOR_GREY);
	glUniform4fv(_AmbientColor, 1 , COLOR_ROAD_AMBIANCE);
	//.................... Vertical Lane .....................................................................................,
	glVertexAttribPointer(_Position, 3, GL_FLOAT, GL_FALSE, 0, &_VertsRoadBuffer[0]);
	glEnableVertexAttribArray(_Position);
	glDrawArrays(GL_TRIANGLES, 0 , _VertsVertices/3);
	glDisableVertexAttribArray(_Position);
	//.................... Vertical Lane Border.................................................................................,
	//float amb[] = {1.0f , 1.0f , 1.0f , 1.0f};
	glUniform4fv(_Color, 1 , COLOR_YELLOW);	
	glUniform4fv(_AmbientColor, 1 , amb);
	glVertexAttribPointer(_Position, 2, GL_FLOAT, GL_FALSE, 0, &_VertsBorderBuffer[0]);
	glEnableVertexAttribArray(_Position);
	glDrawArrays(GL_LINES, 0 , _VertsBorders/2);
	glDisableVertexAttribArray(_Position);
	//...,
	glUseProgram(0);
	// Blocking Indicator Display....................................................................................................,
	if(_PTRBlockingTrafficIndicators){
		glUseProgram(_ProgrammeTI);
		glUniformMatrix4fv(_ProjectionTI, 1, false, glm::value_ptr(_ProjectionMatrix));
		glUniformMatrix4fv(_ScreenScaleTI, 1, false, glm::value_ptr(_WindowScaleMatrix));
		int count = _PTRBlockingTrafficIndicators->size();
		float scalex = 1.0f;
		float scaley = 1.0f;
		float aspect = _Width/_Height;
		for(int i = 0; i < count; i++){
			KrazyTrafficIndicator* _indicator = _PTRBlockingTrafficIndicators->at(i);
			glm::vec2* vec  = _indicator->Vector();
			float x = vec->x;float y = vec->y;
			glm::mat4 model_view;
			switch(_indicator->Type())
			{
			case KRAZY_TRAFFIC_INDC_Exit:
				{
					scalex = 1.0f;scaley = aspect;
					glm::mat4 model(
						1.0f , 0.0f , 0.0f ,0.0f,
						0.0f , 1.0f , 0.0f ,0.0f,
						0.0f , 0.0f , 1.0f ,0.0f,
						x	 , y    , 0.0f ,1.0f
						);
					glm::mat4 scale = glm::mat4( scalex , 0.0f , 0.0f , 0.0f,
						0.0f , scaley , 0.0f , 0.0f,
						0.0f , 0.0f , 1.0f , 0.0f,
						0.0f , 0.0f , 0.0f , 1.0f);

					model_view = model * scale;
					glUniform4fv(_ColorTI, 1 , COLOR_WHITE);
					glVertexAttribPointer(_PositionTI, 3, GL_FLOAT, GL_FALSE, 0, _g_BlockingIndicator);
				}
				break;
			case KRAZY_TRAFFIC_INDC_NoEntry:
				{
				scalex = 1.0f;scaley = aspect;
				glm::mat4 model(
						1.0f , 0.0f , 0.0f ,0.0f,
						0.0f , 1.0f , 0.0f ,0.0f,
						0.0f , 0.0f , 1.0f ,0.0f,
						x	 , y    , 0.0f ,1.0f
						);
					glm::mat4 scale = glm::mat4( scalex , 0.0f , 0.0f , 0.0f,
						0.0f , scaley , 0.0f , 0.0f,
						0.0f , 0.0f , 1.0f , 0.0f,
						0.0f , 0.0f , 0.0f , 1.0f);
				
					model_view = model * scale;
					glUniform4fv(_ColorTI, 1 , COLOR_RED);
					glVertexAttribPointer(_PositionTI, 3, GL_FLOAT, GL_FALSE, 0, _g_BlockingIndicator);
				}
					break;
			case KRAZY_TRAFFIC_INDC_Wait:
				{
#if 0
					switch(_indicator->waiting_vehical_state())
					{
					case KRAZY_VEHICAL_WAITING_Start:
						if(_indicator->waiting_vehical() && _indicator->wait()++ == MAX_WAIT_TI)
						{
							_indicator->wait() = 0;
							_indicator->swap_lights();
						}
						break;
					case KRAZY_VEHICAL_WAITING_End:
						if(_indicator->waiting_vehical() && _indicator->wait()++ ==  MAX_GO_TI)
						{
							_indicator->wait() = 0;
						    _indicator->swap_lights();
							_indicator->set_WaitingVehical(0);
						    _indicator->waiting_vehical_state() = KRAZY_VEHICAL_WAITING_Default;
						} 
						break;
					}
#endif
					scalex = 1.0f;scaley = aspect;
					glm::mat4 model(
						1.0f , 0.0f , 0.0f ,0.0f,
						0.0f , 1.0f , 0.0f ,0.0f,
						0.0f , 0.0f , 1.0f ,0.0f,
						x	 , y    , 0.0f ,1.0f
						);
					glm::mat4 scale = glm::mat4( scalex , 0.0f , 0.0f , 0.0f,
						0.0f , scaley , 0.0f , 0.0f,
						0.0f , 0.0f , 1.0f , 0.0f,
						0.0f , 0.0f , 0.0f , 1.0f);
					model_view = model * scale;
					glUniform4fv(_ColorTI, 1 , _indicator->get_CurrentLight());
					glVertexAttribPointer(_PositionTI, 3, GL_FLOAT, GL_FALSE, 0, _g_BlockingIndicator);
				}
				break;
#if 0
			case KRAZY_TRAFFIC_INDC_SherrifBay:
				{
					scalex = 1.0f;scaley = aspect;
					glm::mat4 view(
						1.0f , 0.0f , 0.0f ,0.0f,
						0.0f , 1.0f , 0.0f ,0.0f,
						0.0f , 0.0f , 1.0f ,0.0f,
						x	 , y    , 0.0f ,1.0f
						);
					float angle = _indicator->RotationAngle() * DEG_2_RAD;
					glm::mat4 model( cos(angle) , sin(angle) ,	0.0f , 0.0f ,
		        	-sin(angle) , cos(angle) ,  0.0f , 0.0f ,
					0.0f , 0.0f , 1.0f , 0.0f,
					0.0f , 0.0f , 0.0f , 1.0f);
    
					glm::mat4 scale = glm::mat4( scalex , 0.0f , 0.0f , 0.0f,
						0.0f , scaley , 0.0f , 0.0f,
						0.0f , 0.0f , 1.0f , 0.0f,
						0.0f , 0.0f , 0.0f , 1.0f);
					model_view = view * model * scale;
					glUniform4fv(_ColorTI, 1 , COLOR_GREY);
					glBindBuffer(GL_ARRAY_BUFFER, _VBOSherrifBay);
					glVertexAttribPointer(_PositionTI, 3, GL_FLOAT, GL_FALSE, 0, 0);
				}
				 break;
#endif
			}
			glUniformMatrix4fv(_ModelViewTI, 1, false, glm::value_ptr(model_view));
			glEnableVertexAttribArray(_PositionTI);
			glDrawArrays(GL_TRIANGLES, 0 , 6);
			glDisableVertexAttribArray(_PositionTI);
		}
	}
	return true;
}
#endif

