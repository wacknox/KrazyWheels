#pragma once
#include "KrazyTextRendrer.h"
#include "KrazyProgramme.h"
#include "KrazyPrimitiveDataCollections.h"
#include "KrazyKallbacks.h"
//..........................................................
#define _KRAZY_WINDOW_VERTICAL_OFFSET   0.1f
#define _KRAZY_BEGNING_OFFSET           0.025f
//......................................
#define _KRAZY_GAME_MUSIC_MUTE     string("                     Mute")
#define _KRAZY_GAME_MUSIC_UNMUTE   string("                    UnMute")
//......................................
const unsigned short KRAZY_WINDOW_STATEPARAM_Id = 1000; 
struct KrazyWindowStateParam
{
	KrazyWindowStateParam(unsigned short _id) : 
    _Id(_id)
	{
	}
	//..,
	unsigned short _Id;
};
//.........................................
enum KrazyWindowStyle
{
	KRAZY_WS_None    ,
	KRAZY_WS_Tool    ,
	KRAZY_WS_Message
};
//..,
class KrazyWindowRendrer;
class KrazyWindow
{
//..,
public:
#ifdef _KRAZY_FOR_VS
	static KrazyWindow* getDefault(bool _create = false);
	static void releaseDefault();
#endif
	KrazyWindow(void);
	virtual ~KrazyWindow(void);
	
	void on_setWndRect(float* _rect);
	void on_setClientRect(float* _rect);
	void on_setPoint(KrazyFloatList& _point);
	void on_setScale(KrazyFloatList& _scale);
	void on_resize();
	//..,
	virtual void create(KrazyWindowStyle _ws_ = KRAZY_WS_None);
	virtual void render();
	virtual KrazyBool on_touch(float _x , float _y);
	virtual void on_refresh();
	//..
	KrazyWindowStyle& get_WS();
	KrazyFloatList*   get_WndRect();
	KrazyFloatList*   get_ClientRect();
	KrazyFloatList*   get_Point();
	KrazyFloatList*	  get_Scale();
	//..
	
	//..,
	//..
	void on_attachCallback(_KrazyOnTouch _callback)
	{ 
		_Callback = _callback; 
	}
	//..,
protected:
	void init();
	KrazyTextAreaList      _TextAreaList;
	KrazyTextAreaRendrer*  _TextAreaRendrer;
	KrazyWindowRendrer*    _WindowRendrer;
	KrazyWindowStyle       _WS;
	KrazyFloatList         _WndRect;
	KrazyFloatList         _ClientRect;
	KrazyFloatList         _Point;
	KrazyFloatList		   _ScaleList;
	//..............................................................
#ifdef _KRAZY_FOR_VS
	static KrazyWindow*  _Me;
#endif
	_KrazyOnTouch          _Callback;
};
//
//class KrazyWindow;
class KrazyWindowRendrer
{
//....,
private:
	KrazyWindowRendrer();
	~KrazyWindowRendrer();
//......
public:
	static KrazyWindowRendrer* getDefault();
	static void releaseDefault();
	//..,
	void create();
	void render(KrazyWindow* _window);
	void reset();
//........
private:
	KrazyProgramme  _Programme;
	unsigned int	_Position      ;
	unsigned int	_TextureCoords ;
	unsigned int	_Projection    ;
	unsigned int    _ModelView     ;
	unsigned int    _ScreenScale   ;
	unsigned int    _TextureId     ;
	unsigned int    _Color         ;
	static KrazyWindowRendrer*     _Me;
};
//

//..,

