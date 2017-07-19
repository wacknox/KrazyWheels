#pragma once 
#ifndef _WINDOWS
#include<pthread.h>
//..
class KrazyThreadLock
{
public:
	KrazyThreadLock();
	~KrazyThreadLock();
	void lock();
	void unlock();
private:
	pthread_mutex_t _MutexLock;
};
//..,
class KrazyLock
{
public:
	KrazyLock(KrazyThreadLock* _lock);
	~KrazyLock();
private:
	KrazyThreadLock* _Lock;
};
#endif