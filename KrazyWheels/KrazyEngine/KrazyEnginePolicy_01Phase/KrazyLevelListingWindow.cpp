#include "stdafx.h"
#include "KrazyLevelListingWindow.h"
#include "KrazyTextRendrer.h"
#include "KrazyColorDefinations.h"
//..,
#if 1
static  float _krazy_lisiting_wnd[] = {
	0.95f ,  0.95f ,
   -0.95f ,  0.95f ,
    0.95f , -0.95f ,
	//..,
	-0.95f ,   0.95f ,
	-0.95f ,  -0.95f ,
	 0.95f , -0.95f 
};
//
static  float _krazy_lisiting_client[] = {
	0.9f ,   0.9f ,
   -0.9f ,   0.9f ,
    0.9f ,  -0.9f ,
	//..,
	-0.9f ,   0.9f ,
    -0.9f ,  -0.9f ,
	 0.9f ,  -0.9f 
};
#else
static  float _krazy_lisiting_wnd[] = {
	 1.0f ,  1.0f ,
	-1.0f ,  1.0f ,
	 1.0f , -1.0f ,
	-1.0f ,  1.0f ,
    -1.0f , -1.0f ,
	 1.0f , -1.0f 
};
//
static  float _krazy_lisiting_client[] = {
	0.8f ,  0.8f ,
   -0.8f ,  0.8f ,
	0.8f , -0.8f ,
   -0.8f ,  0.8f ,
   -0.8f , -0.8f ,
	0.8f , -0.8f
};
#endif
//..,
KrazyLevelListingWindow::KrazyLevelListingWindow(void)
{
}
//..,
KrazyLevelListingWindow::~KrazyLevelListingWindow(void)
{
}
//..,
void KrazyLevelListingWindow::create(KrazyWindowStyle _ws_ /*= KRAZY_WS_None*/)
{
	KrazyWindow::create(_ws_);
	on_load();
}
//..,
void KrazyLevelListingWindow::on_load()
{
	//KrazyFloatList _scale;_scale.push_back(1.0f);_scale.push_back(1.5f);
	on_setWndRect(_krazy_lisiting_wnd);
	on_setClientRect(_krazy_lisiting_client);
	//on_setScale(_scale);
	//..,
	float _start_x = _krazy_lisiting_client[2] + 0.25f;
	float _start_y = _krazy_lisiting_client[3] - 0.2f;
	//..,
	KrazyFloatList  _yellow = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Yellow);
	KrazyFloatList  _black  = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Black);
	KrazyFloatList  _red    = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Red);
	KrazyFloatList  _white  = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_White);
	KrazyFloatList  _blue   = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Blue);
	//..,
	for(int i = 0; i < _KRAZY_MAX_LEVELS; i++){
		int _text_index = i + 1;
		//..,
		char sz[256];sprintf(sz , "%d" , _text_index);
		string _text ( sz );
		KrazyFloatList _rect_wnd , _rect_text , _rect_touch;
		float _x  = _start_x;
		float _y  = _start_y;
		float _x1 = _start_x + 0.2f;
		float _y1 = _start_y - 0.2f; 
		/*v1..*/_rect_wnd.push_back(_start_x + 0.2f);_rect_wnd.push_back(_start_y);
		/*v2..*/_rect_wnd.push_back(_start_x);_rect_wnd.push_back(_start_y);
		/*v4..*/_rect_wnd.push_back(_start_x + 0.2f);_rect_wnd.push_back(_start_y - 0.2f);
		/*v2..*/_rect_wnd.push_back(_start_x);_rect_wnd.push_back(_start_y);
		/*v3..*/_rect_wnd.push_back(_start_x);_rect_wnd.push_back(_start_y - 0.2f);
		/*v4..*/_rect_wnd.push_back(_start_x + 0.2f);_rect_wnd.push_back(_start_y - 0.2f);
		//..,
		_rect_text.push_back(_start_x + 0.075f);_rect_text.push_back(_start_y - 0.15f);
		_rect_text.push_back(_start_x + 0.5f);
		//..,
		_rect_touch.push_back(_start_x);_rect_touch.push_back(_start_y);
		_rect_touch.push_back(_start_x + 0.2f);_rect_touch.push_back(_start_y - 0.2f);
		//..,
		KrazyTextArea* _textarea = new KrazyTextArea;
		_textarea->enable_touch() = true;
		_textarea->on_attachCallback(_Callback);
		_textarea->on_setText(_text);
		_textarea->on_setRect(_rect_text);
		_textarea->on_setTouchRect(_rect_touch);
		_textarea->on_setBackground(_rect_wnd);
		_textarea->on_setBackGroundColor(_blue);
		_textarea->on_setForeGroundColor(_white);
	//	_textarea->on_setScale(_scale);
		_textarea->calculate_textarea();
		_TextAreaList.push_back(_textarea);
		_start_x = _start_x + 0.25f;
		if(_text_index % 5 == 0){
			_start_y =  _start_y - 0.4f;
			_start_x = _krazy_lisiting_client[2] + 0.25f;
		}
		//..,
	}
	//..,
	KrazyFloatList _rect;
	_rect.push_back(-0.1f);_rect.push_back(_start_y - 0.5f);
	_rect.push_back(0.8f);
	KrazyTextArea* _textarea = new KrazyTextArea();
	_textarea->enable_touch() = true;
	_textarea->on_setText(_KRAZY_MAINMENU_LL);
	_textarea->on_setRect(_rect);
	_textarea->on_setBackGroundColor(_black);
	_textarea->on_setForeGroundColor(_yellow);
	_textarea->calculate_textarea();
	_TextAreaList.push_back(_textarea);
}
//..,