#include "stdafx.h"
#include "KrazyThreadLock.h"
//..,
#ifndef _WINDOWS
KrazyThreadLock::KrazyThreadLock()
{
	pthread_mutex_init(&_MutexLock , 0);
}
//..,
KrazyThreadLock::~KrazyThreadLock()
{
	pthread_mutex_destroy(&_MutexLock);
}
//..,
void KrazyThreadLock::lock()
{
	pthread_mutex_lock(&_MutexLock);
}
//
void KrazyThreadLock::unlock()
{
	pthread_mutex_unlock(&_MutexLock);
}
//..,
KrazyLock::KrazyLock(KrazyThreadLock* _lock)
{
	_Lock = _lock;
	_Lock->lock();
}
//..,
KrazyLock::~KrazyLock()
{
	_Lock->unlock();
}
#endif