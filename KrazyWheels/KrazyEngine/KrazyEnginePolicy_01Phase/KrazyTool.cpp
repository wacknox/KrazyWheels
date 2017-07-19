#include "stdafx.h"
#include "KrazyTool.h"
#include "GLeish.h"
#include "KrazyPlayToolRect.h"
#include "KrazyViewPortContext.h"
#include "KrazyWheelsRendrer.h"
//..,
KrazyToolRendrer* KrazyToolRendrer::_Me = 0;
//..,
KrazyToolButton::KrazyToolButton(KrazyToolButtonId _tool_button_id) :
_Id(_tool_button_id)
{
	reset();
}
//..,
KrazyToolButton::~KrazyToolButton(void)
{
}
//..,
void KrazyToolButton::reset()
{
	_BackgroundColor.clear();
	_BackgroundRect.clear();
	_ToolColor.clear();
	_ToolRect.clear();
}
//..,
KrazyBool KrazyToolButton::on_touch(float _x , float _y)
{
	glm::vec2 _left_top(_BackgroundRect[2] , _BackgroundRect[3]);
	glm::vec2 _right_bottom(_BackgroundRect[10] , _BackgroundRect[11]);
	//..,
	float _width  =  float(KrazyViewPortContext::_Width);
	float _height =  float(KrazyViewPortContext::_Height);
	float aspect = _width/_height;
	float left   = -1.0f * aspect;
	float right  =  1.0f * aspect;
	float bottom = -1.0f;
	float top    =  1.0f;
	float near_p   = -1.0f;
	float far_p    =  1.0f;
	//..,
	glm::mat4 _projectionMatrix  = glm::ortho(left , right , bottom , top , near_p , far_p);
	glm::mat4 _windowScaleMatrix = glm::mat4((aspect/* - 0.015f*/), 0.0f , 0.0f , 0.0f, /*make this adjustment from crazy wheel editor*/
								  0.0f , 1.0f , 0.0f , 0.0f,
								  0.0f , 0.0f , 1.0f , 0.0f,
								  0.0f   , 0.0f , 0.0f , 1.0f);
	float x = _Vector.x;
	float y = _Vector.y;
	float scale_x = _Scale[0];
	float scale_y = _Scale[1];
	//.......
	for(int i = 0; i < 2; i++){
		glm::vec2* _vec = 0;
		switch(i){
		case 0 : _vec = &_left_top;     break;
		case 1 : _vec = &_right_bottom; break;
		}
		//..,
		glm::mat4 _model = glm::mat4(
							1.0f , 0.0f , 0.0f ,0.0f,
							0.0f , 1.0f , 0.0f ,0.0f,
							0.0f , 0.0f , 1.0f ,0.0f,
							x	 , y    , 0.0f ,1.0f
							);
		//..,
		glm::mat4 scale = glm::mat4( scale_x , 0.0f , 0.0f , 0.0f,
					0.0f , scale_y , 0.0f , 0.0f,
					0.0f , 0.0f , 1.0f , 0.0f,
					0.0f , 0.0f , 0.0f , 1.0f);
	//
		glm::mat4 modelview = _model * scale; 
		glm::vec4 position = _projectionMatrix * _windowScaleMatrix * modelview * glm::vec4(*_vec , 0.0f , 1.0f);
		*_vec = glm::vec2(position);
	}
	//..,
	if((_x > _left_top.x && _y < _left_top.y) && (_x < _right_bottom.x && _y > _right_bottom.y)){
		return KRAZY_BOOL_True;
	}
	//..,
	return KRAZY_BOOL_False;
}
//..,
void KrazyToolButton::on_set_background_color(KrazyFloatList& _color)
{
	_KRAZY_COPY_LIST(_color , _BackgroundColor);
}
//..
void KrazyToolButton::on_set_background_rect(KrazyFloatList& _rect)
{
	_KRAZY_COPY_LIST(_rect , _BackgroundRect);
}
//..,
void KrazyToolButton::on_set_tool_color(KrazyFloatList& _color)
{
	_KRAZY_COPY_LIST(_color , _ToolColor);
}
//..,
void KrazyToolButton::on_set_tool_rect(KrazyFloatList& _rect)
{
	_KRAZY_COPY_LIST(_rect , _ToolRect);
}
//..,
void KrazyToolButton::on_set_scale(KrazyFloatList& _scale)
{
	_KRAZY_COPY_LIST(_scale , _Scale);
}
//..,
void KrazyToolButton::on_set_vector(glm::vec2& _vec)
{
	_Vector = _vec;
}
//..,
KrazyFloatList* KrazyToolButton::get_background_color() { return &_BackgroundColor; }
KrazyFloatList* KrazyToolButton::get_background_rect()  { return &_BackgroundRect;  }
KrazyFloatList* KrazyToolButton::get_tool_color()       { return &_ToolColor;       }
KrazyFloatList* KrazyToolButton::get_tool_rect()        { return &_ToolRect;        }
KrazyFloatList* KrazyToolButton::get_scale()			{ return &_Scale;			}
glm::vec2*		KrazyToolButton::get_vector()			{ return &_Vector;	        }
KrazyToolButtonId KrazyToolButton::get_id()             { return _Id; }
//..,
KrazyToolWindow::KrazyToolWindow(KrazyWheelsRendrer* _rendrer) :
_Rendrer(_rendrer)
{

#if 0
	_Rendrer = KrazyWheelsRendrer::getDefault();
#endif

	reset();
}
//.,
KrazyToolWindow::~KrazyToolWindow()
{
	cleanUp();
}
//..,
void KrazyToolWindow::reset()
{
}
//..,
void KrazyToolWindow::cleanUp()
{
	int _count = _ToolButtonList.size();
	for(int i = 0; i < _count; i++){
		KrazyToolButton*  _tool_button =_ToolButtonList[i];
		delete _tool_button;_tool_button = 0;
	}
	//..,
	_ToolButtonList.clear();
}
//..,
void KrazyToolWindow::create(KrazyWindowStyle _ws_ /*= KRAZY_WS_None*/)
{
	_ToolRendrer = KrazyToolRendrer::getDefault();
	on_load();
}
//..,
void KrazyToolWindow::on_load()
{
	KrazyFloatList    _coords , _color,_scale;
	KrazyFloatList* _vectors = KrazyPlayToolRect::get_default()->get_vectors();
	KrazyToolButton*  _tool_button = 0;
	_scale.push_back(0.5f);_scale.push_back(0.5f);
	glm::vec2 _vec = glm::vec2(_vectors->at(0) , _vectors->at(1));
	//...........................Pause..............................................
	_color.push_back(0.0f);_color.push_back(0.0f);
	_color.push_back(0.0f);_color.push_back(1.0f);
	//..,
	_coords.push_back(0.1f);_coords.push_back(0.1f);
	_coords.push_back(-0.1f);_coords.push_back(0.1f);
	_coords.push_back(0.1f);_coords.push_back(-0.1f);
	//..,,,,,,,,
	_coords.push_back(-0.1f);_coords.push_back(0.1f);
	_coords.push_back(-0.1f);_coords.push_back(-0.1f);
	_coords.push_back(0.1f);_coords.push_back(-0.1f);
	//..,
	_tool_button = new KrazyToolButton(KRAZY_TOOL_BUTTON_Id_Pause);
	_tool_button->on_set_background_color(_color);
	_tool_button->on_set_background_rect(_coords);
	_tool_button->on_set_vector(_vec);
	//..,
	_color.clear();
	_coords.clear();
	//..,
	_color.push_back(1.0f);_color.push_back(0.0f);
	_color.push_back(0.0f);_color.push_back(1.0f);
	//..,
	_coords.push_back(0.075f);_coords.push_back(0.075);
	_coords.push_back(0.025f);_coords.push_back(0.075);
	_coords.push_back(0.075f);_coords.push_back(-0.075f);
	//..,,,,,,,,
	_coords.push_back(0.025f);_coords.push_back(0.075f);
	_coords.push_back(0.025f);_coords.push_back(-0.075f);
	_coords.push_back(0.075f);_coords.push_back(-0.075f);
	//..,
	_coords.push_back(-0.025f);_coords.push_back(0.075f);
	_coords.push_back(-0.075f);_coords.push_back(0.075f);
	_coords.push_back(-0.025f);_coords.push_back(-0.075f);
	//..,,,,,,,,
	_coords.push_back(-0.075f);_coords.push_back(0.075f);
	_coords.push_back(-0.075f);_coords.push_back(-0.075f);
	_coords.push_back(-0.025f);_coords.push_back(-0.075f);
	//..,
	_tool_button->on_set_tool_color(_color);
	_tool_button->on_set_tool_rect(_coords);
	//..,
	_color.clear();
	_coords.clear();
	//..,
	_tool_button->on_set_scale(_scale);
	_ToolButtonList.push_back(_tool_button);
	//......................................Reset...........................................................
	_vec = glm::vec2(_vectors->at(2) , _vectors->at(3));
	_color.push_back(0.0f);_color.push_back(0.0f);
	_color.push_back(0.0f);_color.push_back(1.0f);
	//..,
	_coords.push_back(0.1f);_coords.push_back(0.1f);
	_coords.push_back(-0.1f);_coords.push_back(0.1f);
	_coords.push_back(0.1f);_coords.push_back(-0.1f);
	//..,,,,,,,,
	_coords.push_back(-0.1f);_coords.push_back(0.1f);
	_coords.push_back(-0.1f);_coords.push_back(-0.1f);
	_coords.push_back(0.1f);_coords.push_back(-0.1f);
	//..,
	_tool_button = new KrazyToolButton(KRAZY_TOOL_BUTTON_Id_Reset);
	_tool_button->on_set_background_color(_color);
	_tool_button->on_set_background_rect(_coords);
	_tool_button->on_set_vector(_vec);
	//..,
	_color.clear();
	_coords.clear();
	//..,
	_color.push_back(0.25f);_color.push_back(1.0f);
	_color.push_back(0.25f);_color.push_back(1.0f);
	//..,
	_coords.push_back(0.075f);_coords.push_back(0.025);
	_coords.push_back(0.025f);_coords.push_back(0.025);
	_coords.push_back(0.075f);_coords.push_back(-0.075f);
	//..,,,,,,,,
	_coords.push_back(0.025f);_coords.push_back(0.025f);
	_coords.push_back(0.025f);_coords.push_back(-0.075f);
	_coords.push_back(0.075f);_coords.push_back(-0.075f);
	//..,
	_coords.push_back(-0.025f);_coords.push_back(0.075f);
	_coords.push_back(-0.075f);_coords.push_back(0.075f);
	_coords.push_back(-0.025f);_coords.push_back(-0.05f);//_coords.push_back(-0.075f);
	//..,,,,,,,,
	_coords.push_back(-0.075f);_coords.push_back(0.075f);
	_coords.push_back(-0.075f);_coords.push_back(-0.05f);//_coords.push_back(-0.075f);
	_coords.push_back(-0.025f);_coords.push_back(-0.05f);
	//..,
	_coords.push_back(0.1f);_coords.push_back(0.025f);
	_coords.push_back(0.05f);_coords.push_back(0.1f);
	_coords.push_back(0.0f);_coords.push_back(0.025f);
	//..,,,,,,,,
	_coords.push_back(0.0f);_coords.push_back(-0.025f);
	_coords.push_back(-0.1f);_coords.push_back(-0.025f);
	_coords.push_back(-0.05f);_coords.push_back(-0.1f);

	_tool_button->on_set_tool_color(_color);
	_tool_button->on_set_tool_rect(_coords);
	//..,
	/*_color.clear();
	_coords.clear();*/
	//..,
	_tool_button->on_set_scale(_scale);
	_ToolButtonList.push_back(_tool_button);

}
//..,
void KrazyToolWindow::render()
{
	int _count = _ToolButtonList.size();
	for(int i = 0; i < _count; i++){
		KrazyToolButton*  _tool_button =_ToolButtonList[i];
		_ToolRendrer->render(_tool_button);
	}
}
//..,
void KrazyToolWindow::on_refresh()
{
	_ToolRendrer = KrazyToolRendrer::getDefault();
}
//
KrazyBool KrazyToolWindow::on_touch(float _x , float _y)
{
	int _count = _ToolButtonList.size();
	for(int i = 0; i < _count; i++){
		KrazyToolButton*  _tool_button =_ToolButtonList[i];
		KrazyBool _touched = _tool_button->on_touch(_x , _y);
		switch(_touched)
		{
		case KRAZY_BOOL_True: 
			switch(_tool_button->get_id()){
			case KRAZY_TOOL_BUTTON_Id_Pause  : _Rendrer->onPause();break;
			case KRAZY_TOOL_BUTTON_Id_Reset  : _Rendrer->onPlaySameLevelAgain();break;
			}
			return _touched;
		}
	}
	//..,
	return KRAZY_BOOL_False;
}
//..,
KrazyToolRendrer* KrazyToolRendrer::getDefault()
{
	if(!_Me){
		_Me = new KrazyToolRendrer;
		_Me->create();
	}
	//..,
	return _Me;
}
//..,
void KrazyToolRendrer::releaseDefault()
{
	if(_Me){
		delete _Me;_Me = 0;
	}
}
//..,
KrazyToolRendrer::KrazyToolRendrer()
{
}
//..,
KrazyToolRendrer::~KrazyToolRendrer()
{
}
//..,
void KrazyToolRendrer::reset()
{
}
//..,
void KrazyToolRendrer::create()
{
	char vertex_shader[] = 
	"attribute vec2 inputPosition;\n"
	"uniform mat4 projection;\n"
	"uniform mat4 window_scale;\n"
	"uniform mat4 modelview;\n"
	"uniform vec4 u_vColor;\n"
	"varying vec4 v_vColor;\n"
	"void main(){\n"
	    "v_vColor = u_vColor;\n"
		"gl_Position  = projection * window_scale * modelview * vec4(inputPosition , 0.0 , 1.0);\n"
	"}\n";
	//..,
	char fragment_shader[] = 
	"precision mediump float;\n"
	"varying vec4 v_vColor;\n"
	"void main()\n"
	"{\n"
	"gl_FragColor = v_vColor;\n"
	"}\n";
	//..,
	KrazyShader vertexShader;vertexShader.LoadShader(GL_VERTEX_SHADER,vertex_shader);
    KrazyShader fragmentShader;fragmentShader.LoadShader(GL_FRAGMENT_SHADER,fragment_shader);
	//.................
	_Programme.create(vertexShader,fragmentShader);
	_Programme.compile();
	//..........................................
	_Position    = glGetAttribLocation (_Programme , "inputPosition");
	_Projection  = glGetUniformLocation(_Programme , "projection");
	_ModelView   = glGetUniformLocation(_Programme , "modelview");
	_WindowScale = glGetUniformLocation(_Programme , "window_scale");
	_Color       = glGetUniformLocation(_Programme , "u_vColor");

}
//..,
void KrazyToolRendrer::render(KrazyToolButton* _tool_button)
{
	float _width  =  float(KrazyViewPortContext::_Width);
	float _height =  float(KrazyViewPortContext::_Height);
	//......................................................................
	KrazyFloatList* _scale = _tool_button->get_scale();
	float scale_x = _scale->at(0);
	float scale_y = _scale->at(1);
	//....................................................................
#if 1
	float aspect = _width/_height;
	float left   = -1.0f * aspect;
	float right  =  1.0f * aspect;
	float bottom =  -1.0f;
	float top    =  1.0f;
	float near   = -1.0f;
	float far    =  1.0f;
#else
	float num    =  2.0f;
	float left   =  0.0f;
	float right  = _width;  /** aspect;*/
	float bottom =  0.0f;
	float top    = _height;
	float near   = -1.0f;
	float far    =  1.0f;
#endif
	glm::vec2* _vec = _tool_button->get_vector();
	KrazyFloatList* _bkg_color = _tool_button->get_background_color();
	KrazyFloatList* _bkg_rect  = _tool_button->get_background_rect();
	KrazyFloatList* _tool_color = _tool_button->get_tool_color();
	KrazyFloatList* _tool_rect  = _tool_button->get_tool_rect();
	glUseProgram(_Programme);
	glm::mat4 view( 1.0f , 0.0f , 0.0f , 0.0f, 
					0.0f , 1.0f , 0.0f , 0.0f, 
					0.0f , 0.0f , 1.0f , 0.0f, 
					_vec->x , _vec->y , 0.0 , 1.0f
					);
	float angle = 0.0f;
	glm::mat4 model( cos(angle) , sin(angle) ,	0.0f , 0.0f ,
		        	-sin(angle) , cos(angle) ,  0.0f , 0.0f ,
					0.0f , 0.0f , 1.0f , 0.0f,
					0.0f , 0.0f , 0.0f , 1.0f);
    //
	
	glm::mat4 scale = glm::mat4( scale_x , 0.0f , 0.0f , 0.0f,
					0.0f , scale_y , 0.0f , 0.0f,
					0.0f , 0.0f , 1.0f , 0.0f,
					0.0f , 0.0f , 0.0f , 1.0f);
	//
	glm::mat4 modelview = view * scale; 
	//
	glm::mat4 _projectionMatrix = glm::ortho(left , right , bottom , top , near , far);
	glm::mat4 _windowScaleMatrix = glm::mat4((aspect/* - 0.015f*/), 0.0f , 0.0f , 0.0f, /*make this adjustment from crazy wheel editor*/
							  0.0f , 1.0 , 0.0f , 0.0f,
							  0.0f , 0.0f , 1.0f , 0.0f,
							  0.0f   , 0.0f , 0.0f , 1.0f);
	//..,
	glUniformMatrix4fv(_Projection, 1, false, glm::value_ptr(_projectionMatrix));
	glUniformMatrix4fv(_WindowScale, 1, false, glm::value_ptr(_windowScaleMatrix));
	glUniformMatrix4fv(_ModelView, 1, false, glm::value_ptr(modelview));
	glUniform4fv(_Color , 1 , &_bkg_color->at(0));
	glVertexAttribPointer(_Position, 2, GL_FLOAT, GL_FALSE, 0, &_bkg_rect->at(0));
	glEnableVertexAttribArray(_Position);
	glDrawArrays(GL_TRIANGLES, 0 , _bkg_rect->size()/2);
	glDisableVertexAttribArray(_Position);
	glUseProgram(0);
	//..,
	int _tool_count = _tool_rect->size();
	if(_tool_count > 0){
		glUseProgram(_Programme);
		glUniformMatrix4fv(_Projection, 1, false, glm::value_ptr(_projectionMatrix));
		glUniformMatrix4fv(_WindowScale, 1, false, glm::value_ptr(_windowScaleMatrix));
		glUniformMatrix4fv(_ModelView, 1, false, glm::value_ptr(modelview));
		glUniform4fv(_Color , 1 , &_tool_color->at(0));
		glVertexAttribPointer(_Position, 2, GL_FLOAT, GL_FALSE, 0, &_tool_rect->at(0));
		glEnableVertexAttribArray(_Position);
		glDrawArrays(GL_TRIANGLES, 0 , _tool_rect->size()/2);
		glDisableVertexAttribArray(_Position);
	}
	//..,
}
//..,