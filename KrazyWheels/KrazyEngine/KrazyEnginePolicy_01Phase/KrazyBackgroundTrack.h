#pragma once
#include <Windows.h>
#include <string>
#include <process.h> 
using namespace std;
#define BACKGROUND_TRACK L"assets\\music\\Wheels.mp3"
class KrazyBackgroundTrack
{
public:
	static KrazyBackgroundTrack* getDefault();
	static void releaseDefault();
	void play(wstring _file);
	void stop();
	bool is_stopped();
private:
	enum _PLAY_STATE
	{
		_State_None ,
		_State_Playing,
		_State_Stopped,
	};
	
	KrazyBackgroundTrack(void);
	virtual ~KrazyBackgroundTrack(void);
	static KrazyBackgroundTrack* _ptrMe;
	wstring m_File;
	bool m_isStopped;
};

