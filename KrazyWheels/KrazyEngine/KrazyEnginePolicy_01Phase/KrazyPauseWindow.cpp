#include "stdafx.h"
#include "KrazyPauseWindow.h"
#include "KrazyColorDefinations.h"
#include "KrazyAudioThriver.h"
#include "KrazyBackgroundTrack.h"
//..,
static float _krazy_pause_wnd[] = 
{
	0.3f , 0.4f,
   -0.3f , 0.4f,
    0.3f ,-0.4f,
   -0.3f , 0.4f,
   -0.3f ,-0.4f,
	0.3f ,-0.4f
};
//..,
static float _krazy_pause_client[] = 
{
	0.28f , 0.38f,
   -0.28f , 0.38f,
    0.28f ,-0.38f,
   -0.28f , 0.38f,
   -0.28f ,-0.38f,
	0.28f ,-0.38f
};
//..,
KrazyPauseWindow::KrazyPauseWindow()
{
}
//..,
KrazyPauseWindow::~KrazyPauseWindow()
{
}
//..,
void KrazyPauseWindow::create(KrazyWindowStyle _ws_ /*= KRAZY_WS_None*/)
{
	KrazyWindow::create(_ws_);
	on_load();
}
//..,
void KrazyPauseWindow::on_load()
{
	KrazyFloatList  _scale , _rect;
	_scale.push_back(1.5f);_scale.push_back(1.0f);
	on_setWndRect(_krazy_pause_wnd);
	on_setClientRect(_krazy_pause_client);
	//..,
	KrazyFloatList  _red    = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Red);
	KrazyFloatList  _green  = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Green);
	KrazyFloatList  _yellow = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Yellow);
	KrazyFloatList  _black  = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Black);
	KrazyFloatList  _white  = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_White);
	KrazyFloatList  _blue   = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Blue);
	//..,
	KrazyTextArea*  _textarea = 0;
	_rect.clear();_rect.push_back(_krazy_pause_client[2]);
	_rect.push_back(_krazy_pause_client[3]);_rect.push_back(_krazy_pause_client[0]);
	_textarea = new KrazyTextArea();
	_textarea->on_setText(_KRAZY_GAME_TITLE_Pause);
	_textarea->on_setRect(_rect);
	_textarea->on_setBackGroundColor(_blue);
	_textarea->on_setForeGroundColor(_white);
	_textarea->calculate_textarea();
	_TextAreaList.push_back(_textarea);
	//..,
	_rect.clear();_rect.push_back(_krazy_pause_client[2]);
	_rect.push_back(_krazy_pause_client[3] - 0.2f);_rect.push_back(_krazy_pause_client[0] );
	_textarea = new KrazyTextArea();
	_textarea->enable_touch() = true;
	_textarea->on_attachCallback(_Callback);
	_textarea->on_setText(_KRAZY_RESUME);
	_textarea->on_setRect(_rect);
	_textarea->on_setBackGroundColor(_blue);
	_textarea->on_setForeGroundColor(_white);
	_textarea->calculate_textarea();
	_TextAreaList.push_back(_textarea);
	//..,
	_rect.clear();_rect.push_back(_krazy_pause_client[2]);
	_rect.push_back(_krazy_pause_client[3] - 0.4f);_rect.push_back(_krazy_pause_client[0] );
	_textarea = new KrazyTextArea();
	_textarea->enable_touch() = true;
	_textarea->on_attachCallback(_Callback);
	_textarea->on_setText(_KRAZY_MENU);
	_textarea->on_setRect(_rect);
	_textarea->on_setBackGroundColor(_blue);
	_textarea->on_setForeGroundColor(_white);
	_textarea->calculate_textarea();
	_TextAreaList.push_back(_textarea);
	//..
	_rect.clear();_rect.push_back(_krazy_pause_client[2]);
	_rect.push_back(_krazy_pause_client[3] - 0.6f);_rect.push_back(_krazy_pause_client[0] );
	_textarea = new KrazyTextArea();
	_textarea->enable_touch() = true;
	_textarea->on_attachCallback(_Callback);
#ifndef _WINDOWS
	string _mute_ = (KrazyAudioThriver::getDefault()->stopped()) ? _KRAZY_GAME_MUSIC_UNMUTE : _KRAZY_GAME_MUSIC_MUTE;
#else
	string _mute_ = (KrazyBackgroundTrack::getDefault()->is_stopped()) ? _KRAZY_GAME_MUSIC_UNMUTE : _KRAZY_GAME_MUSIC_MUTE;
#endif
	_textarea->on_setText(_mute_);
	_textarea->on_setRect(_rect);
	_textarea->on_setBackGroundColor(_blue);
	_textarea->on_setForeGroundColor(_white);
	_textarea->calculate_textarea();
	_TextAreaList.push_back(_textarea);
	
}