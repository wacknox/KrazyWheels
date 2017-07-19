#include "StdAfx.h"
#include "KrazyBackgroundTrack.h"

KrazyBackgroundTrack* KrazyBackgroundTrack::_ptrMe = 0;

KrazyBackgroundTrack::KrazyBackgroundTrack(void) :  m_isStopped(false)
{
}


KrazyBackgroundTrack::~KrazyBackgroundTrack(void) 
{
}

KrazyBackgroundTrack* KrazyBackgroundTrack::getDefault()
{
	if(!_ptrMe)
	{
		_ptrMe = new KrazyBackgroundTrack;
	}

	return _ptrMe; 
}

void KrazyBackgroundTrack::releaseDefault()
{

	if(_ptrMe)
	{
		delete _ptrMe;_ptrMe = 0;
	}
}

bool KrazyBackgroundTrack::is_stopped()
{
	return m_isStopped;
}

void KrazyBackgroundTrack::play(wstring _file)
{
	m_File = _file;
	wstring _command = L"open ";
	_command.append(m_File);
	_command.append(L" type mpegvideo alias song1");
	MCIERROR me =  mciSendString(_command.c_str(),  NULL, 0, 0);  
    if(me == 0){
		me =  mciSendString(L"play song1 repeat", NULL, 0, 0);
		m_isStopped = false;
    }
	  	
}

void KrazyBackgroundTrack::stop()
{
	wstring _command = L"close song1";
	mciSendString(_command.c_str() , NULL, 0, 0);
	m_isStopped = true;
}
	