#include "stdafx.h"
#include <assert.h>
#include "KrazyAudioThriver.h"
#include "KrazyAsset.h"
#ifndef _WINDOWS
#define _KRAZY_GAME_ON_MUSIC			  string("music/Wheels.mp3")
#if 0
#define _KRAZY_SHERRIF_CHASE_ON_MUSIC     string("music/Time_Is_Tight.mp3")
#endif
#define _KRAZY_MAX_NUMBER                 1
//..,
KrazyAudioThriver* KrazyAudioThriver::_Me = 0;
//..,
KrazyAudio::KrazyAudio(string _fileName) :
_FileName(_fileName)
{
}
//..,
KrazyAudio::~KrazyAudio()
{

}
//..,
void KrazyAudio::destroy()
{
	
    // destroy file descriptor audio player object, and invalidate all associated interfaces
    if (_PlayerObject != NULL) {
        (*_PlayerObject)->Destroy(_PlayerObject);
        _PlayerObject = NULL;
        _PlayerPlay = NULL;
        _PlayerSeek = NULL;
        _PlayerMuteSolo = NULL;
        _PlayerVolume = NULL;
    }
	
}
//...,
void KrazyAudio::create(SLEngineItf _engine , SLObjectItf _output_mix_)
{
	SLresult result;
	KrazyAsset _asset;
	if(_asset.open_asset(_FileName.c_str())){
		off_t _start , _length;
		int _fd = _asset.open_filedescriptor(&_start , &_length);
		assert(0 <= _fd);
		_asset.close_asset();

		SLDataLocator_AndroidFD loc_fd = {SL_DATALOCATOR_ANDROIDFD, _fd, _start, _length};
		SLDataFormat_MIME format_mime = {SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED};
		SLDataSource audioSrc = {&loc_fd, &format_mime};
		 // configure audio sink
		SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, _output_mix_};
		SLDataSink audioSnk = {&loc_outmix, NULL};
		// create audio player
		const SLInterfaceID ids[3] = {SL_IID_SEEK, SL_IID_MUTESOLO, SL_IID_VOLUME};
		const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
		result = (*_engine)->CreateAudioPlayer(_engine, &_PlayerObject, &audioSrc, &audioSnk,
            3, ids, req);
		assert(SL_RESULT_SUCCESS == result);
		(void)result;
		// realize the player
		result = (*_PlayerObject)->Realize(_PlayerObject, SL_BOOLEAN_FALSE);
		assert(SL_RESULT_SUCCESS == result);
		(void)result;
		// get the play interface
		result = (*_PlayerObject)->GetInterface(_PlayerObject, SL_IID_PLAY, &_PlayerPlay);
		assert(SL_RESULT_SUCCESS == result);
		(void)result;
		// get the seek interface
		result = (*_PlayerObject)->GetInterface(_PlayerObject, SL_IID_SEEK, &_PlayerSeek);
		assert(SL_RESULT_SUCCESS == result);
		(void)result;
		// get the mute/solo interface
		result = (*_PlayerObject)->GetInterface(_PlayerObject, SL_IID_MUTESOLO, &_PlayerMuteSolo);
		assert(SL_RESULT_SUCCESS == result);
		(void)result;
		// get the volume interface
		result = (*_PlayerObject)->GetInterface(_PlayerObject, SL_IID_VOLUME, &_PlayerVolume);
		assert(SL_RESULT_SUCCESS == result);
		(void)result;
		// enable whole file looping
		result = (*_PlayerSeek)->SetLoop(_PlayerSeek, SL_BOOLEAN_TRUE, 0, SL_TIME_UNKNOWN);
		assert(SL_RESULT_SUCCESS == result);
		(void)result;
		
	}
}
//..,
void KrazyAudio::play()
{
	 SLresult result;
    // make sure the asset audio player was created
    if (NULL != _PlayerPlay) {
        // set the player's state
        result = (*_PlayerPlay)->SetPlayState(_PlayerPlay , SL_PLAYSTATE_PLAYING);
        assert(SL_RESULT_SUCCESS == result);
        (void)result;
    }
}
//..,
void KrazyAudio::pause()
{
	 SLresult result;
    // make sure the asset audio player was created
    if (NULL != _PlayerPlay) {
        // set the player's state
        result = (*_PlayerPlay)->SetPlayState(_PlayerPlay , SL_PLAYSTATE_PAUSED);
        assert(SL_RESULT_SUCCESS == result);
        (void)result;
    }
}
//..,,
void KrazyAudio::stop()
{
	 SLresult result;
    // make sure the asset audio player was created
    if (NULL != _PlayerPlay) {
        // set the player's state
        result = (*_PlayerPlay)->SetPlayState(_PlayerPlay , SL_PLAYSTATE_STOPPED);
        assert(SL_RESULT_SUCCESS == result);
        (void)result;
    }
}
//..
KrazyAudioThriver* KrazyAudioThriver::getDefault()
{
	if(!_Me){
		_Me = new KrazyAudioThriver;
		_Me->Initialize();
	}
	//..,
	return _Me;
}
//..,
void KrazyAudioThriver::releaseDefault()
{
	if(_Me){
		delete _Me;_Me = 0;
	}
}
//..,
KrazyAudioThriver::KrazyAudioThriver() :
_Stopped(false)
{
}
//..,
KrazyAudioThriver::~KrazyAudioThriver()
{
	destroy();
}
//..,
void KrazyAudioThriver::destroy()
{
	int _count = _AudioList.size();
	for(int i = 0; i < _count; i++){
		KrazyAudio* _audio = _AudioList[i];
		_audio->destroy();
		delete _audio;_audio = 0;
	}
	//..,
	_AudioList.clear();
	// destroy output mix object, and invalidate all associated interfaces
    if (_OutputMixObject != NULL) {
        (*_OutputMixObject)->Destroy(_OutputMixObject);
        _OutputMixObject = NULL;
    }
    // destroy engine object, and invalidate all associated interfaces
    if (_EngineObject != NULL) {
        (*_EngineObject)->Destroy(_EngineObject);
        _EngineObject = NULL;
        _Engine = NULL;
    }
}
//..,
bool& KrazyAudioThriver::stopped() { return _Stopped; }
//..,
void KrazyAudioThriver::Initialize()
{
	 SLresult result;
    // create engine
    result = slCreateEngine(&_EngineObject, 0, NULL, 0, NULL, NULL);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

    // realize the engine
    result = (*_EngineObject)->Realize(_EngineObject, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

    // get the engine interface, which is needed in order to create other objects
    result = (*_EngineObject)->GetInterface(_EngineObject, SL_IID_ENGINE, &_Engine);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

    // create output mix, with environmental reverb specified as a non-required interface
    const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean req[1] = {SL_BOOLEAN_FALSE};
    result = (*_Engine)->CreateOutputMix(_Engine, &_OutputMixObject, 1, ids, req);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

    // realize the output mix
    result = (*_OutputMixObject)->Realize(_OutputMixObject, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;
	//..,
	for(int i = 0; i < _KRAZY_MAX_NUMBER; i++){
		string _filename;
		switch(i)
		{
		case 0 : _filename = _KRAZY_GAME_ON_MUSIC;	         break;
#if 0
		case 1 : _filename = _KRAZY_SHERRIF_CHASE_ON_MUSIC;  break;
#endif
		}
		//..,
		KrazyAudio* _audio = new KrazyAudio(_filename);
		_audio->create(_Engine , _OutputMixObject);
		_AudioList.push_back(_audio);
	}
	//..,
	_CurrentIndex = 0;
}
//..,
void KrazyAudioThriver::on_changeMusic(unsigned int _index)
{
	if(_index + 1 > _AudioList.size())
		return;

	on_stop();
	_CurrentIndex  = _index;
	_NumberChanged = KRAZY_BOOL_True;
	on_play();
}
//..,
KrazyBool KrazyAudioThriver::number_changed()
{
	return _NumberChanged;
}
//.
void KrazyAudioThriver::on_play()
{
	_AudioList[_CurrentIndex]->play();
	_Stopped = false;

}
//..,
void KrazyAudioThriver::on_pause()
{
	_AudioList[_CurrentIndex]->pause();
}
//..,
void KrazyAudioThriver::on_stop()
{
	_AudioList[_CurrentIndex]->stop();
	_Stopped = true;
}
//.,,
void KrazyAudioThriver::on_increaseVolume()
{
}
//..,
void KrazyAudioThriver::on_decreaseVolume()
{
}
//..,
void KrazyAudioThriver::on_mute()
{
}
//..,
#endif