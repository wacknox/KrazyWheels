#pragma once
#include "KrazyPrimitiveDataCollections.h"
#include "KrazyProgramme.h"
#include "KrazyGLMInclusion.h"
#include "KrazyEnums.h"
#include "KrazyKallbacks.h"
//...
#define BFG_RS_NONE  0x0      // Blend flags
#define BFG_RS_ALPHA 0x1
#define BFG_RS_RGB   0x2
#define BFG_RS_RGBA  0x4

#define BFG_MAXSTRING 255     // Maximum string length

#define WIDTH_DATA_OFFSET  20 // Offset to width data with BFF file
#define MAP_DATA_OFFSET   276 // Offset to texture image data with BFF file

// This definition is missing from some GL libs
#ifndef GL_CLAMP_TO_EDGE 
#define GL_CLAMP_TO_EDGE 0x812F 
#endif
//
#define _KRAZY_TEXT_PIXEL	0.0025f

class KrazyText
{
public:
	KrazyText();
	virtual ~KrazyText();
	void on_setText(string _text);
	void on_setColor(KrazyFloatList&   _color);
	void on_setBkColor(KrazyFloatList& _bkcolor);
//.................
	string& text();
	KrazyFloatList* get_UVList();
	KrazyFloatList* get_VList();
	KrazyFloatList* get_Rect();
	KrazyFloatList* get_BkRect();
	KrazyFloatList* color();
	KrazyFloatList* bkcolor();
//.........................
	void calculate_bkrect();
private:
	string			_Text;
	KrazyFloatList  _UVList;
	KrazyFloatList  _VList;
	KrazyFloatList  _Rect;
	KrazyFloatList  _BkRect;
	KrazyFloatList  _Color;
	KrazyFloatList  _BkColor;
};
//...
class KrazyTextList : public vector<KrazyText*> {};

class KrazyTextArea;
class KrazyBitmapFontFile
{
public:
	static KrazyBitmapFontFile* getDefault();
	static void releaseDefault();
	//
	bool load(string _file);
	unsigned int get_texture();
	int   get_CurX();
	int   get_CurY();
	int   get_OffsetY();
	int   get_RowPitch();
	float get_RowFactor();
	float get_ColFactor();
	void  get_texture_list_from_text(KrazyTextArea* _text_area);
	void  get_texture_list_from_textlist(KrazyTextArea* _text_area);
private:
	KrazyBitmapFontFile();
	~KrazyBitmapFontFile();
	void reset();
	//.......................................................
	unsigned int  _Texture;
	int			  _CurX;
	int			  _CurY;
	int           _CellX;
	int			  _CellY;
    int			  _OffsetY;
	int			  _RowPitch;
	float		  _RowFactor;
	float		  _ColFactor;
	char		  _Base;
	int			  _RenderStyle;
	char		  _Width[256];   
	//.......................................................
	static KrazyBitmapFontFile* _Me;
};
//
enum KrazyTextAreaCalculationMode
{
	KRAZY_TEXTAREA_CALCULATION_mode_Normal , 
	KRAZY_TEXTAREA_CALCULATION_mode_Copy_from_Given ,
};
//
class KrazyTextArea
{
public:
	KrazyTextArea();
	virtual ~KrazyTextArea();
	//...........................................................
	void	calculate_textarea();
	void	calculate_textlist();
	void	reset();
	bool    empty();
	//......................
	bool& enable_touch();
	bool& parent_will_handle_touch();
	//...........................................................
	KrazyBool on_touch(float _x , float _y);
	//...........................................................
	KrazyFloatList* get_UVList();
	KrazyFloatList* get_VertexList();
	KrazyIntList* get_iUVList();
	KrazyIntList* get_iVertexList();
	KrazyFloatList* get_Rect();
	KrazyFloatList* get_BackGroundDrawBuffer();
	KrazyFloatList* get_BackGroundColor();
	KrazyFloatList* get_ForeGroundColor();
	KrazyFloatList* get_Scale();
	KrazyFloatList* getTouchRect();
	string&         get_Text();
	KrazyTextList*  get_TextList();
	//.........................................................
	void      on_setRect(KrazyFloatList& rect);
	void      on_setText(string text);
	void      on_setBackGroundColor(KrazyFloatList& _color);
	void      on_setForeGroundColor(KrazyFloatList& _color);
	void      on_setScale(KrazyFloatList& scale);
	void      on_attachCallback(_KrazyOnTouch _callback);
	void      on_setBackground(KrazyFloatList& _rect_backround);
	void      on_setTouchRect(KrazyFloatList& _rect_touch);
	void      on_append_text(KrazyText* _text);
private:
	//..,
	void cleanUp();
	void init();
	void fill_rect(KrazyFloatList& rect);
	void on_calculateBackgroundBuffer();
	//...................................................
	void calculate_normal_background();
	void copy_given_background();
	//...................................................
	KrazyFloatList  _BacKGroundColor;
	KrazyFloatList  _BackgroundDrawBuffer;
	KrazyFloatList  _ForeColor;
	KrazyFloatList  _Rect;
	KrazyFloatList	_UVList;
	KrazyFloatList	_VertexList;
	KrazyFloatList	_ScaleList;
	KrazyFloatList  _TouchRect;
	//...................................................
	KrazyIntList  _iUVList;
	KrazyIntList  _iVertexList;
	//...................................................
	string			_Text;
	bool			_EnableTouch;
	bool            _UseTouchRect;
	bool            _Parent_wiil_Handle_Touch;
	_KrazyOnTouch   _Callback;
	KrazyTextAreaCalculationMode  _CalculationMode;
	KrazyTextList    _KrazyTextList;
};
//
class KrazyTextAreaList : public vector<KrazyTextArea*>{};
//
class KrazyTextAreaRendrer
{
//..,
private:
	KrazyTextAreaRendrer(void);
	virtual ~KrazyTextAreaRendrer(void);
	//..,
public:
	static KrazyTextAreaRendrer* getDefault();
	static void releaseDefault();
	//..,
	void create();
	void reset();
	void on_resize();
	void render(KrazyTextArea* _text_area);
//..,
private:
	KrazyProgramme		 _Programme;
	KrazyProgramme		 _ProgrammeBkg;
	KrazyBitmapFontFile* _FontFile;
	unsigned int  _Position;   
	unsigned int  _Projection;  
	unsigned int  _ModelView;  
	unsigned int  _ScreenScale; 
	unsigned int  _Color;  
	unsigned int  _TextureId;
	unsigned int  _TextureCoords;
	unsigned int  _PositionBkg;      
	unsigned int  _TextureCoordsBkg ;
	unsigned int  _ProjectionBkg   ; 
	unsigned int  _ModelViewBkg    ; 
	unsigned int  _ScreenScaleBkg  ; 
	unsigned int  _TextureIdBkg    ; 
	unsigned int  _ColorBkg        ; 
	static KrazyTextAreaRendrer* _Me;
};
//.......................................
