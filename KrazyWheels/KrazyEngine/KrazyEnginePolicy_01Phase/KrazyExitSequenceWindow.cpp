#include "stdafx.h"
#include "KrazyExitSequenceWindow.h"
#include "KrazyColorDefinations.h"
#include "KrazyLevelThriver.h"
//..,
static float _krazy_half_exit_sequence_wnd[] = 
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
static float _krazy_client_half_exit_sequencewnd[] = 
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
static float _krazy_touch_area_exit_sequence_[] = 
{
	 0.14f ,  -0.25f, //v1
    -0.14f ,  -0.14f, //v2
     0.14f ,  -0.25f, //v3
	
	-0.14f ,  -0.14f,  //v2
	-0.14f ,  -0.25f,  //v4	
	 0.14f ,  -0.25f   //v1
};
KrazyExitSequenceWindow::KrazyExitSequenceWindow(void)
{
}
//..,
KrazyExitSequenceWindow::~KrazyExitSequenceWindow(void)
{
}
//..,
void KrazyExitSequenceWindow::create(KrazyWindowStyle _ws_ /*= KRAZY_WS_None*/)
{
	KrazyWindow::create(_ws_);
	on_load();
}
//..
void KrazyExitSequenceWindow::render()
{
	KrazyWindow::render();
}
//.
KrazyBool KrazyExitSequenceWindow::on_touch(float _x , float _y)
{
	return KrazyWindow::on_touch(_x , _y);
}
//..,
void KrazyExitSequenceWindow::on_refresh()
{
	KrazyWindow::on_refresh();
}
//..,
void KrazyExitSequenceWindow::on_load()
{
	_ScaleList.clear();
	_ScaleList.push_back(1.0f);_ScaleList.push_back(1.0f);
	string _title_text;
	string _touch_text;
	KrazyFloatList _rect;
	KrazyTextArea* _textarea = 0;

	KrazyFloatList  _red    = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Red);
	KrazyFloatList  _green  = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Green);
	KrazyFloatList  _yellow = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Yellow);
	KrazyFloatList  _black  = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Black);
	KrazyFloatList  _white  = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_White);
	KrazyFloatList  _blue   = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Blue);
	//..,
	_title_text = _KRAZY_EXIT_SEQUENCE;
	_rect.clear();_rect.push_back(_krazy_touch_area_exit_sequence_[2]);
	_rect.push_back(_krazy_touch_area_exit_sequence_[3] - 0.1f);_rect.push_back(_krazy_touch_area_exit_sequence_[0]);
	_textarea = new KrazyTextArea(/*_KRAZY_TOUCH_REPLAY*/);
	//_textarea->on_attachCallback(_Callback);
	_textarea->enable_touch() = true;
	_textarea->on_setText(_KRAZY_EXIT_SEQUENCE_GAMEON);
	_textarea->on_setRect(_rect);
	_textarea->on_setBackGroundColor(_black);
	_textarea->on_setForeGroundColor(_yellow);
	_textarea->calculate_textarea();
	_TextAreaList.push_back(_textarea);
	//..,
	on_setWndRect(_krazy_half_exit_sequence_wnd);
	on_setClientRect(_krazy_client_half_exit_sequencewnd);
	//..,
	_rect.clear();
	_rect.push_back(_krazy_half_exit_sequence_wnd[2] + _KRAZY_BEGNING_OFFSET);
	_rect.push_back(_krazy_half_exit_sequence_wnd[3] - _KRAZY_WINDOW_VERTICAL_OFFSET);
	_rect.push_back(_krazy_client_half_exit_sequencewnd[0]);
	//..,
	_textarea = new KrazyTextArea;
	_textarea->on_setText(_title_text);
	_textarea->on_setRect(_rect);
	_textarea->on_setBackGroundColor(_blue);
	_textarea->on_setForeGroundColor(_white);
	_textarea->calculate_textarea();
	_TextAreaList.push_back(_textarea);
	//..,
	KrazyFloatList* _rect_text = _textarea->get_Rect();
	float _y = _rect_text->at(1) - 0.175f;//_KRAZY_TEXT_PIXEL;
	_rect.clear();
	_rect.push_back(_krazy_half_exit_sequence_wnd[2] + _KRAZY_BEGNING_OFFSET);
	_rect.push_back(_y);
	_rect.push_back(_krazy_client_half_exit_sequencewnd[0]);
	//..,
	string _msg = KrazyLevelThriver::get_default()->exit_sequencetext();
	_textarea = new KrazyTextArea;
	_textarea->on_setText(_msg);
	_textarea->on_setRect(_rect);
	_textarea->on_setBackGroundColor(_black);
	_textarea->on_setForeGroundColor(_white);
	_textarea->calculate_textarea();
	_TextAreaList.push_back(_textarea);
//..,
}