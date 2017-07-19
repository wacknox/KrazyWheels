#pragma once
//..,
#ifndef _WINDOWS
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#endif
#include <sys/types.h>
#include "KrazyPrimitiveDataCollections.h"
#include "KrazyEnums.h"
//..,
#ifndef _WINDOWS
class KrazyAudio
{
public :

	KrazyAudio(string _fileName);
	~KrazyAudio();
	//..
	void create(SLEngineItf _engine , SLObjectItf _output_mix_);
	void destroy();
	//..,
	void play();
	void pause();
	void stop();
	//..,
private:
	//..,
	string					_FileName;
	SLObjectItf				_PlayerObject;
	SLPlayItf				_PlayerPlay;
	SLSeekItf				_PlayerSeek;
	SLMuteSoloItf			_PlayerMuteSolo;
	SLVolumeItf				_PlayerVolume;
	
};
//..
class KrazyAudioList : public vector<KrazyAudio*> {};
//..,
enum KrazyMusicIter
{
	_KRAZY_MUSIC_GameOn , 
};
class KrazyAudioThriver
{
public:
	static KrazyAudioThriver* getDefault();
	static void releaseDefault();
	///.,
	void on_changeMusic(unsigned int _index);
	void on_play();
	void on_pause();
	void on_stop();
	void on_increaseVolume();
	void on_decreaseVolume();
	void on_mute();
	KrazyBool number_changed();
	void Initialize();
	void destroy();
	//..,
	unsigned int current_index() { return _CurrentIndex; }
	bool& stopped();
private:
	
	
	//..,
	KrazyAudioThriver();
	virtual ~KrazyAudioThriver();
	//..,
	static KrazyAudioThriver*  _Me;
	KrazyBool               _NumberChanged;
	SLObjectItf				_EngineObject;
    SLEngineItf				_Engine;
	SLObjectItf				_OutputMixObject;
	KrazyAudioList		    _AudioList;
	unsigned int            _CurrentIndex;
	bool                    _Stopped;
};
#endif