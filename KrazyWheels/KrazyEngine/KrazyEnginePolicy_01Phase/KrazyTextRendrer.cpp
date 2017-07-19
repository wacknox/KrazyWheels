#include "stdafx.h"
#include "KrazyTextRendrer.h"
#include "KrazyShader.h"
#include "GLeish.h"
#include "KrazyViewPortContext.h"
#include "KrazyAsset.h"
#include "KrazyWindowController.h"
#include <fstream>
#include <iostream>
#include <fstream>
#define _KRAZY_FONTFILE_PATH		string("assets/fonts/KrazyFont.bff")
#define _KRAZY_FONT_FILE_SIZE       0x40114
//#define _KRAZY_DISABLE_PREV_FONT_READING
//////////............................................ KrazyTextArea.................................................................
KrazyBitmapFontFile* KrazyBitmapFontFile::_Me = 0;
KrazyBitmapFontFile* KrazyBitmapFontFile::getDefault()
{
	if(!_Me){
		_Me = new KrazyBitmapFontFile;
		_Me->load(_KRAZY_FONTFILE_PATH);
	}
	return _Me;
}
//
void KrazyBitmapFontFile::releaseDefault()
{
	if(_Me){
		delete _Me;_Me = 0;
	}
}
//
KrazyBitmapFontFile::KrazyBitmapFontFile() 
{
	reset();
}
//
KrazyBitmapFontFile::~KrazyBitmapFontFile()
{
}
//
void KrazyBitmapFontFile::reset()
{
	_Texture   = 0;
	_CurX      = 0;
	_CurY      = 0;
	_CellX	   = 0;
	_CellY     = 0;
	_RowFactor = 0;
	_ColFactor = 0;
	_OffsetY   = 0;
	_RowPitch  = 0;
	_Base      = 0;
	_RenderStyle = 0;
}
//
#ifdef _KRAZY_DISABLE_PREV_FONT_READING
bool KrazyBitmapFontFile::load(string _file)
{
  char *dat,*img;
  unsigned long fileSize(_KRAZY_FONT_FILE_SIZE);
  char bpp;
  int ImgX,ImgY;
  //
#ifndef _WINDOWS
  KrazyAsset _asset;
  if(!_asset.open_asset(_file.c_str() , 0)){
	  return false;
  }
  
  // allocate space for file data
 
 int _read = _asset.read_asset(dat , fileSize);
  _asset.close_asset();
#else
    string data;
	ifstream  myfile;
    myfile.open (_file);
	string line;
    if (myfile.is_open()){
		dat = new char[fileSize];
		myfile.read(dat , fileSize);
	}
    myfile.close();
#endif
  // Check ID is 'BFF2'
  if((unsigned char)dat[0]!=0xBF || (unsigned char)dat[1]!=0xF2){
     delete [] dat;
     return false;
  }
  // Grab the rest of the header
  memcpy(&ImgX,&dat[2],sizeof(int));
  memcpy(&ImgY,&dat[6],sizeof(int));
  memcpy(&_CellX,&dat[10],sizeof(int));
  memcpy(&_CellY,&dat[14],sizeof(int));
  //
  bpp   = dat[18];
  _Base = dat[19];
   // Check filesize
   if(fileSize!=((MAP_DATA_OFFSET)+((ImgX*ImgY)*(bpp/8))))
	   return false;

  // Calculate font params
  _RowPitch  = ImgX/_CellX;
  _ColFactor = (float)_CellX/(float)ImgX;
  _RowFactor = (float)_CellY/(float)ImgY;
  _OffsetY   = _CellY;
  // Determine blending options based on BPP
   switch(bpp)
    {
     case 8: // Greyscale
      _RenderStyle = BFG_RS_ALPHA;
      break;

     case 24: // RGB
      _RenderStyle = BFG_RS_RGB;
      break;

     case 32: // RGBA
      _RenderStyle = BFG_RS_RGBA;
      break;

     default: // Unsupported BPP
      delete [] dat;
      return false;
      break;
    }
  // Allocate space for image
  img=new char[(ImgX*ImgY)*(bpp/8)];

   if(!img)
    {
     delete [] dat;
     return false;
    }
  // Grab char widths
  memcpy(_Width,&dat[WIDTH_DATA_OFFSET],256);
  // Grab image data
  memcpy(img,&dat[MAP_DATA_OFFSET],(ImgX*ImgY)*(bpp/8));
  // Create Texture
  glGenTextures(1,&_Texture);
  glBindTexture(GL_TEXTURE_2D,_Texture);
  // Fonts should be rendered at native resolution so no need for texture filtering
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  // Stop chararcters from bleeding over edges
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
  // Tex creation params are dependent on BPP
   switch(_RenderStyle)
    {
     case BFG_RS_ALPHA:
      glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,ImgX,ImgY,0,GL_LUMINANCE,GL_UNSIGNED_BYTE,img);
      break;

     case BFG_RS_RGB:
      glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,ImgX,ImgY,0,GL_RGB,GL_UNSIGNED_BYTE,img);
      break;

     case BFG_RS_RGBA:
      glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,ImgX,ImgY,0,GL_RGBA,GL_UNSIGNED_BYTE,img);
      break;
    }
   switch(glGetError()){
   case GL_INVALID_ENUM:break;
   case GL_INVALID_VALUE:break;
   case GL_INVALID_OPERATION:break;
   case GL_NO_ERROR:break;
   case GL_OUT_OF_MEMORY:break;
   }
  // Clean up
  delete [] img;
  delete [] dat;
  return true;
}
#else
bool KrazyBitmapFontFile::load(string _file)
{
  char *dat,*img;
  fstream in;
  unsigned long fileSize;
  char bpp;
  int ImgX,ImgY;
  //
  in.open(_file.c_str(), ios_base::binary | ios_base::in);
  //
   if(in.fail())
    return false;
  // Get Filesize
  in.seekg(0,ios_base::end);
  fileSize=in.tellg();
  in.seekg(0,ios_base::beg);
  // allocate space for file data
  dat = new char[fileSize];
  // Read filedata
   if(!dat)
    return false;
   //.........................
   in.read(dat,fileSize);
   //...............................
   if(in.fail()){
     delete [] dat;
     in.close();
     return false;
    }
  //................................................
  in.close();
  // Check ID is 'BFF2'
  if((unsigned char)dat[0]!=0xBF || (unsigned char)dat[1]!=0xF2){
     delete [] dat;
     return false;
  }
  // Grab the rest of the header
  memcpy(&ImgX,&dat[2],sizeof(int));
  memcpy(&ImgY,&dat[6],sizeof(int));
  memcpy(&_CellX,&dat[10],sizeof(int));
  memcpy(&_CellY,&dat[14],sizeof(int));
  //
  bpp   = dat[18];
  _Base = dat[19];
   // Check filesize
   if(fileSize!=((MAP_DATA_OFFSET)+((ImgX*ImgY)*(bpp/8))))
	   return false;

  // Calculate font params
  _RowPitch  = ImgX/_CellX;
  _ColFactor = (float)_CellX/(float)ImgX;
  _RowFactor = (float)_CellY/(float)ImgY;
  _OffsetY   = _CellY;
  // Determine blending options based on BPP
   switch(bpp)
    {
     case 8: // Greyscale
      _RenderStyle = BFG_RS_ALPHA;
      break;

     case 24: // RGB
      _RenderStyle = BFG_RS_RGB;
      break;

     case 32: // RGBA
      _RenderStyle = BFG_RS_RGBA;
      break;

     default: // Unsupported BPP
      delete [] dat;
      return false;
      break;
    }
  // Allocate space for image
  img=new char[(ImgX*ImgY)*(bpp/8)];

   if(!img)
    {
     delete [] dat;
     return false;
    }
  // Grab char widths
  memcpy(_Width,&dat[WIDTH_DATA_OFFSET],256);
  // Grab image data
  memcpy(img,&dat[MAP_DATA_OFFSET],(ImgX*ImgY)*(bpp/8));
  // Create Texture
  glGenTextures(1,&_Texture);
  glBindTexture(GL_TEXTURE_2D,_Texture);
  // Fonts should be rendered at native resolution so no need for texture filtering
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  // Stop chararcters from bleeding over edges
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
  // Tex creation params are dependent on BPP
   switch(_RenderStyle)
    {
     case BFG_RS_ALPHA:
      glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,ImgX,ImgY,0,GL_LUMINANCE,GL_UNSIGNED_BYTE,img);
      break;

     case BFG_RS_RGB:
      glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,ImgX,ImgY,0,GL_RGB,GL_UNSIGNED_BYTE,img);
      break;

     case BFG_RS_RGBA:
      glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,ImgX,ImgY,0,GL_RGBA,GL_UNSIGNED_BYTE,img);
      break;
    }
   switch(glGetError()){
   case GL_INVALID_ENUM:break;
   case GL_INVALID_VALUE:break;
   case GL_INVALID_OPERATION:break;
   case GL_NO_ERROR:break;
   case GL_OUT_OF_MEMORY:break;
   }
  // Clean up
  delete [] img;
  delete [] dat;
  return true;
}
#endif
//
unsigned int KrazyBitmapFontFile::get_texture()   { return _Texture; }
int KrazyBitmapFontFile::get_CurX()				  { return _CurX; }
int KrazyBitmapFontFile::get_CurY()				  { return _CurY; }
int KrazyBitmapFontFile::get_OffsetY()		      { return _OffsetY;}
int KrazyBitmapFontFile::get_RowPitch()           { return _RowPitch;}
float KrazyBitmapFontFile::get_RowFactor()        { return _RowFactor;}
float KrazyBitmapFontFile::get_ColFactor()        { return _ColFactor;}
//..
void  KrazyBitmapFontFile::get_texture_list_from_textlist(KrazyTextArea* _text_area)
{
	KrazyTextList* _text_list = _text_area->get_TextList();
	int _count = _text_list->size();
	KrazyFloatList* _rect = _text_area->get_Rect();
	float _curx = _rect->at(0);
	float _cury = _rect->at(1);
	float _limx = _rect->at(2);
	float _cyList = 0.0f;
	for(int i = 0; i < _count; i++){
		KrazyText* _text = _text_list->at(i);
		KrazyFloatList* _t_rect  = _text->get_Rect();_t_rect->clear();
		_t_rect->push_back(_curx);_t_rect->push_back(_cury);
		_t_rect->push_back(_limx);
		KrazyFloatList* _list_uv = _text->get_UVList(); _list_uv->clear();
		KrazyFloatList* _list_vertex = _text->get_VList(); _list_vertex->clear();
		string _t_text   = _text->text();
		int len = _t_text.size();
		for(int ch = 0; ch < len; ch++){
			char _ch = _t_text.at(ch);
			int _row = (_ch - _Base )/_RowPitch;
			int _col = (_ch - _Base) - _row * _RowPitch;
			float  _u  = _col *  _ColFactor;
			float  _v  = _row * _RowFactor;
			float  _u1 = _u + _ColFactor;
			float  _v1 = _v + _RowFactor;
			//..........................................................................................................
			_list_uv->push_back(_u); _list_uv->push_back(_v1); 
			_list_uv->push_back(_u1);_list_uv->push_back(_v1);
			_list_uv->push_back(_u1);_list_uv->push_back(_v);
			//...				
			_list_uv->push_back(_u); _list_uv->push_back(_v); 
			_list_uv->push_back(_u1);_list_uv->push_back(_v);
			_list_uv->push_back(_u); _list_uv->push_back(_v1); 
			//..........................................................................................................
			float _cx  = _curx;
			float _cy  = _cury;
			//..,
			float _abs_x = abs(KrazyCaertsianConverter::get_x(_CellX   ,  KrazyViewPortContext::_Width));
			float _abs_y = abs(KrazyCaertsianConverter::get_x(_CellY   ,  KrazyViewPortContext::_Height));
			float _x = (1.0f - _abs_x ) ;
			float _y = (1.0f - _abs_y ) ;
			float _cx1 = _curx + _x;
			float _cy1 = _cyList = _cury + _y;
			//..,//...,
			_list_vertex->push_back(_cx);_list_vertex->push_back(_cy);
			_list_vertex->push_back(_cx1);_list_vertex->push_back(_cy);
			_list_vertex->push_back(_cx1);_list_vertex->push_back(_cy1);
			//..		
			_list_vertex->push_back(_cx);_list_vertex->push_back(_cy1);
			_list_vertex->push_back(_cx1);_list_vertex->push_back(_cy1);
			_list_vertex->push_back(_cx);_list_vertex->push_back(_cy);
			//..........................................................................................................
			_abs_x = abs(KrazyCaertsianConverter::get_x(int(_Width[_ch])  ,  KrazyViewPortContext::_Width));
			_x = 1.0f - _abs_x ;
			_curx += _x; 
			//..,..........................................................................................................
			if(_curx >= _limx){
				_curx = _t_rect->at(0);
				_cury = _cury - ((float(_OffsetY) * _KRAZY_TEXT_PIXEL) + (5*_KRAZY_TEXT_PIXEL));
			}
		}
		_t_rect->push_back(_cyList);
		_text->calculate_bkrect();
	}
	_rect->push_back(_cyList);
}
//..,
void  KrazyBitmapFontFile::get_texture_list_from_text(KrazyTextArea* _text_area)
{
	string _text = _text_area->get_Text();
	KrazyFloatList* _list_uv = _text_area->get_UVList(); _list_uv->clear();
	KrazyFloatList* _list_vertex = _text_area->get_VertexList(); _list_vertex->clear();
	//..
	KrazyFloatList* _rect = _text_area->get_Rect();
	int _len = _text.size();
	float _curx = _rect->at(0);
	float _cury = _rect->at(1);
	float _limx = _rect->at(2);
	float _cyList = 0.0f;
	//..,
	for(int i = 0; i < _len; i++){
		char _ch = _text.at(i);
		int _row = (_ch - _Base )/_RowPitch;
		int _col = (_ch - _Base) - _row * _RowPitch;
		float  _u  = _col *  _ColFactor;
		float  _v  = _row * _RowFactor;
		float  _u1 = _u + _ColFactor;
        float  _v1 = _v + _RowFactor;
		//..........................................................................................................
		_list_uv->push_back(_u); _list_uv->push_back(_v1); 
		_list_uv->push_back(_u1);_list_uv->push_back(_v1);
		_list_uv->push_back(_u1);_list_uv->push_back(_v);
		//...				
		_list_uv->push_back(_u); _list_uv->push_back(_v); 
		_list_uv->push_back(_u1);_list_uv->push_back(_v);
		_list_uv->push_back(_u); _list_uv->push_back(_v1); 
		//..........................................................................................................
		float _cx  = _curx;
		float _cy  = _cury;
		//..,
		float _abs_x = abs(KrazyCaertsianConverter::get_x(_CellX   ,  KrazyViewPortContext::_Width));
		float _abs_y = abs(KrazyCaertsianConverter::get_x(_CellY   ,  KrazyViewPortContext::_Height));
		float _x = (1.0f - _abs_x ) ;
		float _y = (1.0f - _abs_y ) ;
		float _cx1 = _curx + _x;
		float _cy1 = _cyList = _cury + _y;
		//..,//...,
		_list_vertex->push_back(_cx);_list_vertex->push_back(_cy);
		_list_vertex->push_back(_cx1);_list_vertex->push_back(_cy);
		_list_vertex->push_back(_cx1);_list_vertex->push_back(_cy1);
		//..		
		_list_vertex->push_back(_cx);_list_vertex->push_back(_cy1);
		_list_vertex->push_back(_cx1);_list_vertex->push_back(_cy1);
		_list_vertex->push_back(_cx);_list_vertex->push_back(_cy);
		//..........................................................................................................
		_abs_x = abs(KrazyCaertsianConverter::get_x(int(_Width[_ch])  ,  KrazyViewPortContext::_Width));
		_x = 1.0f - _abs_x ;
		_curx += _x; 
		//..,..........................................................................................................
		if(_curx >= _limx){
			_curx = _rect->at(0);
			_cury = _cury - ((float(_OffsetY) * _KRAZY_TEXT_PIXEL) + (5*_KRAZY_TEXT_PIXEL));
		}
	}
	//..,
	_rect->push_back(_cyList);
}
//.................................................................................................................................
KrazyText::KrazyText()
{
}
//..,
KrazyText::~KrazyText()
{
}
//..,
void KrazyText::on_setText(string _text)
{
	_Text = _text;
}
//..,
void KrazyText::on_setBkColor(KrazyFloatList& _bkcolor)
{
	_KRAZY_COPY_LIST(_bkcolor , _BkColor);
}
//..,
void KrazyText::on_setColor(KrazyFloatList& _color)
{
	_KRAZY_COPY_LIST(_color , _Color);
}
//..,
void KrazyText::calculate_bkrect()
{
	/*_V1*/_BkRect.push_back(_Rect[2]);_BkRect.push_back(_Rect[1]);
	/*_V2*/_BkRect.push_back(_Rect[2]);_BkRect.push_back(_Rect[3]);
	/*_V3*/_BkRect.push_back(_Rect[0]);_BkRect.push_back(_Rect[1]);
	//..,
	/*_V2*/_BkRect.push_back(_Rect[2]);_BkRect.push_back(_Rect[3]);
	/*_V4*/_BkRect.push_back(_Rect[0]);_BkRect.push_back(_Rect[3]);
	/*_V3*/_BkRect.push_back(_Rect[0]);_BkRect.push_back(_Rect[1]);
}
//...............................................................................................................................
string& KrazyText::text()				{ return _Text;     }
KrazyFloatList* KrazyText::get_UVList() { return &_UVList;  }
KrazyFloatList* KrazyText::get_VList()  { return &_VList;   }
KrazyFloatList* KrazyText::get_Rect()	{ return &_Rect;    }
KrazyFloatList* KrazyText::get_BkRect() { return &_BkRect;  }
KrazyFloatList* KrazyText::color()      { return &_Color;   }
KrazyFloatList* KrazyText::bkcolor()    { return &_BkColor; }
//.................................................................................................................................
KrazyTextArea::KrazyTextArea() :
_Callback(0)
{
	init();
	reset();
}
//..,
KrazyTextArea::~KrazyTextArea()
{
  cleanUp();
}
//..,
void KrazyTextArea::cleanUp()
{
	int _count = _KrazyTextList.size();
	for(int i = 0; i < _count; i++){
		KrazyText* _text = _KrazyTextList[i];
		delete _text;_text = 0;
	}
	//..,
	_KrazyTextList.clear();

}
//..,
void KrazyTextArea::init()
{
	_ScaleList.clear();
	for(int i = 0; i < 2; i++)
		_ScaleList.push_back(1.0f);

}
//..,
bool& KrazyTextArea::enable_touch()							 { return _EnableTouch; }
bool& KrazyTextArea::parent_will_handle_touch()              { return _Parent_wiil_Handle_Touch; }
//...,	
void KrazyTextArea::on_append_text(KrazyText* _text)
{
	_KrazyTextList.push_back(_text);
}
//....,
KrazyBool KrazyTextArea::on_touch(float _x , float _y)
{
	glm::vec2 _left_top;
	glm::vec2 _right_bottom;
	//..,
	float scale_x = _ScaleList[0];float scale_y = _ScaleList[1];
	if(!_UseTouchRect){
		 _left_top = glm::vec2(_Rect[0] , _Rect[1]);
		 _right_bottom = glm::vec2(_Rect[2] , _Rect[3]);
	} 
	else{
		_left_top = glm::vec2(_TouchRect[0] , _TouchRect[1]);
		_right_bottom = glm::vec2(_TouchRect[2] , _TouchRect[3]);
	}
	//..,
	float _width  =  float(KrazyViewPortContext::_Width);
	float _height =  float(KrazyViewPortContext::_Height);
	float aspect = _width/_height;
	float left   = -1.0f * aspect;
	float right  =  1.0f * aspect;
	float bottom = -1.0f;
	float top    =  1.0f;
	float near_p = -1.0f;
	float far_p  =  1.0f;
	//..,
	float x = 0.0f;float y = 0.0f;
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
	if(!_UseTouchRect){
		if( (_x > _left_top.x && _y > _left_top.y) && (_x < _right_bottom.x && _y < _right_bottom.y)){
#if 1
			KrazyWindowController::getDefault()->on_textarea_touched(this);
#else
			if(_Callback)
				_Callback(this);
#endif
			return KRAZY_BOOL_True;
		}
	}
	else{
		if((_x < _right_bottom.x && _x > _left_top.x) && (_y < _left_top.y && _y > _right_bottom.y)){
			KrazyWindowController::getDefault()->on_textarea_touched(this);
			return KRAZY_BOOL_True;
		}
	}
	//..,
	return KRAZY_BOOL_False;
}
//.....
void KrazyTextArea::reset()
{
	_EnableTouch  = false;
	_UseTouchRect = false;
	_Parent_wiil_Handle_Touch = false;
	_CalculationMode = KRAZY_TEXTAREA_CALCULATION_mode_Normal;
	_UVList.clear();
	_VertexList.clear();
	_Rect.clear();
	_Text.clear();
	_BackgroundDrawBuffer.clear();
	_BacKGroundColor.clear();
	_ForeColor.clear();
}
//...........................................................
KrazyFloatList* KrazyTextArea::get_UVList()      { return &_UVList;     }
KrazyFloatList* KrazyTextArea::get_VertexList()  { return &_VertexList; }
KrazyIntList*	KrazyTextArea::get_iUVList()     { return &_iUVList;    }
KrazyIntList*	KrazyTextArea::get_iVertexList() { return &_iVertexList;}
KrazyFloatList* KrazyTextArea::get_Rect()        { return &_Rect;       }
KrazyFloatList* KrazyTextArea::get_BackGroundDrawBuffer() { return &_BackgroundDrawBuffer; }
KrazyFloatList* KrazyTextArea::get_BackGroundColor() { return &_BacKGroundColor; }
KrazyFloatList* KrazyTextArea::get_ForeGroundColor() { return &_ForeColor; }
KrazyFloatList* KrazyTextArea::get_Scale()		 { return &_ScaleList; }
string&			KrazyTextArea::get_Text()        { return _Text;        }
bool		    KrazyTextArea::empty()			 { return ( _Text.empty() && _KrazyTextList.size() == 0 ); } 
KrazyTextList*  KrazyTextArea::get_TextList()    { return &_KrazyTextList; }
//.........................................................
void KrazyTextArea::on_setRect(KrazyFloatList& rect)
{
	_KRAZY_COPY_LIST(rect , _Rect);
}
//
void KrazyTextArea::on_setBackGroundColor(KrazyFloatList& _color)
{
	_KRAZY_COPY_LIST(_color , _BacKGroundColor);
}
//
void KrazyTextArea::on_setForeGroundColor(KrazyFloatList& _color)
{
	_KRAZY_COPY_LIST(_color , _ForeColor);
}
//
void KrazyTextArea::on_setScale(KrazyFloatList& _scale)
{
	_KRAZY_COPY_LIST(_scale , _ScaleList);
}
//
void KrazyTextArea::on_setText(string text)
{
	_Text = text;
}
//
void KrazyTextArea::on_setBackground(KrazyFloatList& _rect_backround)
{
	_CalculationMode = KRAZY_TEXTAREA_CALCULATION_mode_Copy_from_Given;
	_KRAZY_COPY_LIST(_rect_backround , _BackgroundDrawBuffer);
}
//
void  KrazyTextArea::on_setTouchRect(KrazyFloatList& _rect_touch)
{
	_UseTouchRect = true;
	_KRAZY_COPY_LIST(_rect_touch , _TouchRect);
}
//
void KrazyTextArea::on_attachCallback(_KrazyOnTouch _callback)
{
	_Callback = _callback;
}
//..,
void KrazyTextArea::fill_rect(KrazyFloatList& rect)
{
	_KRAZY_COPY_LIST(rect , _Rect);
	
}
//
void KrazyTextArea::on_calculateBackgroundBuffer()
{
	switch(_CalculationMode){
	case KRAZY_TEXTAREA_CALCULATION_mode_Normal         : calculate_normal_background(); break;
		KRAZY_TEXTAREA_CALCULATION_mode_Copy_from_Given : break;
	}
}
//..,
void KrazyTextArea::calculate_normal_background()
{
	/*_V1*/_BackgroundDrawBuffer.push_back(_Rect[2]);_BackgroundDrawBuffer.push_back(_Rect[1]);
	/*_V2*/_BackgroundDrawBuffer.push_back(_Rect[2]);_BackgroundDrawBuffer.push_back(_Rect[3]);
	/*_V3*/_BackgroundDrawBuffer.push_back(_Rect[0]);_BackgroundDrawBuffer.push_back(_Rect[1]);
	//..,
	/*_V2*/_BackgroundDrawBuffer.push_back(_Rect[2]);_BackgroundDrawBuffer.push_back(_Rect[3]);
	/*_V4*/_BackgroundDrawBuffer.push_back(_Rect[0]);_BackgroundDrawBuffer.push_back(_Rect[3]);
	/*_V3*/_BackgroundDrawBuffer.push_back(_Rect[0]);_BackgroundDrawBuffer.push_back(_Rect[1]);
}
//..,
void KrazyTextArea::copy_given_background()
{
}
//..
void KrazyTextArea::calculate_textarea()
{
	KrazyBitmapFontFile::getDefault()->get_texture_list_from_text(this);
	on_calculateBackgroundBuffer();
}
//..,
void KrazyTextArea::calculate_textlist()
{
	KrazyBitmapFontFile::getDefault()->get_texture_list_from_textlist(this);
	on_calculateBackgroundBuffer();
}
//........................................................................
KrazyTextAreaRendrer* KrazyTextAreaRendrer::_Me = 0;
//..,
KrazyTextAreaRendrer* KrazyTextAreaRendrer::getDefault()
{
	if(!_Me){
		_Me = new KrazyTextAreaRendrer;
		_Me->create();
	}
	//..,
	return _Me;
}
//..,
void KrazyTextAreaRendrer::releaseDefault()
{
	if(_Me){
		delete _Me;_Me = 0;
	}
}
//..,
KrazyTextAreaRendrer::KrazyTextAreaRendrer(void)
{
	_FontFile = KrazyBitmapFontFile::getDefault();
	reset();
}
//
KrazyTextAreaRendrer::~KrazyTextAreaRendrer(void)
{
	//KrazyBitmapFontFile::releaseDefault();_FontFile = 0;
	_Programme.reset();
	_ProgrammeBkg.reset();
}
//
void KrazyTextAreaRendrer::reset()
{
	_Position			= 0;   
	_Projection			= 0;  
	_ModelView			= 0;  
	_ScreenScale		= 0; 
	_Color				= 0;  
	_TextureId			= 0; 
	_TextureCoords		= 0;
	_PositionBkg		= 0;      
	_TextureCoordsBkg   = 0;
	_ProjectionBkg		= 0; 
	_ModelViewBkg		= 0; 
	_ScreenScaleBkg		= 0; 
	_TextureIdBkg		= 0; 
	_ColorBkg			= 0; 
}
//
void KrazyTextAreaRendrer::on_resize()
{
}
//
void KrazyTextAreaRendrer::create()
{
	//............................................................................
	char vertex_shader[] = 
	"precision mediump float;\n"
	"attribute vec2   a_vPosition;\n"
	"uniform mat4     u_mProjection;\n"
	"uniform mat4     u_mModelView;\n"
	"uniform mat4     u_mScreenScale;\n"
    "uniform vec4     u_vColor;\n"
	"attribute vec2   a_vTexCoord;\n"
	"varying vec2     v_vTexCoord;\n"
	"varying vec4     v_vColor;\n"
	"void main()\n"
	"{\n"
		"gl_Position = u_mProjection  * u_mScreenScale * u_mModelView * vec4(a_vPosition , 0.0 , 1.0);\n"
		//"gl_Position = u_mProjection  *  vec4(a_vPosition , 0.0 , 1.0);\n"
		"v_vTexCoord = a_vTexCoord;\n"
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
	"gl_FragColor = texture2D(s_texture , v_vTexCoord) * v_vColor;\n"
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
	//........................................................
	char fragment_shader_bkg[] = 
	"precision mediump float;\n"
	"varying vec4       v_vColor;\n"
	"varying vec2       v_vTexCoord;\n"
	"uniform sampler2D  s_texture;\n"
	"void main()\n"
	"{\n"
	"gl_FragColor = v_vColor;\n"
	"}\n";
	//..
	KrazyShader vertexShader_bkg;vertexShader_bkg.LoadShader(GL_VERTEX_SHADER,vertex_shader);
    KrazyShader fragmentShader_bkg;fragmentShader_bkg.LoadShader(GL_FRAGMENT_SHADER,fragment_shader_bkg);
	//..,
	_ProgrammeBkg.create(vertexShader,fragmentShader_bkg);
	_ProgrammeBkg.compile();
	//..,
	_PositionBkg      = glGetAttribLocation (_ProgrammeBkg , "a_vPosition");
	_TextureCoordsBkg = glGetAttribLocation(_ProgrammeBkg ,  "a_vTexCoord");
	_ProjectionBkg    = glGetUniformLocation(_ProgrammeBkg , "u_mProjection");
	_ModelViewBkg     = glGetUniformLocation(_ProgrammeBkg , "u_mModelView");
	_ScreenScaleBkg   = glGetUniformLocation(_ProgrammeBkg , "u_mScreenScale");
	_TextureIdBkg     = glGetUniformLocation(_ProgrammeBkg , "s_texture");
	_ColorBkg         = glGetUniformLocation(_ProgrammeBkg , "u_vColor");
}
//	
void KrazyTextAreaRendrer::render(KrazyTextArea* _text_area)
{		
	/*There is a problem with background color and multiple lines.., */ 
	float _width  =  float(KrazyViewPortContext::_Width);
	float _height =  float(KrazyViewPortContext::_Height);
	//.............................................................
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
	float bottom = _height; //0.0f;
	float top    =  0.0f;
	float near   = -1.0f;
	float far    =  1.0f;
#endif
	//..
	KrazyTextList* _t_list = _text_area->get_TextList();
	int _kt_count = _t_list->size();
	if(_kt_count > 0){
		float _x = 0.0f;float _y = 0.0f;
		float scale_x = 1.0f;float scale_y = 1.0f;
		//..
		glm::mat4 _projectionMatrix = glm::ortho(left , right , bottom , top , near , far);
		glm::mat4 _windowScaleMatrix = glm::mat4((aspect/* - 0.015f*/), 0.0f , 0.0f , 0.0f, /*make this adjustment from crazy wheel editor*/
			0.0f , 1.0 , 0.0f , 0.0f,
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
		glm::mat4 scale = glm::mat4(scale_x , 0.0f    , 0.0f ,0.0f,
			0.0f   , scale_y , 0.0f ,0.0f,
			0.0f   , 0.0f    , 1.0f ,0.0f,
			0.0f   , 0.0f    , 0.0f ,1.0f);

		glUseProgram(_ProgrammeBkg);
		glUniformMatrix4fv(_ProjectionBkg, 1, false, glm::value_ptr(_projectionMatrix));
		glUniformMatrix4fv(_ScreenScaleBkg, 1, false, glm::value_ptr(_windowScaleMatrix));
		glUniformMatrix4fv(_ModelViewBkg, 1, false, glm::value_ptr(view * scale));
		//................ background ..........................................................
		for(int i = 0; i < _kt_count; i++){
			KrazyText* _kt_ = _t_list->at(i);
			KrazyFloatList* _bk_color_ = _kt_->bkcolor();
			KrazyFloatList* _bk_rect_  = _kt_->get_BkRect();
			glUniform4fv(_ColorBkg , 1 , &_bk_color_->at(0));
			glVertexAttribPointer(_PositionBkg, 2, GL_FLOAT, GL_FALSE, 0, &_bk_rect_->at(0));
			glEnableVertexAttribArray(_PositionBkg);
			glDrawArrays(GL_TRIANGLES, 0 , _bk_rect_->size()/2);
			glDisableVertexAttribArray(_PositionBkg);
		}
		glUseProgram(0);
		//............. Font Rendering ..........................................
		glUseProgram(_Programme);
		glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _FontFile->get_texture());
		glUniform1i(_TextureId, 0);

		glUniformMatrix4fv(_Projection, 1, false, glm::value_ptr(_projectionMatrix));
		glUniformMatrix4fv(_ScreenScale, 1, false, glm::value_ptr(_windowScaleMatrix));
		glUniformMatrix4fv(_ModelView , 1, false, glm::value_ptr(view * scale));
		//..,
		for(int i = 0; i < _kt_count; i++){
			KrazyText* _kt_ = _t_list->at(i);
			KrazyFloatList* _bk_color_ = _kt_->bkcolor();
			KrazyFloatList* _bk_rect_  = _kt_->get_BkRect();
			
			KrazyFloatList* _color   = _kt_->color();
			KrazyFloatList* _v_list  = _kt_->get_VList();
			KrazyFloatList* _uv_list = _kt_->get_UVList();
			
#if 0
			glUniform4fv(_Color , 1 , &_bk_color_->at(0));
			glVertexAttribPointer(_Position, 2, GL_FLOAT, GL_FALSE, 0, &_bk_rect_->at(0));
			glEnableVertexAttribArray(_Position);
			glDrawArrays(GL_TRIANGLES, 0 , _bk_rect_->size()/2);
			glDisableVertexAttribArray(_Position);
#endif

			glUniform4fv(_Color , 1 , &_color->at(0));
			glVertexAttribPointer(_Position, 2, GL_FLOAT, GL_FALSE, 0, &_v_list->at(0));
			glEnableVertexAttribArray(_Position);
			glVertexAttribPointer(_TextureCoords, 2, GL_FLOAT, GL_FALSE, 0, &_uv_list->at(0));
			glEnableVertexAttribArray(_TextureCoords);
			glDrawArrays(GL_TRIANGLES, 0 , _v_list->size()/2);
			glDisableVertexAttribArray(_Position);
			glDisableVertexAttribArray(_TextureCoords);
		}
		glUseProgram(0);

	}
	else{
		KrazyFloatList* _verts = _text_area->get_VertexList();
		KrazyFloatList* _scale = _text_area->get_Scale();
		KrazyFloatList* _bkgDrawBuffer = _text_area->get_BackGroundDrawBuffer();
		//........
		float _x = 0.0f;float _y = 0.0f;
		float scale_x = 1.0f;float scale_y = 1.0f;
#if 0
		if(_scale->size()){
			scale_x = _scale->at(0);
			scale_y = _scale->at(1);
		}
#endif
		glm::mat4 _projectionMatrix = glm::ortho(left , right , bottom , top , near , far);
		glm::mat4 _windowScaleMatrix = glm::mat4((aspect/* - 0.015f*/), 0.0f , 0.0f , 0.0f, /*make this adjustment from crazy wheel editor*/
			0.0f , 1.0 , 0.0f , 0.0f,
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
		glm::mat4 scale = glm::mat4(scale_x , 0.0f    , 0.0f ,0.0f,
			0.0f   , scale_y , 0.0f ,0.0f,
			0.0f   , 0.0f    , 1.0f ,0.0f,
			0.0f   , 0.0f    , 0.0f ,1.0f);
		//..,
		glUseProgram(_ProgrammeBkg);
		glUniformMatrix4fv(_ProjectionBkg, 1, false, glm::value_ptr(_projectionMatrix));
		glUniformMatrix4fv(_ScreenScaleBkg, 1, false, glm::value_ptr(_windowScaleMatrix));
		glUniformMatrix4fv(_ModelViewBkg, 1, false, glm::value_ptr(view * scale));
		glUniform4fv(_ColorBkg , 1 , &_text_area->get_BackGroundColor()->at(0));
		glVertexAttribPointer(_PositionBkg, 2, GL_FLOAT, GL_FALSE, 0, &_bkgDrawBuffer->at(0));
		glEnableVertexAttribArray(_PositionBkg);
		glDrawArrays(GL_TRIANGLES, 0 , _bkgDrawBuffer->size()/2);
		glDisableVertexAttribArray(_PositionBkg);
		glUseProgram(0);
		//............. Font Rendering ..........................................
		glUseProgram(_Programme);
		glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _FontFile->get_texture());
		glUniform1i(_TextureId, 0);

		glUniformMatrix4fv(_Projection, 1, false, glm::value_ptr(_projectionMatrix));
		glUniformMatrix4fv(_ScreenScale, 1, false, glm::value_ptr(_windowScaleMatrix));
		glUniformMatrix4fv(_ModelView , 1, false, glm::value_ptr(view * scale));
		//..,
#if 0
		glUniform4fv(_Color , 1 , &_text_area->get_BackGroundColor()->at(0));
		glVertexAttribPointer(_Position, 2, GL_FLOAT, GL_FALSE, 0, &_bkgDrawBuffer->at(0));
		glEnableVertexAttribArray(_Position);
		glDrawArrays(GL_TRIANGLES, 0 , _bkgDrawBuffer->size()/2);
		glDisableVertexAttribArray(_Position);
#endif

		glUniform4fv(_Color , 1 , &_text_area->get_ForeGroundColor()->at(0));
		glVertexAttribPointer(_Position, 2, GL_FLOAT, GL_FALSE, 0, &_verts->at(0));
		glEnableVertexAttribArray(_Position);
		glVertexAttribPointer(_TextureCoords, 2, GL_FLOAT, GL_FALSE, 0, &_text_area->get_UVList()->at(0));
		glEnableVertexAttribArray(_TextureCoords);
		glDrawArrays(GL_TRIANGLES, 0 , _verts->size()/2);
		glDisableVertexAttribArray(_Position);
		glDisableVertexAttribArray(_TextureCoords);
	}
}
//.............................................................................................