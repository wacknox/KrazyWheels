#include "stdafx.h"
#include "KrazyWindow.h"
#include "GLeish.h"
#include "KrazyViewPortContext.h"
//..,
KrazyWindowRendrer* KrazyWindowRendrer::_Me = 0;
//..,
KrazyWindow::KrazyWindow(void)
{
	init();
}
//
KrazyWindow::~KrazyWindow(void)
{
	_KRAZY_DEALLOCATE_LIST(_TextAreaList);
}
//................................................................
KrazyWindowStyle& KrazyWindow::get_WS()         { return  _WS; }
KrazyFloatList*   KrazyWindow::get_WndRect()    { return &_WndRect; }
KrazyFloatList*   KrazyWindow::get_ClientRect() { return &_ClientRect; }
KrazyFloatList*   KrazyWindow::get_Point()      { return &_Point;}
KrazyFloatList*	  KrazyWindow::get_Scale()		{ return &_ScaleList;}
//................................................................
#ifdef _KRAZY_FOR_VS
KrazyWindow* KrazyWindow::_Me = 0;
KrazyWindow* KrazyWindow::getDefault(bool _create /*= false*/)
{
	if(!_Me){
		_Me = new KrazyWindow;
		if(_create)
			_Me->create();
	}
    //...,
	return _Me;
}
//
void KrazyWindow::releaseDefault()
{
	if(_Me){
		delete _Me;_Me = 0;
	}
}
#endif
//..,
void  KrazyWindow::create(KrazyWindowStyle _ws_ /*= KRAZY_WS_Modal*/)
{
	_WS = _ws_;
	//.........................................................
	_WindowRendrer   = KrazyWindowRendrer::getDefault();
	_TextAreaRendrer = KrazyTextAreaRendrer::getDefault();
}
//
void KrazyWindow::init()
{
	_ScaleList.clear();
	for(int i = 0; i < 2; i++){
		_ScaleList.push_back(1.0f);
	}
}
//.
void KrazyWindow::on_setWndRect(float* _rect)
{
	_KRAZY_COPY_LIST_PTR(_rect , _WndRect , 12);
}
//
void KrazyWindow::on_setClientRect(float* _rect)
{
	_KRAZY_COPY_LIST_PTR(_rect , _ClientRect , 12);
}
//
void KrazyWindow::on_setPoint(KrazyFloatList& _point)
{
	_KRAZY_COPY_LIST(_point ,  _Point);
}
//
void KrazyWindow::on_setScale(KrazyFloatList& _scale)
{
	_KRAZY_COPY_LIST(_scale ,  _ScaleList);
}
//..,
void KrazyWindow::render()
{
	_WindowRendrer->render(this);
	int _count = _TextAreaList.size();
	for(int i = 0; i < _count; i++){
		KrazyTextArea* _text_area = _TextAreaList[i];
		if(!_text_area->empty()){
			_TextAreaRendrer->render(_text_area);
		}
	}
}
//
void KrazyWindow::on_resize()
{
}
//..
void KrazyWindow::on_refresh()
{
	init();
	_WindowRendrer   = KrazyWindowRendrer::getDefault();
	_TextAreaRendrer = KrazyTextAreaRendrer::getDefault();
}
//..
KrazyBool KrazyWindow::on_touch(float _x , float _y)
{
	glm::vec2 _left_top(_ClientRect[2] , _ClientRect[3]);
	glm::vec2 _right_bottom(_ClientRect[10] , _ClientRect[11]);
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
	
	float x = 0.0f;float y = 0.0f;
	float scale_x = _ScaleList[0];float scale_y = _ScaleList[1];
	//.......
	for(int i = 0; i < 2; i++){
		glm::vec2* _vec = 0;
		switch(i){
		case 0 : _vec = &_left_top;     break;
		case 1 : _vec = &_right_bottom; break;
		}
		//..,
		glm::mat4 _projectionMatrix  = glm::ortho(left , right , bottom , top , near_p , far_p);
		glm::mat4 _windowScaleMatrix = glm::mat4((aspect/* - 0.015f*/), 0.0f , 0.0f , 0.0f, /*make this adjustment from crazy wheel editor*/
								  0.0f , 1.0f , 0.0f , 0.0f,
								  0.0f , 0.0f , 1.0f , 0.0f,
								  0.0f   , 0.0f , 0.0f , 1.0f);
		//..,
		glm::mat4 _model = glm::mat4(
							1.0f , 0.0f , 0.0f ,0.0f,
							0.0f , 1.0f , 0.0f ,0.0f,
							0.0f , 0.0f , 1.0f ,0.0f,
							x	 , y    , 0.0f ,1.0f
							);
		//..,
			glm::mat4 scale = glm::mat4(scale_x , 0.0f    , 0.0f ,0.0f,
								 0.0f   , scale_y , 0.0f ,0.0f,
								 0.0f   , 0.0f    , 1.0f ,0.0f,
								 0.0f   , 0.0f    , 0.0f ,1.0f);
		//..,
		glm::vec4 position = _projectionMatrix * _windowScaleMatrix * _model * scale * glm::vec4(*_vec , 0.0f , 1.0f);
		*_vec = glm::vec2(position);
	}
	//..,
	if( (_x > _left_top.x && _y < _left_top.y) && (_x < _right_bottom.x && _y > _right_bottom.y)){
		int _count = _TextAreaList.size();
		for(int i = 0; i < _count; i++){
			KrazyTextArea* _text_area = _TextAreaList[i];
			if(_text_area->enable_touch()){
				KrazyBool _touched = _text_area->on_touch(_x , _y);
				switch(_touched)
				{
				case KRAZY_BOOL_True	:   return _touched;
				}
			}

		}
	}
	//..,
	return KRAZY_BOOL_False;
}
//
KrazyWindowRendrer* KrazyWindowRendrer::getDefault()
{
	if(!_Me){
		_Me = new KrazyWindowRendrer;
		_Me->create();
	}
	//..,
	return _Me;
}
//..,
void KrazyWindowRendrer::releaseDefault()
{
	if(_Me){
		delete _Me; _Me = 0;
	}
}
//..,
KrazyWindowRendrer::KrazyWindowRendrer() : 
  _Position      (0)
