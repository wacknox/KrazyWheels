#include "stdafx.h"
#include "KrazyWindowPreferance.h"
#include "KrazyColorDefinations.h"
#include "KrazyAudioThriver.h"
#include "KrazyBackgroundTrack.h"
//..,
static float _krazy_pref_wnd[] = 
{
	0.3f , 0.5f,
   -0.3f , 0.5f,
    0.3f ,-0.5f,
   -0.3f , 0.5f,
   -0.3f ,-0.5f,
	0.3f ,-0.5f
};
//..,
static float _krazy_pref_client[] = 
{
	0.28f , 0.48f,
   -0.28f , 0.48f,
    0.28f ,-0.48f,
   -0.28f , 0.48f,
   -0.28f ,-0.48f,
	0.28f ,-0.48f
};
//..,
KrazyWindowPreferance::KrazyWindowPreferance(void)
{
}
//..,
KrazyWindowPreferance::~KrazyWindowPreferance(void)
{
}
//..,
void KrazyWindowPreferance::create(KrazyWindowStyle _ws_ /*= KRAZY_WS_None*/)
{
	KrazyWindow::create(_ws_);
	on_load();
}
//..,
void KrazyWindowPreferance::on_load()
{
	KrazyFloatList  _scale , _rect;
	_scale.push_back(1.5f);_scale.push_back(1.0f);
	on_setWndRect(_krazy_pref_wnd);
	on_setClientRect(_krazy_pref_client);
//	on_setScale(_scale);
	//..,
	KrazyFloatList  _red    = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Red);
	KrazyFloatList  _green  = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Green);
	KrazyFloatList  _yellow = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Yellow);
	KrazyFloatList  _black  = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Black);
	KrazyFloatList  _white  = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_White);
	KrazyFloatList  _blue   = KrazyFloatListColorDispensor::getDefault()->getColor(KRAZY_COLOR_Blue);
	//..,
	KrazyTextArea*  _textarea = 0;
	_rect.clear();_rect.push_back(_krazy_pref_client[2]);
	_rect.push_back(_krazy_pref_client[3]);_rect.push_back(_krazy_pref_client[0]);
	_textarea = new KrazyTextArea();
	_textarea->on_setText(_KRAZY_GAME_TITLE);
	_textarea->on_setRect(_rect);
	_textarea->on_setBackGroundColor(_blue);
	_textarea->on_setForeGroundColor(_white);
//	_textarea->on_setScale(_scale);
	_textarea->calculate_textarea();
	_TextAreaList.push_back(_textarea);
	//..,
	_rect.clear();_rect.push_back(_krazy_pref_client[2]);
	_rect.push_back(_krazy_pref_client[3] - 0.2f);_rect.push_back(_krazy_pref_client[0] );
	_textarea = new KrazyTextArea();
	_textarea->enable_touch() = true;
	_textarea->on_attachCallback(_Callback);
	_textarea->on_setText(_KRAZY_GAME_PLAY);
	_textarea->on_setRect(_rect);
	_textarea->on_setBackGroundColor(_blue);
	_textarea->on_setForeGroundColor(_white);
//	_textarea->on_setScale(_scale);
	_textarea->calculate_textarea();
	_TextAreaList.push_back(_textarea);
	//..,
	_rect.clear();_rect.push_back(_krazy_pref_client[2]);
	_rect.push_back(_krazy_pref_client[3] - 0.4f);_rect.push_back(_krazy_pref_client[0] );
	_textarea = new KrazyTextArea();
	_textarea->enable_touch() = true;
	_textarea->on_attachCallback(_Callback);
	_textarea->on_setText(_KRAZY_GAME_MOVES);
	_textarea->on_setRect(_rect);
	_textarea->on_setBackGroundColor(_blue);
	_textarea->on_setForeGroundColor(_white);
//	_textarea->on_setScale(_scale);
	_textarea->calculate_textarea();
	_TextAreaList.push_back(_textarea);
	//..
	_rect.clear();_rect.push_back(_krazy_pref_client[2]);
	_rect.push_back(_krazy_pref_client[3] - 0.6f);_rect.push_back(_krazy_pref_client[0] );
	_textarea = new KrazyTextArea();
	_textarea->enable_touch() = true;
	_textarea->on_attachCallback(_Callback);
	_textarea->on_setText(_KRAZY_GAME_STORY);
	_textarea->on_setRect(_rect);
	_textarea->on_setBackGroundColor(_blue);
	_textarea->on_setForeGroundColor(_white);
//	_textarea->on_setScale(_scale);
	_textarea->calculate_textarea();
	_TextAreaList.push_back(_textarea);
	//..,
	_rect.clear();_rect.push_back(_krazy_pref_client[2]);
	_rect.push_back(_krazy_pref_client[3] - 0.8f);_rect.push_back(_krazy_pref_client[0] );
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
//	_textarea->on_setScale(_scale);
	_textarea->calculate_textarea();
	_TextAreaList.push_back(_textarea);

}
//..,

