#include "stdafx.h"
#include "KrazyLevelThriverWindow.h"
#include "KrazyColorDefinations.h"
//..,
static float _krazy_half_wnd[] = 
{
	0.4f ,  0.3f,
   -0.4f ,  0.3f,
    0.4f , -0.3f,
	//..,
	-0.4f ,  0.3f,
	-0.4f , -0.3f,
	 0.4f , -0.3f,
};
//..,
static float _krazy_client_half_wnd[] = 
{
	0.38f ,  0.28f,
   -0.38f ,  0.28f,
    0.38f , -0.28f,
	//..,
	-0.38f ,  0.28f,
	-0.38f , -0.28f,
	 0.38f , -0.28f,
};
//..,
static float _krazy_touch_area_level_gameon_[] = 
{
	 0.14f ,  -0.25f, //v1
    -0.14f ,  -0.14f, //v2
     0.14f ,  -0.25f, //v3
	
	-0.14f ,  -0.14f,  //v2
	-0.14f ,  -0.25f,  //v4	
	 0.14f ,  -0.25f   //v1
};
//..,
KrazyLevelThriverWindow::KrazyLevelThriverWindow(KrazyLevelThriverWindowType _typ , string& _msg) :
 _WndType(_typ)
,_Msg(_msg)
{
	
}
//..,
KrazyLevelThriverWindow::~KrazyLevelThriverWindow(void)
{
}
//...,
void KrazyLevelThriverWindow::create(KrazyWindowStyle _ws_ /*= KRAZY_WS_None*/)
{
	KrazyWindow::create(_ws_);
	on_load();
}
//...,
#if 0
void KrazyLevelThriverWindow::render()
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
#endif
//...,
void KrazyLevelThriverWindow::on_load()
{
	_ScaleList.clear();
	_ScaleList.push_back(1.0f);_ScaleList.push_back(1.0f);
	string _title_text;
	string _touch_text;
	KrazyFloatList _rect;
	KrazyTextArea* _textarea = 0;
	KrazyFloatList* _bkg_color = 0;
	KrazyFloatList  _red    = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Red);
	KrazyFloatList  _green  = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Green);
	KrazyFloatList  _yellow = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Yellow);
	KrazyFloatList  _black  = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Black);
	KrazyFloatList  _white  = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_White);
	KrazyFloatList  _blue   = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Blue);
	switch(_WndType){
	case KRAZY_LEVEL_THRIVER_WND_TYPE_GameOver      : 
		 _title_text = _KRAZY_GAME_OVER;
		 _bkg_color  = &_red;//_KRAZY_RED;
		 _touch_text = _KRAZY_TOUCH_REPLAY;
		 _rect.clear();_rect.push_back(_krazy_touch_area_level_gameon_[2]);
		 _rect.push_back(_krazy_touch_area_level_gameon_[3] - 0.1f);_rect.push_back(_krazy_touch_area_level_gameon_[0]);
		 _textarea = new KrazyTextArea(/*_KRAZY_TOUCH_REPLAY*/);
		 _textarea->on_attachCallback(_Callback);
		 _textarea->enable_touch() = true;
		 _textarea->on_setText(_KRAZY_TOUCH_REPLAY);
		 _textarea->on_setRect(_rect);
		 _textarea->on_setBackGroundColor(_black);
		 _textarea->on_setForeGroundColor(_yellow);
		 _textarea->calculate_textarea();
		 _TextAreaList.push_back(_textarea);
		 break;
	case KRAZY_LEVEL_THRIVER_WND_TYPE_LevelComplete : 
		_title_text = _KRAZY_LEVEL_COMPLETE;
		_bkg_color  = &_blue;
		_touch_text = _KRAZY_TOUCH_NEXT;
		_rect.clear();_rect.push_back(_krazy_touch_area_level_gameon_[2]);
		_rect.push_back(_krazy_touch_area_level_gameon_[3] - 0.1f);_rect.push_back(_krazy_touch_area_level_gameon_[0]);
		_textarea = new KrazyTextArea(/*_KRAZY_TOUCH_NEXT*/);
		_textarea->on_attachCallback(_Callback);
		_textarea->enable_touch() = true;
		_textarea->on_setText(_KRAZY_TOUCH_NEXT);
		_textarea->on_setRect(_rect);
		_textarea->on_setBackGroundColor(_black);
		_textarea->on_setForeGroundColor(_green);
		_textarea->calculate_textarea();
		_TextAreaList.push_back(_textarea);
		break;
	}
	//..,
	on_setWndRect(_krazy_half_wnd);
	on_setClientRect(_krazy_client_half_wnd);
	//..,
    _rect.clear();
	_rect.push_back(_krazy_half_wnd[2] + _KRAZY_BEGNING_OFFSET);
	_rect.push_back(_krazy_half_wnd[3] - _KRAZY_WINDOW_VERTICAL_OFFSET);
	_rect.push_back(_krazy_client_half_wnd[0]);
	//..,
	_textarea = new KrazyTextArea;
	_textarea->on_setText(_title_text);
	_textarea->on_setRect(_rect);
	_textarea->on_setBackGroundColor(*_bkg_color);
	_textarea->on_setForeGroundColor(_white);
	_textarea->calculate_textarea();
	_TextAreaList.push_back(_textarea);
	//..,
	KrazyFloatList* _rect_text = _textarea->get_Rect();
	float _y = _rect_text->at(1) - 0.175f;//_KRAZY_TEXT_PIXEL;
	_rect.clear();
	_rect.push_back(_krazy_half_wnd[2] + _KRAZY_BEGNING_OFFSET);
	_rect.push_back(_y);
	_rect.push_back(_krazy_client_half_wnd[0] - 0.03f);
	//..,
	//_Msg = "Bangatoona was expected to Exit instead of Rangtoona";
	_textarea = new KrazyTextArea;
	_textarea->on_setText(_Msg);
	_textarea->on_setRect(_rect);
	_textarea->on_setBackGroundColor(_black);
	_textarea->on_setForeGroundColor(_white);
	_textarea->calculate_textarea();
	_TextAreaList.push_back(_textarea);
//..,
}
//

