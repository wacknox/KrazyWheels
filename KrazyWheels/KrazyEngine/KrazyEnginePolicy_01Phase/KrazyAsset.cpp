#include "stdafx.h"
#include "KrazyAsset.h"
#include <stdio.h>
//..,
#ifdef _ENABLE_SINGLETON
KrazyAsset* KrazyAsset::_Me = 0;

KrazyAsset* KrazyAsset::getDefault(AAssetManager* _asset_mgr)
{
	if(!_Me){
		_Me = new KrazyAsset(_asset_mgr);
	}
	//..,
	return _Me;
}
void  KrazyAsset::releaseDefault()
{
	if(_Me){
		delete _Me; _Me = 0;
	}

}
#endif
//..,
#ifndef _WINDOWS
KrazyAssetManager* KrazyAssetManager::_Me = 0;
KrazyAssetManager* KrazyAssetManager::getDefault(AAssetManager* _asset_mgr)
{
	if(!_Me){
		_Me = new KrazyAssetManager(_asset_mgr);
	}
	//..,
	return _Me;
}
//..,
KrazyAssetManager* KrazyAssetManager::getDefault()
{
	return _Me;
}
//..,
void KrazyAssetManager::releaseDefault()
{
	if(_Me){
		delete _Me; _Me = 0;
	}
}
//..,
AAssetManager* KrazyAssetManager::asset_manager() { return _AssetManager; }
//..,
void KrazyAssetManager::reset()
{
	_AssetManager = 0;
}
//..,
KrazyAssetManager::KrazyAssetManager(AAssetManager* _asset_mgr) : 
_AssetManager(_asset_mgr)
{
}
//..,
KrazyAssetManager::~KrazyAssetManager()
{
}
//..
#ifdef _ENABLE_SINGLETON
KrazyAsset::KrazyAsset(AAssetManager* _asset_mgr) : 
_AssetManager(_asset_mgr)
{
}
#else
KrazyAsset::KrazyAsset() : 
 _Asset(0)
{
	setAssetManager();
	
}
#endif
//..,
KrazyAsset::~KrazyAsset(void)
{
}
//..,
void KrazyAsset::setAssetManager()
{
#if 0
	_KrazyAssetMgr _mgr;
	_AssetManager = _mgr._get();
#else
	_AssetManager = 0;
	KrazyAssetManager* _asset_manager = KrazyAssetManager::getDefault();
	if(_asset_manager){
		_AssetManager = _asset_manager->asset_manager();
	}
#endif
}
//..,
 bool KrazyAsset::open_asset(const char* _name ,  unsigned int _mode /*= 0*/)
{
	if(_AssetManager){
		_Asset = AAssetManager_open(_AssetManager , _name , _mode);
		if(!_Asset)
			return false;
		//..,
		return true;
	}
	//..,
	return  false;
}
//..,
int KrazyAsset::open_filedescriptor(off_t* _start , off_t* _length)
{
	return AAsset_openFileDescriptor(_Asset, _start, _length);
}
//..,
int KrazyAsset::read_asset(char* buf , int size)
{
	if(_Asset){
		return AAsset_read(_Asset , buf , size);
	}
	return -1;
}
//..,
int KrazyAsset::close_asset()
{
	if(_Asset){
		 AAsset_close(_Asset);
	}
	//..,
	return 0;
}
//..,
int KrazyAsset::read_asset(void* cookie , char* buf , int size)
{
	return 0;
}
//
int KrazyAsset::write_asset(void* cookie , char* buf , int size)
{
	return 0;
}
//..,
int KrazyAsset::seek_asset(void* cookie , fpos_t offset , int whence)
{
	return 0;
}
//..,
int KrazyAsset::close_asset(void* cookie)
{
	return 0;
}
//..,
#endif