, _TextureCoords (0)
, _Projection    (0)
, _ModelView     (0)
, _ScreenScale   (0)
, _TextureId     (0)
, _Color         (0)
{
}
//..
KrazyWindowRendrer::~KrazyWindowRendrer()
{
	_Programme.reset();
}
//......
void KrazyWindowRendrer::reset(){}
//......
void KrazyWindowRendrer::create()
{
	char vertex_shader[] = 
	"precision mediump float;\n"
	"attribute vec2   a_vPosition;\n"
	"uniform mat4     u_mProjection;\n"
	"uniform mat4     u_mModelView;\n"
	"uniform mat4     u_mScreenScale;\n"
	"varying vec4     v_vColor;\n"
    "uniform vec4     u_vColor;\n"
	"void main()\n"
	"{\n"
		"gl_Position = u_mProjection  * u_mScreenScale * u_mModelView  *  vec4(a_vPosition , 0.0 , 1.0);\n"
		"v_vColor = u_vColor;\n"
		";\n"
	"}\n"
	;
	//..,
	char fragment_shader[] = 
	"precision mediump float;\n"
	"varying vec4       v_vColor;\n"
	"varying vec2       v_vTexCoord;\n"
	"uniform sampler2D  s_texture;\n"
	"void main()\n"
	"{\n"
	"gl_FragColor =  v_vColor;\n"
	"}\n";
	//................
	KrazyShader vertexShader;vertexShader.LoadShader(GL_VERTEX_SHADER,vertex_shader);
    KrazyShader fragmentShader;fragmentShader.LoadShader(GL_FRAGMENT_SHADER,fragment_shader);
	//.................
	_Programme.create(vertexShader,fragmentShader);
	_Programme.compile();
	//........................................................
	_Position      = glGetAttribLocation (_Programme , "a_vPosition");
	_TextureCoords = glGetAttribLocation(_Programme ,  "a_vTexCoord");
	_Projection    = glGetUniformLocation(_Programme , "u_mProjection");
	_ModelView     = glGetUniformLocation(_Programme , "u_mModelView");
	_ScreenScale   = glGetUniformLocation(_Programme , "u_mScreenScale");
	_TextureId     = glGetUniformLocation(_Programme , "s_texture");
	_Color         = glGetUniformLocation(_Programme , "u_vColor");
}
//..
static const float _krazy_msg_wnd[] = 
{
	100.0f  ,  100.0f  , 
	0.0f    ,  100.0f  ,
	100.0f  ,    0.0f  ,
	//..,
	 0.0f    ,  100.0f  ,
	 0.0f    ,    0.0f  ,
	100.0f   ,    0.0f  
};
//
void KrazyWindowRendrer::render(KrazyWindow* _window)
{
	float _width  =  float(KrazyViewPortContext::_Width);
	float _height =  float(KrazyViewPortContext::_Height);
	//.............
	float aspect = _width/_height;
	float left   = -1.0f * aspect;
	float right  =  1.0f * aspect;
	float bottom = -1.0f;
	float top    =  1.0f;
	float near_p   = -1.0f;
	float far_p    =  1.0f;
	//..,
	KrazyFloatList* _scale   = _window->get_Scale();
	KrazyFloatList* _wndRect = _window->get_WndRect();
	KrazyFloatList* _clientRect = _window->get_ClientRect();
	float _x = 0.0f;
	float _y = 0.0f;
	float _scalex = _scale->at(0);float _scaley = _scale->at(1);
	//........
	glm::mat4 _projectionMatrix  = glm::ortho(left , right , bottom , top , near_p , far_p);
	glm::mat4 _windowScaleMatrix = glm::mat4((aspect/* - 0.015f*/), 0.0f , 0.0f , 0.0f, /*make this adjustment from crazy wheel editor*/
							  0.0f , 1.0f , 0.0f , 0.0f,
							  0.0f , 0.0f , 1.0f , 0.0f,
							  0.0f   , 0.0f , 0.0f , 1.0f);
	//..,
	glm::mat4 view = glm::mat4(
						1.0f , 0.0f , 0.0f ,0.0f,
						0.0f , 1.0f , 0.0f ,0.0f,
						0.0f , 0.0f , 1.0f ,0.0f,
						_x	 , _y    , 0.0f ,1.0f
						);

	//..,
	glm::mat4 scale = glm::mat4(_scalex , 0.0f    , 0.0f ,0.0f,
								 0.0f   , _scaley , 0.0f ,0.0f,
								 0.0f   , 0.0f    , 1.0f ,0.0f,
								 0.0f   , 0.0f    , 0.0f ,1.0f);
	//..,
	glUseProgram(_Programme);
	float blue[] = {0.0f , 0.0f , 1.0f, 1.0f};
	glUniform4fv(_Color , 1 , blue);
	glUniformMatrix4fv(_Projection,  1, false , glm::value_ptr(_projectionMatrix));
	glUniformMatrix4fv(_ScreenScale, 1, false , glm::value_ptr(_windowScaleMatrix));
	glUniformMatrix4fv(_ModelView,   1, false , glm::value_ptr(view * scale));
    //..,
	glVertexAttribPointer(_Position, 2, GL_FLOAT , GL_FALSE, 0, &_wndRect->at(0));
 	glEnableVertexAttribArray(_Position);
	glDrawArrays(GL_TRIANGLES, 0 ,6);
	glDisableVertexAttribArray(_Position);
	//..,
	float black[] = {0.0f , 0.0f , 0.0f, 1.0f};
	glUniform4fv(_Color , 1 , black);
	glVertexAttribPointer(_Position, 2, GL_FLOAT , GL_FALSE, 0, &_clientRect->at(0));
 	glEnableVertexAttribArray(_Position);
	glDrawArrays(GL_TRIANGLES, 0 ,6);
	glDisableVertexAttribArray(_Position);
	//..,
}
	
	
